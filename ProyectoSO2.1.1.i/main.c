#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int pipes[5][2];
int pid;
char *sg ="1";

void fun_A(){

    while(1){
        char c[2];
        read(pipes[4][0],c,2);
        printf("A");
        fflush(stdout);
        write(pipes[0][1],sg,2);
    }
}

void fun_B(){

    while(1){
        char c[2];
        read(pipes[0][0],c,2);
        printf("B");
        fflush(stdout);
        write(pipes[1][1],sg,2);
    }
}

void fun_C(){

    while(1){
        char c[2];
        read(pipes[1][0],c,2);
        printf("C");
        fflush(stdout);
        write(pipes[2][1],"D",2);
    }
}

void fun_D(){

    while(1){
        char c[2];
        read(pipes[2][0],c,2);
        printf("D");
        fflush(stdout);
        write(pipes[3][1],sg,2);
    }
}

void fun_E(){

    while(1){
        char c[2];
        read(pipes[3][0],c,2);
        printf("E");
        fflush(stdout);
        sleep(1);
        write(pipes[4][1],sg,2);
    }
}

void crearPipes(){
    int i;
    for(i=0;i<5;i++){
        if(pipe(pipes[i])==-1){
            perror("Error pipe");
            exit(EXIT_FAILURE);
        }
    }
}

void crearProcesos(){

    pid = fork();
    if(pid<0){
        printf("fork failed");
        exit(1);
    }
    if(pid==0){
        fun_A();
    }

    pid = fork();
    if(pid<0){
        printf("fork failed");
        exit(1);
    }
    if(pid==0){
        fun_B();
    }

    pid = fork();
    if(pid<0){
        printf("fork failed");
        exit(1);
    }
    if(pid==0){
        fun_C();
    }

    pid = fork();
    if(pid<0){
        printf("fork failed");
        exit(1);
    }
    if(pid==0){
        fun_D();
    }

    pid = fork();
    if(pid<0){
        printf("fork failed");
        exit(1);
    }
    if(pid==0){
        fun_E();
    }

}

int main()
{
    crearPipes();
    crearProcesos();
    write(pipes[4][1],sg,2);
    while(wait(NULL)){//espero a que terminen los procesos
        if (errno == ECHILD){
            break;
        }
    }



    return 0;
}
