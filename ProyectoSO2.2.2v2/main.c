#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

int t_pag[256];//tabla de paginas
int tlb[16][2];//tlb, 16 entradas que contienen un numero de pagina con su frame asociado
int memoria[256][256];//memoria fisica, 256 frames y 8 bits de offset
int ult;//proxima ubicacion del tlb en la cual insertar

int buscar(int page, int offset){
    //dado un numero de pagina y un offset, retorna los datos en la memoria
    //devuelve -1 en caso de que no se encuentre en memoria
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
    else{//si no esta en el tlb, busco en la tabla de pagina
            frame = t_pag[page];
            tlb[ult][0]=page;
            tlb[ult][1]=frame;
            ult=(ult+1) % 16;
            printf("[tabla]");
    }
    return memoria[frame][offset];
}

void iniciar(){
    //inicio los arreglos con valores aleatorios
    long int i;
    for(i=0;i<16;i++){
        tlb[i][0] = rand()%256;
        tlb[i][1] = rand()%256;
    }
    for(i=0;i<256;i++){
        t_pag[i] = rand()%256;
    }
    for(i=0;i<256;i++){
        int j;
        for(j=0;j<256;j++){
            memoria[i][j] = rand()%8;
        }
    }
}

int main()
{
    iniciar();
    ult = 0;
    FILE *fp = fopen("memoria.txt", "r");
    char buffer[18];
    char *ptr;
    unsigned int address;
    int cant = 0;
    while(fgets(buffer,18,fp)!=NULL){
        cant++;
        printf("%d_",cant);
        if(strlen(buffer)!=17){
            printf("formato de memoria incorrecto\n\n");
        }else{
            address = strtol(buffer,&ptr,2);//base 2, puedo poner la que sea dependiendo de el archivo de entrada.
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
    }
    fclose(fp);
    return 0;
}
