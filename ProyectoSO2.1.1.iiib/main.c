#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define TAM (sizeof(aux)-sizeof(long))

pid_t p[5];

typedef struct estructura{
    long tipo;
} aux;

void fun_A(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux a;
    aux b;
    aux c;
    aux aob;
    a.tipo=1;
    b.tipo=2;
    c.tipo=3;
    aob.tipo=6;
    while(1){
        sleep(1);
        msgrcv (identificador,(struct msgbuf *)&a, TAM,1, 0);//espero a que llegue un mensaje de tipo a bloqueante, no avanzo hasta no recibir
        msgrcv (identificador,(struct msgbuf *)&aob, TAM,6, 0);//espero a que llegue un mensaje de tipo aob bloqueante, no avanzo hasta no recibir
        printf("A");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&c, TAM,IPC_NOWAIT);//envio un mensaje de tipo c
        msgsnd (identificador,(struct msgbuf *)&b, TAM,IPC_NOWAIT);//envio un mensaje de tipo b
        msgrcv (identificador,(struct msgbuf *)&a, TAM,1, 0);//espero a que llegue un mensaje de tipo a bloqueante, no avanzo hasta no recibir
        msgrcv (identificador,(struct msgbuf *)&aob, TAM,6, 0);//espero a que llegue un mensaje de tipo aob bloqueante, no avanzo hasta no recibir
        printf("A");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&b, TAM,IPC_NOWAIT);//envio un mensaje de tipo b

    }
}

void fun_B(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux a;
    aux b;
    aux c;
    aux aob;
    a.tipo=1;
    b.tipo=2;
    c.tipo=3;
    aob.tipo=6;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&b, TAM,2, 0);//espero a que llegue un mensaje de tipo b bloqueante, no avanzo hasta no recibir
        msgrcv (identificador,(struct msgbuf *)&aob, TAM,6, 0);//espero a que llegue un mensaje de tipo aob bloqueante, no avanzo hasta no recibir
        printf("B");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&c, TAM,IPC_NOWAIT);//envio un mensaje de tipo c
        msgsnd (identificador,(struct msgbuf *)&a, TAM,IPC_NOWAIT);//envio un mensaje de tipo a
        msgrcv (identificador,(struct msgbuf *)&b, TAM,2, 0);//espero a que llegue un mensaje de tipo b bloqueante, no avanzo hasta no recibir
        printf("B");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&c, TAM,IPC_NOWAIT);//envio un mensaje de tipo c
        msgsnd (identificador,(struct msgbuf *)&a, TAM,IPC_NOWAIT);//envio un mensaje de tipo a

    }
}

void fun_C(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux c;
    aux d;
    c.tipo=3;
    d.tipo=4;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&c, TAM,3, 0);//espero a que llegue un mensaje de tipo c bloqueante, no avanzo hasta no recibir
        printf("C");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&d, TAM,IPC_NOWAIT);//envio un mensaje de tipo d
    }
}

void fun_D(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux d;
    aux e;
    d.tipo=4;
    e.tipo=5;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&d, TAM,4, 0);//espero a que llegue un mensaje de tipo d bloqueante, no avanzo hasta no recibir
        printf("D");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&e, TAM,IPC_NOWAIT);//envio un mensaje de tipo e
    }
}

void fun_E(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux aob;
    aux e;
    aob.tipo=6;
    e.tipo=5;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&e, TAM,5, 0);//espero a que llegue un mensaje de tipo e bloqueante, no avanzo hasta no recibir
        printf("E\n");//el salto de linea es para facilitar la lectura, la secuencia deberia ser de corrido
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&aob, TAM,IPC_NOWAIT);//envio un mensaje de tipo aob
    }
}

void crearProcesos(int identificador){
    int i;
    for(i=0;i<5;i++){
        p[i] = fork();
        if(p[i]<0){
            printf("fork failed");
            exit(1);
        }
        if(p[i]==0){
            if(i==0){
                fun_A(identificador);
            }
            if(i==1){
                fun_B(identificador);
            }
            if(i==2){
                fun_C(identificador);
            }
            if(i==3){
                fun_D(identificador);
            }
            if(i==4){
                fun_E(identificador);
            }
        }
    }
}

int main()
{
    //Obtengo la clave;
    char path[200];
    getcwd(path,200*sizeof(char));
    key_t clave=ftok(path,33);
    //Veo si se produjo error.
    if(clave==(key_t)-1){
        printf("Error al obtener la clave.\n");
        exit(1);
    }
    //creo, elimino y vuelvo a crear la cola para vaciar mensajes anteriores
    //esto no es necesario hacerlo desde aqui y se puede hacer desde la consola
    //pero para facilitar las ejecuciones sucesivas se agrega en el programa
    int identificador;
    //Creo
    identificador = msgget (clave, 0600| IPC_CREAT );
    //Elimino
    msgctl(identificador,IPC_RMID,0);
    //Vuelvo a crear
    identificador=msgget(clave,0600|IPC_CREAT);
    //Veo si se produjo error.
    if(identificador==-1){
        printf("Error al obtener el identificador.\n");
        exit(1);
    }
    //defino los tipos de mensaje que necesito recibir y enviar
    aux a;
    aux aob;
    a.tipo=1;
    aob.tipo=6;
    //Mensaje inicial para comenzar la secuencia
    msgsnd (identificador,(struct msgbuf *)&a, TAM, IPC_NOWAIT);
    msgsnd (identificador,(struct msgbuf *)&aob, TAM, IPC_NOWAIT);

    crearProcesos(identificador);
    while(waitpid(-1, NULL, 0)){
        if(errno == ECHILD){
            break;
        }
    }
    return 0;
}
