#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define FIFO1 "/tmp/f1"
#define FIFO2 "/tmp/f2"
#define FIFO3 "/tmp/f3"
#define FIFO4 "/tmp/f4"
#define FIFO5 "/tmp/f5"
#define PERMS 0666

int pipe1[2], pipe2[2], pipe3[2], pipe4[2], pipe5[2];


void cliente(){
    
    int numDePipetas, max, tempoDeEspera;//x,y, res;
    float prob;
    pipe1[1] = open(FIFO1,1);
    pipe2[1] = open(FIFO2,1);
    pipe3[1] = open(FIFO3,1);
    pipe4[1] = open(FIFO4,1);
    pipe5[0] = open(FIFO5,0);

    scanf("%i %i %i", &numDePipetas, &max, &tempoDeEspera);
    scanf(" %f",&prob);
    write(pipe1[1], &numDePipetas, sizeof(int));
    write(pipe2[1], &max, sizeof(int));
    write(pipe3[1], &tempoDeEspera, sizeof(int));
    write(pipe4[1], &prob, sizeof(float));
    int pipetasDeLeitura[numDePipetas][2];
    //int pipetasDeEscrita[numDePipetas*2][2];
    for(int i = 0;i<numDePipetas   ;i++ ){
        char * bufferLei = malloc(15);
        //char * bufferEsc = malloc(15);
        sprintf(bufferLei,"/tmp/f%d",i+6);
        //sprintf(bufferEsc,"/tmp/f%d",i+6);
        //limpar
        //mudar no server
       // printf("%d || %d\n",i,numDePipetas);
        pipetasDeLeitura[i][0] = open(bufferLei, 0);
        //pipetasDeEscrita[i/2][1] = open(bufferEsc,1);
        //printf("%d || %d\n",i,numDePipetas);
        memset(bufferLei,0,15);
        free(bufferLei);
        //memset(bufferEsc,0,15);
    }
    
    /*while (1)
    {
        
        
        read(pipe2[0], &res, sizeof(int));
        
        printf("%d \n", res);
    }
*/
    int token= 0;
    int change=0;
    int pipeta=0;
    while((read(pipe5[0],&token ,sizeof(int)))< max && token< max ){
        if(change != token){
            read(pipetasDeLeitura[token % numDePipetas ][0],&pipeta,sizeof(int));//será?
            printf("[p%d] blocked on token (val = %d)\n",pipeta+1,token);
            sleep(tempoDeEspera );
            change= token;
        }
        
    }

    return; 
}

int main()
{

    cliente();
    
    return 0;
}
