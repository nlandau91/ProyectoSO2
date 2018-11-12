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
    aux abc;
    aux d;
    abc.tipo=1;
    d.tipo=2;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&abc, TAM,1, 0);//espero a que llegue un mensaje de tipo abc bloqueante, no avanzo hasta no recibir
        printf("A");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&d, TAM,IPC_NOWAIT);//envio un mensaje de tipo d

    }
}

void fun_B(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux abc;
    aux d;
    abc.tipo=1;
    d.tipo=2;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&abc, TAM,1, 0);//espero a que llegue un mensaje de tipo abc bloqueante, no avanzo hasta no recibir
        printf("B");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&d, TAM,IPC_NOWAIT);//envio un mensaje de tipo d
    }
}

void fun_C(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux abc;
    aux d;
    abc.tipo=1;
    d.tipo=2;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&abc, TAM,1, 0);//espero a que llegue un mensaje de tipo abc bloqueante, no avanzo hasta no recibir
        printf("C");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&d, TAM,IPC_NOWAIT);//envio un mensaje de tipo d
    }
}

void fun_D(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux d;
    aux e;
    d.tipo=2;
    e.tipo=3;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&d, TAM,2, 0);//espero a que llegue un mensaje de tipo d bloqueante, no avanzo hasta no recibir
        msgrcv (identificador,(struct msgbuf *)&d, TAM,2, 0);//necesito recibir dos veces para avanzar
        printf("D");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&e, TAM,IPC_NOWAIT);//envio un mensaje de tipo e
    }
}

void fun_E(int identificador){
    //defino los tipos de mensaje que necesito recibir y enviar
    aux abc;
    aux e;
    abc.tipo=1;
    e.tipo=3;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&e, TAM,3, 0);//espero a que llegue un mensaje de tipo e bloqueante, no avanzo hasta no recibir
        printf("E\n");//el salto de linea es para facilitar la lectura, la secuencia deberia ser de corrido
        fflush(stdout);
        sleep(1);
        msgsnd (identificador,(struct msgbuf *)&abc, TAM,IPC_NOWAIT);//envio un mensaje de tipo abc
        msgsnd (identificador,(struct msgbuf *)&abc, TAM,IPC_NOWAIT);//envio dos ya que asi lo requiere la secuencia
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
    printf("La secuencia es (A o B o C)(A o B o C)DE...\n");
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
    aux abc;
    abc.tipo=1;
    //Mensaje inicial para comenzar la secuencia
    msgsnd (identificador,(struct msgbuf *)&abc, TAM, IPC_NOWAIT);
    msgsnd (identificador,(struct msgbuf *)&abc, TAM, IPC_NOWAIT);

    crearProcesos(identificador);
    while(waitpid(-1, NULL, 0)){
        if(errno == ECHILD){
            break;
        }
    }
    return 0;
}
