#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FIFO1 "/tmp/f1"
#define FIFO2 "/tmp/f2"
#define FIFO3 "/tmp/f3"
#define FIFO4 "/tmp/f4"
#define FIFO5 "/tmp/f5"
#define PERMS 0666

int pipe1[2],pipe2[2],pipe3[2],pipe4[2],pipe5[2];

void server(){
    int numeroDePipetas,max,tempoDeEspera;//x,y,res;
    float prob;
    mknod(FIFO1, S_IFIFO | PERMS, 0);
    mknod(FIFO2, S_IFIFO | PERMS, 0);
    mknod(FIFO3, S_IFIFO | PERMS, 0);
    mknod(FIFO4, S_IFIFO | PERMS, 0);
    mknod(FIFO5, S_IFIFO | PERMS, 0);
    pipe1[0] = open(FIFO1,0); // 0 leitura
    pipe2[0] = open(FIFO2,0); // 1 escrita
    pipe3[0] = open(FIFO3,0);
    pipe4[0] = open(FIFO4,0);
    pipe5[1] = open(FIFO5,1);

    read(pipe1[0], &numeroDePipetas,sizeof(int));
    read(pipe2[0],&max,sizeof(int));
    read(pipe3[0],&tempoDeEspera,sizeof(int));
    read(pipe4[0],&prob,sizeof(float));
   // printf("Debug1\n");
    /*int * pipetasDeLeitura = malloc(numeroDePipetas);
    int * pipetasDeEscrita = malloc(numeroDePipetas);
    for(int i = 1; i<= numeroDePipetas; i++)
    {
       pipetasDeLeitura[i] =(int *) malloc(2 * sizeof(int));
       pipetasDeEscrita[i] =(int*)malloc(2 * sizeof(int));
    }*/
    //int pipetasDeLeitura[numeroDePipetas*2 ][2];
    int pipetasDeEscrita[numeroDePipetas ][2];
    for(int i = 0;i<numeroDePipetas  ;i++){
        
        //char * bufferLei = malloc(15);
        char * bufferEsc = malloc(15);
        //sprintf(bufferLei,"/tmp/f%d",i+7);
        sprintf(bufferEsc,"/tmp/f%d",i+6);
        //printf("%d || %d\n",i,numeroDePipetas);
        //mknod(bufferLei,S_IFIFO | PERMS ,0);//limpar
        mknod(bufferEsc,S_IFIFO | PERMS ,0);
       // printf("%d || %d\n",i,numeroDePipetas);
        //pipetasDeLeitura[i/2][0] = open(bufferLei, 0);
        pipetasDeEscrita[i][1] = open(bufferEsc,1);
        //printf("%d || %d\n",i,numeroDePipetas);
        //memset(bufferLei,0,15);
        memset(bufferEsc,0,15);
        free(bufferEsc);
    }
    int token=0;
    int i=0;
    //printf("Debug1\n");
    while (token<= max){  
       // printf("%d\n",token);
        if(token == max)
        {
            //printf("FIM\n");
            write(pipe5[1],&token,sizeof(int) );
            break;
        } 
        //read(pipe1[0], &x,sizeof(int));
        //read(pipe1[0], &y,sizeof(int));
        //read(pipe1[0] , &numeroDePipetas ,sizeof(int));
        //write(pipe2[1], &numeroDePipetas, sizeof(int));
        //res = x + y;

        //write(pipe2[1],&res, sizeof(int)); 
        if(((int) (prob * 100)) >= (rand() % 100 + 1)){
            //printf("OCORREU, %d\n", i);
            write(pipe5[1],&token,sizeof(int));
            write(pipetasDeEscrita[i][1],&(i),sizeof(int));
            
        }
        if(i== numeroDePipetas- 1){
            i=0;
        }
        else{
            i++;
        }
        
        token++;
        ///
    }

    return;
}

int main()
{
    server();
    return 0;
}
