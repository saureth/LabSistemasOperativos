/**
 *   \file psinfo-l.c
 *   \brief base code for the program psinfo-l
 *          
 *  This program prints some basic information for a given 
 *  list of processes.
 *  You can use this code as a basis for implementing parallelization 
 *  through the pthreads library. 
 *
 *   \author: Danny Munera - Sistemas Operativos UdeA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_BUFFER 100
#define BUFFER_SIZE 2
//#define DEBUG
typedef struct p_ {
  int pid;
  char name[MAX_BUFFER];
  char state[MAX_BUFFER];
  char vmsize[MAX_BUFFER];
  char vmdata[MAX_BUFFER];
  char vmexe[MAX_BUFFER];
  char vmstk[MAX_BUFFER];
  int voluntary_ctxt_switches;
  int nonvoluntary_ctxt_switches;
} proc_info;

struct retornarPID{
  int miPid;
};

void* load_info(void* pid);
void print_info(proc_info* pi);
  
proc_info* all_proc;
int iterador = 0;
int iterador2 = 0;
sem_t semaforo;
sem_t semaforoP;
sem_t semaforoC;

int main(int argc, char *argv[]){
  int i;
  // number of process ids passed as command line parameters
  // (first parameter is the program name) 
  int n_procs = argc - 1;
  
  if(argc < 2){
    printf("Error\n");
    exit(1);
  }
  /*Allocate memory for each process info*/
  all_proc = (proc_info *)malloc(sizeof(proc_info)*BUFFER_SIZE);
  assert(all_proc!=NULL);
    

  pthread_t id_Hilos[n_procs];
  struct retornarPID misPID[n_procs];
  sem_init(&semaforo,0,1);
  sem_init(&semaforoP,0,n_procs);
  sem_init(&semaforoC,0,0);
  // Get information from status file
  for(i = 0; i < n_procs; i++){
    int pid = atoi(argv[i+1]);
    misPID[i].miPid = pid;
    pthread_create(&id_Hilos[i],NULL,&load_info,&misPID[i]);
    //pthread_join(id_Hilos[i],NULL);
    //load_info(pid);
  }
  
  //print information from all_proc buffer
  for(i = 0; i < n_procs; i++){
    sem_wait(&semaforoC);
    print_info(&all_proc[iterador2%BUFFER_SIZE]);
    iterador2++;
    sem_post(&semaforoP);
  }

  sem_destroy(&semaforo);
  // free heap memory
  free(all_proc);
  
  return 0;
}

/**
 *  \brief load_info
 *
 *  Load process information from status file in proc fs
 *
 *  \param pid    (in)  process id 
 *  \param myinfo (out) process info struct to be filled
 */
void* load_info(void* pid){
  sem_wait(&semaforoP);
  sem_wait(&semaforo);
  FILE *fpstatus;
  char buffer[MAX_BUFFER]; 
  char path[MAX_BUFFER];
  char* token;
  struct retornarPID* auxPID = (struct retornarPID*) pid;
  int realPID = auxPID->miPid;
  sprintf(path, "/proc/%d/status", realPID);
  fpstatus = fopen(path, "r");
  assert(fpstatus != NULL);

  proc_info* proceso = &all_proc[iterador%BUFFER_SIZE];

  proceso->pid = realPID;
  while (fgets(buffer, MAX_BUFFER, fpstatus)) {
    token = strtok(buffer, ":\t");
    if (strstr(token, "Name")){
      token = strtok(NULL, ":\t");
      strcpy(proceso->name, token);
    }else if (strstr(token, "State")){
      token = strtok(NULL, ":\t");
      strcpy(proceso->state, token);
    }else if (strstr(token, "VmSize")){
      token = strtok(NULL, ":\t");
      strcpy(proceso->vmsize, token);
    }else if (strstr(token, "VmData")){
      token = strtok(NULL, ":\t");
      strcpy(proceso->vmdata, token);
    }else if (strstr(token, "VmStk")){
      token = strtok(NULL, ":\t");
      strcpy(proceso->vmstk, token);
    }else if (strstr(token, "VmExe")){
      token = strtok(NULL, ":\t");
      strcpy(proceso->vmexe, token);
    }else if (strstr(token, "nonvoluntary_ctxt_switches")){
      token = strtok(NULL, ":\t");
      proceso->nonvoluntary_ctxt_switches = atoi(token);
    }else if (strstr(token, "voluntary_ctxt_switches")){
      token = strtok(NULL, ":\t");
      proceso->voluntary_ctxt_switches = atoi(token);
    }
  }
  fclose(fpstatus);
  iterador++;
  sem_post(&semaforo);
  sem_post(&semaforoC);
}
/**
 *  \brief print_info
 *
 *  Print process information to stdout stream
 *
 *  \param pi (in) process info struct
 */ 
void print_info(proc_info* pi){
  printf("PID: %d \n", pi->pid);
  printf("Nombre del proceso: %s", pi->name);
  printf("Estado: %s", pi->state);
  printf("TamaÃ±o total de la imagen de memoria: %s", pi->vmsize);
  printf("TamaÃ±o de la memoria en la regiÃ³n TEXT: %s", pi->vmexe);
  printf("TamaÃ±o de la memoria en la regiÃ³n DATA: %s", pi->vmdata);
  printf("TamaÃ±o de la memoria en la regiÃ³n STACK: %s", pi->vmstk);
  printf("NÃºmero de cambios de contexto realizados (voluntarios"
	 "- no voluntarios): %d  -  %d\n\n", pi->voluntary_ctxt_switches,  pi->nonvoluntary_ctxt_switches);
}