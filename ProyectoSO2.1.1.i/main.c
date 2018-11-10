#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int pipes[5][2];
pid_t p[5];
char *sg ="1";

void fun_A(){
    close(pipes[0][0]);
    close(pipes[1][0]);
    close(pipes[1][1]);
    close(pipes[2][0]);
    close(pipes[2][1]);
    close(pipes[3][0]);
    close(pipes[3][1]);
    close(pipes[4][1]);
    while(1){
        char c[2];
        read(pipes[4][0],c,2);
        printf("A");
        fflush(stdout);
        write(pipes[0][1],sg,2);
    }
}

void fun_B(){
    close(pipes[0][1]);
    close(pipes[1][0]);
    close(pipes[2][0]);
    close(pipes[2][1]);
    close(pipes[3][0]);
    close(pipes[3][1]);
    close(pipes[4][0]);
    close(pipes[4][1]);
    while(1){
        char c[2];
        read(pipes[0][0],c,2);
        printf("B");
        fflush(stdout);
        write(pipes[1][1],sg,2);
    }
}

void fun_C(){
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][1]);
    close(pipes[2][0]);
    close(pipes[3][0]);
    close(pipes[3][1]);
    close(pipes[4][0]);
    close(pipes[4][1]);
    while(1){
        char c[2];
        read(pipes[1][0],c,2);
        printf("C");
        fflush(stdout);
        write(pipes[2][1],"D",2);
    }
}

void fun_D(){
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][0]);
    close(pipes[1][1]);
    close(pipes[2][1]);
    close(pipes[3][0]);
    close(pipes[4][0]);
    close(pipes[4][1]);
    while(1){
        char c[2];
        read(pipes[2][0],c,2);
        printf("D");
        fflush(stdout);
        write(pipes[3][1],sg,2);
    }
}

void fun_E(){
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][0]);
    close(pipes[1][1]);
    close(pipes[2][0]);
    close(pipes[2][1]);
    close(pipes[3][1]);
    close(pipes[4][0]);
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
    int i;
    for(i=0;i<5;i++){
        p[i] = fork();
        if(p[i]<0){
            printf("fork failed");
            exit(1);
        }
        if(p[i]==0){
            if(i==0){
                fun_A();
            }
            if(i==1){
                fun_B();
            }
            if(i==2){
                fun_C();
            }
            if(i==3){
                fun_D();
            }
            if(i==4){
                fun_E();
            }
        }
    }
}

int main()
{
    crearPipes();
    crearProcesos();
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][0]);
    close(pipes[1][1]);
    close(pipes[2][0]);
    close(pipes[2][1]);
    close(pipes[3][0]);
    close(pipes[3][1]);
    close(pipes[4][0]);
    write(pipes[4][1],sg,2);
    close(pipes[4][1]);
    while(wait(NULL)){//espero a que terminen los procesos
        if (errno == ECHILD){
            break;
        }
    }



    return 0;
}
