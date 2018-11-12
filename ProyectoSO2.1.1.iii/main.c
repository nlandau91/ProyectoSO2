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
int pipe_ab[2];
pid_t p[5];
char *sg = "1";

void fun_A(){
    //cierro los pipes que no voy a usar
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_c[0]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_e[1]);
    close(pipe_ab[1]);
    while(1){
        sleep(1);
        char c[2];
        read(pipe_a[0],c,2);//me habilita B
        read(pipe_ab[0],c,2);
        printf("A");
        fflush(stdout);
        write(pipe_c[1],sg,2);//habilito a C inmediatamente
        write(pipe_b[1],sg,2);//la proxima no es una A, es una B
        read(pipe_a[0],c,2);//me habilita B
        read(pipe_ab[0],c,2);
        printf("A");
        fflush(stdout);
        write(pipe_b[1],sg,2);//habilito a B
    }
}

void fun_B(){
    //cierro los pipes que no voy a usar
    close(pipe_a[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_e[1]);
    close(pipe_ab[1]);
    while(1){
        char c[2];
        read(pipe_b[0],c,2);//me habilita A
        read(pipe_ab[0],c,2);
        printf("B");
        fflush(stdout);
        write(pipe_c[1],sg,2);//habilito a C inmediatamente
        write(pipe_a[1],sg,2);//la proxima no es una B, es una A
        read(pipe_b[0],c,2);
        printf("B");
        fflush(stdout);
        write(pipe_c[1],sg,2);//habilito a C inmediatamente
        write(pipe_a[1],sg,2);//cuando termine esta secuencia empieza por A denuevo
    }
}

void fun_C(){
    //cierro los pipes que no voy a usar
    close(pipe_a[0]);
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[1]);
    close(pipe_d[0]);
    close(pipe_e[0]);
    close(pipe_e[1]);
    close(pipe_ab[0]);
    close(pipe_ab[1]);
    while(1){
        char c[2];
        read(pipe_c[0],c,2);//me habilita tanto A como B
        printf("C");
        fflush(stdout);
        write(pipe_d[1],sg,2);
    }
}

void fun_D(){
    //cierro los pipes que no voy a usar
    close(pipe_a[0]);
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_c[1]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_ab[0]);
    close(pipe_ab[1]);
    while(1){
        char c[2];
        read(pipe_d[0],c,2);
        printf("D");
        fflush(stdout);
        write(pipe_e[1],sg,2);
    }
}

void fun_E(){
    //cierro los pipes que no voy a usar
    close(pipe_a[0]);
    close(pipe_a[1]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_c[1]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[1]);
    close(pipe_ab[0]);
    while(1){
        char c[2];
        read(pipe_e[0],c,2);
        printf("E\n");//el salto de linea es para facilitar la lectura, la secuencia deberia ser de corrido
        fflush(stdout);
        write(pipe_ab[1],sg,2);//habilito a A o B, el que toque
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
    if(pipe(pipe_ab)==-1){
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
    printf("La secuencia es ACDEBCDEABCDE...\n");

    crearPipes();
    crearProcesos();
    //cierro los pipes que no voy a usar
    close(pipe_a[0]);
    close(pipe_b[0]);
    close(pipe_b[1]);
    close(pipe_c[0]);
    close(pipe_c[1]);
    close(pipe_d[0]);
    close(pipe_d[1]);
    close(pipe_e[0]);
    close(pipe_e[1]);
    close(pipe_ab[0]);
    write(pipe_a[1],sg,2);//estos writes comienzan la secuencia, luego cierro los pipes
    write(pipe_ab[1],sg,2);
    close(pipe_a[1]);
    close(pipe_ab[1]);
    while(waitpid(-1, NULL, 0)){
        if(errno == ECHILD){
            break;
        }
    }
    return 0;
}
