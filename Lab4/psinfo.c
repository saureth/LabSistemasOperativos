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

#define MAX_BUFFER 100
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

void load_info(int pid);
void print_info(proc_info* pi);
proc_info retorna_estructura(char* pid);
  
proc_info* all_proc;
int iterador = 0;

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
  all_proc = (proc_info *)malloc(sizeof(proc_info)*n_procs);
  assert(all_proc!=NULL);
  
  // Get information from status file
  for(i = 0; i < n_procs; i++){
    int pid = atoi(argv[i+1]);
    load_info(pid);
  }
  
  //print information from all_proc buffer
  for(i = 0; i < n_procs; i++){
    print_info(&all_proc[i]);
  }

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
void load_info(int pid){
  FILE *fpstatus;
  char buffer[MAX_BUFFER]; 
  char path[MAX_BUFFER];
  char* token;
  

  sprintf(path, "/proc/%d/status", pid);
  fpstatus = fopen(path, "r");
  assert(fpstatus != NULL);
  proc_info* proceso = &all_proc[iterador];
  proceso->pid = pid;
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

proc_info retorna_estructura(char* pid){
    proc_info* infoProceso;
    FILE *fpstatus;
    char path1[MAX_BUFFER];
    sprintf(path1, "/proc/%d/status", pid);
    fpstatus = fopen(path1, "r");
    assert(fpstatus != NULL);
    infoProceso-> pid = pid;

}