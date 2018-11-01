#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int pipe_abc[2];
int pipe_d[2];
int pipe_e[2];
int pid;
char *sg = "1";

void fun_A(){

    while(1){
        char c[2];
        read(pipe_abc[0],c,2);
        printf("A");
        fflush(stdout);
        write(pipe_d[1],sg,2);
    }
}

void fun_B(){

    while(1){
        char c[2];
        read(pipe_abc[0],c,2);
        printf("B");
        fflush(stdout);
        write(pipe_d[1],sg,2);
    }
}

void fun_C(){

    while(1){
        char c[2];
        read(pipe_abc[0],c,2);
        printf("C");
        fflush(stdout);
        write(pipe_d[1],sg,2);
    }
}

void fun_D(){

    while(1){
        char c[2];
        read(pipe_d[0],c,2);
        read(pipe_d[0],c,2);
        printf("D");
        fflush(stdout);
        write(pipe_e[1],sg,2);
    }
}

void fun_E(){

    while(1){
        char c[2];
        read(pipe_e[0],c,2);
        printf("E");
        fflush(stdout);
        sleep(1);
        write(pipe_abc[1],sg,2);
        write(pipe_abc[1],sg,2);
    }
}

void crearPipes(){

    if(pipe(pipe_abc)==-1){
        perror("Error pipe");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe_d)==-1){
        perror("Error pipe");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe_e)==-1){
        perror("Error pipe");
        exit(EXIT_FAILURE);
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
    write(pipe_abc[1],sg,2);
    write(pipe_abc[1],sg,2);
    while(wait(NULL)){
        if (errno == ECHILD){
            break;
        }
    }

    return 0;
}
