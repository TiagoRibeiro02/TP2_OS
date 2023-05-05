#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#define FIFO1 "/tmp/f1"
#define FIFO3 "/tmp/f3"
#define FIFO4 "/tmp/f4"
#define FIFO5 "/tmp/f5"
#define PERMS 0666




int pipe1[2],  pipe3[2], pipe4[2], pipe5[2];


void sighandler(int);
void stopHandler(int);


int main(int argc,char *argv[])
{
    int numDePipetas= atoi(argv[1]), tempoDeEspera= atoi(argv[3]);
    srand(time(NULL));
    float prob= atof(argv[2]);
    int pid= fork();
    if(pid==0){
    signal(SIGINT, sighandler);


        pipe1[1] = open(FIFO1,1);
    pipe3[1] = open(FIFO3,1);
    pipe4[1] = open(FIFO4,1);
    pipe5[0] = open(FIFO5,0);
    write(pipe1[1], &numDePipetas, sizeof(int));

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
    while((read(pipe5[0],&token ,sizeof(int)))!= -1){
        

        if(change != token){

            read(pipetasDeLeitura[token % numDePipetas ][0],&pipeta,sizeof(int));//será?
            printf("[p%d] blocked on token (val = %d)\n",pipeta+1,token);
            sleep(tempoDeEspera );
            change= token;
        }
        
    }
    close(pipe1[1]);
    
    close(pipe3[1]);
    close(pipe4[1]);
    close(pipe5[0]);
    for(int i =0 ; i<numDePipetas; i++){
        close(pipetasDeLeitura[i][0]);
    }
    }
    else{
    signal(SIGINT, stopHandler);

            mknod(FIFO1, S_IFIFO | PERMS, 0);
    mknod(FIFO3, S_IFIFO | PERMS, 0);
    mknod(FIFO4, S_IFIFO | PERMS, 0);
    mknod(FIFO5, S_IFIFO | PERMS, 0);
    pipe1[0] = open(FIFO1,0); // 0 leitura
    
    pipe3[0] = open(FIFO3,0);
    pipe4[0] = open(FIFO4,0);
    pipe5[1] = open(FIFO5,1);// 1 escrita

    read(pipe1[0], &numDePipetas,sizeof(int));
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
    int pipetasDeEscrita[numDePipetas ][2];
    for(int i = 0;i<numDePipetas  ;i++){
        
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
    while (1){  
        //printf("%d\n",token);
        
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
        if(i== numDePipetas- 1){
            i=0;
        }
        else{
            i++;
        }
        
        token++;
        ///
    }
    close(pipe1[0]);
    close(pipe3[0]);
    close(pipe4[0]);
    close(pipe5[1]);
    for(int i = 0; i<numDePipetas ; i++){
        close(pipetasDeEscrita[i][1]);
    }

    wait(NULL);
    }
    return 0;
}

void sighandler(int signum) {
   printf("Terminating...\n");
   exit(1);
}

void stopHandler(int signum){
    exit(1);
}