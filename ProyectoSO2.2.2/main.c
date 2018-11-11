#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int t_pag[65536][2];
int tlb[16][2];
int memoria[256][256];
int ult;

int buscar(int page, int offset){

    int esta=0;
    int i=0;
    int frame;
    while(i<16 && !esta){//busco en el tlb
        esta=tlb[i][0]==page;
        i++;
    }
    if(esta){//esta en el tlb, recupero el frame
        frame = tlb[i-1][1];
        printf("[tlb]");
    }
    else{//si no esta en el tlb, busco en la tabla de paginas
        i=0;
        while(i<65536 && !esta){
            esta=t_pag[i][0]==page;
            i++;
        }
        if(esta){//esta en la tabla de paginas, recupero el frame y agrego al tlb
            frame = t_pag[i-1][1];
            tlb[ult][0]=page;
            tlb[ult][1]=frame;
            ult=(ult+1) % 16;
            printf("[tabla]");
        }
        else{
            //page fault
            return -1;
        }
    }
    return memoria[frame][offset];
}



void iniciar(){
    long int i;
    for(i=0;i<16;i++){
        tlb[i][0] = rand()%256;//8 bits de pagina
        tlb[i][1] = rand()%256;//8 bits de offset
    }
    for(i=0;i<65536;i++){
        t_pag[i][0] = rand()%256;
        t_pag[i][1] = rand()%256;
    }
    for(i=0;i<256;i++){
        int j;
        for(j=0;j<256;j++){
            memoria[i][j] = rand()%512;//cualquier tamano de dato
        }
    }
}

int main()
{
    iniciar();
    ult = 0;
    FILE *fp = fopen("memoria.txt", "r");
    char buffer[16];
    char *ptr;
    unsigned int address;
    while(fgets(buffer,18,fp)!=NULL){
        address = strtol(buffer,&ptr,2);//base 2, puedo poner la que sea.
        if(address > 65536){
            printf("error rango de memoria\n");
            exit(1);
        }
        int page = address >> 8;
        int offset = address & 0xFF;
        printf("Direccion: %x, Pagina : %x, Offset : %x\n", address,page,offset);
            int data = buscar(page,offset);
            if(data == -1){
                printf("no se encuentra la pagina para la direccion %x\n\n",address);
            }
            else{
                printf("los datos en la direccion %x son %x\n\n",address,data);
            }
    }

    while(wait(NULL)){//espero a que terminen los procesos
        if (errno == ECHILD){
            break;
        }
    }
    fclose(fp);
    return 0;
}
