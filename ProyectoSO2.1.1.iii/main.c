#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int pipe_a[2];
int pipe_b[2];
int pipe_c[2];
int pipe_d[2];
int pipe_e[2];
int pipe_aux[2];
pid_t p[5];
char *sg = "1";

void fun_A(){
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_c[0]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_e[1]);
    close(pipe_aux[1]);
    while(1){
        char c[2];
        read(pipe_a[0],c,2);
        read(pipe_aux[0],c,2);
        printf("A");
        fflush(stdout);
        write(pipe_c[1],sg,2);
        write(pipe_b[1],sg,2);
        read(pipe_a[0],c,2);
        read(pipe_aux[0],c,2);
        printf("A");
        fflush(stdout);
        write(pipe_b[1],sg,2);
    }
}

void fun_B(){
    close(pipe_a[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_e[1]);

    close(pipe_aux[1]);
    while(1){
        char c[2];
        read(pipe_b[0],c,2);
        read(pipe_aux[0],c,2);
        printf("B");
        fflush(stdout);
        write(pipe_c[1],sg,2);
        write(pipe_a[1],sg,2);
        read(pipe_b[0],c,2);
        printf("B");
        fflush(stdout);
        write(pipe_c[1],sg,2);
        write(pipe_a[1],sg,2);
    }
}

void fun_C(){
    close(pipe_a[0]);
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[1]);
    close(pipe_d[0]);
    close(pipe_e[0]);
    close(pipe_e[1]);
    close(pipe_aux[0]);
    close(pipe_aux[1]);
    while(1){
        char c[2];
        read(pipe_c[0],c,2);
        printf("C");
        fflush(stdout);
        write(pipe_d[1],sg,2);
    }
}

void fun_D(){
    close(pipe_a[0]);
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_c[1]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_aux[0]);
    close(pipe_aux[1]);
    while(1){
        char c[2];
        read(pipe_d[0],c,2);
        printf("D");
        fflush(stdout);
        write(pipe_e[1],sg,2);
    }
}

void fun_E(){
    close(pipe_a[0]);
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_c[1]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[1]);
    close(pipe_aux[0]);
    while(1){
        char c[2];
        read(pipe_e[0],c,2);
        printf("E");
        fflush(stdout);
        sleep(1);
        write(pipe_aux[1],sg,2);
    }
}

void crearPipes(){

    if(pipe(pipe_a)==-1){
        perror("Error pipe");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe_b)==-1){
        perror("Error pipe");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe_c)==-1){
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
    if(pipe(pipe_aux)==-1){
        perror("Error pipe");
        exit(EXIT_FAILURE);
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
    close(pipe_a[0]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_c[1]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_e[1]);
    close(pipe_aux[0]);
    write(pipe_a[1],sg,2);
    write(pipe_aux[1],sg,2);
    close(pipe_a[1]);
    close(pipe_aux[1]);
    while(wait(NULL)){//espero a que terminen los procesos
        if (errno == ECHILD){
            break;
        }
    }
    return 0;
}
