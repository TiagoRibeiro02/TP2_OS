#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FIFO1 "/tmp/f1"
#define PERMS 0666

int pipe1[2];

int main(int argc,char *argv[])
{
    int numDePipetas= atoi(argv[1]), max = atoi(argv[4]), tempoDeEspera= atoi(argv[3]);
    srand(time(NULL));
    float prob= atof(argv[2]);
    int pid= fork();
    if(pid==0){
        pipe1[0] = open(FIFO1,0);
        int pipetasDeLeitura[numDePipetas][2];
        for(int i = 0;i<numDePipetas;i++ ){
            char * bufferLei = malloc(15);
            sprintf(bufferLei,"/tmp/f%d",i+2);
            //limpar
            pipetasDeLeitura[i][0] = open(bufferLei, 0);
            memset(bufferLei,0,15);
            free(bufferLei);
        }
    
        int token= 0;
        int change=0;
        int pipeta=0;
        while((read(pipe1[0],&token ,sizeof(int)))< max && token< max ){
            if(change != token){
                read(pipetasDeLeitura[token % numDePipetas ][0],&pipeta,sizeof(int));
                printf("[p%d] blocked on token (val = %d)\n",pipeta+1,token);
                sleep(tempoDeEspera);
                change=token;
            }
        
        }
        close(pipe1[0]);
        for(int i =0 ; i<numDePipetas; i++){
            close(pipetasDeLeitura[i][0]);
        }
    }
    else{
        mknod(FIFO1, S_IFIFO | PERMS, 0);
        pipe1[1] = open(FIFO1,1);
        int pipetasDeEscrita[numDePipetas ][2];
        for(int i = 0;i<numDePipetas  ;i++){
            char * bufferEsc = malloc(15);
            sprintf(bufferEsc,"/tmp/f%d",i+2);
            //limpar
            mknod(bufferEsc,S_IFIFO | PERMS ,0);
            pipetasDeEscrita[i][1] = open(bufferEsc,1);
            memset(bufferEsc,0,15);
            //unlink(bufferEsc);
            free(bufferEsc);
        }
    
        int token=0;
        int i=0;
        while (token<= max){  
            if(token == max)
            {
                //printf("FIM\n");
                write(pipe1[1],&token,sizeof(int) );
                break;
            }  
            if(((int) (prob * 100)) >= (rand() % 100 + 1)){
                //printf("OCORREU, %d\n", i);
                write(pipe1[1],&token,sizeof(int));
                write(pipetasDeEscrita[i][1],&(i),sizeof(int));
            
            }
            if(i== numDePipetas- 1){
                i=0;
            }
            else{
                i++;
            }
        
            token++;
        }
        //unlink(FIFO1);
        close(pipe1[1]);
   
        for(int i = 0; i<numDePipetas ; i++){
            close(pipetasDeEscrita[i][1]);
        }
        wait(NULL);
    }
    return 0;
}