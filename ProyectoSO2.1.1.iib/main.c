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
    aux abc;
    aux d;
    abc.tipo=1;
    d.tipo=2;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&abc, TAM,1, 0);
        printf("A");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&d, TAM,IPC_NOWAIT);

    }
}

void fun_B(int identificador){
    aux abc;
    aux d;
    abc.tipo=1;
    d.tipo=2;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&abc, TAM,1, 0);
        printf("B");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&d, TAM,IPC_NOWAIT);
    }
}

void fun_C(int identificador){
    aux abc;
    aux d;
    abc.tipo=1;
    d.tipo=2;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&abc, TAM,1, 0);
        printf("C");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&d, TAM,IPC_NOWAIT);
    }
}

void fun_D(int identificador){
    aux d;
    aux e;
    d.tipo=2;
    e.tipo=3;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&d, TAM,2, 0);
        msgrcv (identificador,(struct msgbuf *)&d, TAM,2, 0);
        printf("D");
        fflush(stdout);
        msgsnd (identificador,(struct msgbuf *)&e, TAM,IPC_NOWAIT);
    }
}

void fun_E(int identificador){
    aux abc;
    aux e;
    abc.tipo=1;
    e.tipo=3;
    while(1){
        msgrcv (identificador,(struct msgbuf *)&e, TAM,3, 0);
        printf("E");
        fflush(stdout);
        sleep(1);
        msgsnd (identificador,(struct msgbuf *)&abc, TAM,IPC_NOWAIT);
        msgsnd (identificador,(struct msgbuf *)&abc, TAM,IPC_NOWAIT);
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
    aux abc;
    int identificador;

   //Obtengo la clave;
    key_t clave=ftok("/bin/ls",33);
    //Veo si se produjo error.
    if(clave==(key_t)-1){
        printf("Error al obtener la clave.\n");
        exit(1);
    }

    /**Debo crear, eliminar y volver a crear, pues pueden quedar mensajes anteriores.*/
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

    //Seteo los tipos.
    abc.tipo=1;

    //Mensaje inicial
    msgsnd (identificador,(struct msgbuf *)&abc, TAM, IPC_NOWAIT);
    msgsnd (identificador,(struct msgbuf *)&abc, TAM, IPC_NOWAIT);

    crearProcesos(identificador);

    while (waitpid(-1, NULL, 0)) {
           if (errno == ECHILD) {
              break;
           }
    }

    return 0;
}
