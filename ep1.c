#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

#define MAX_STRING 1024
#define NUM_THREADS 100
#define EPSLON 0.0000000001

typedef struct sExecucao {
  double d0;
  double dt;
  double deadline;
  char nome[MAX_STRING];
} Execucao;

FILE* openfile(char* file);
int read_process(FILE* fp, Execucao* exec);
void *processo();

int main(int argc, char* argv[])
{
/* pthread_join( thread1, NULL); */
  pthread_t threads[NUM_THREADS];
  pid_t childpid;
  FILE* fp;
  double clock = 0;
  Execucao exec;
  
  /* ESCALONADOR */
  fp = openfile(argv[2]);
  read_process(fp, &exec);
  printf("Escalonador: %s\n", argv[1]);
  while(1) {


    if ((exec.d0 - clock) < EPSLON ) {
      printf("entrou!\n\n\n");
      /* Adiciona exec na fila de processos */
      /* DE ACORDO COM O ALGORITMO DO ESCALONADOR */
      if (read_process(fp, &exec) == -1) /* pega o proximo da lista trace.txt */
        break;
    }
    
    /* se tiver processo na fila */
    /* pegar primeiro da fila */
    /* dispara processo */
    /*pthread_create( &thread1, NULL, &processo, quantum);*/

    /* CASO ROUND ROBIN */
    /*colocar na fila com dt-quantum*/

    /* CASO Prioridade */
    /* usar uma MinPQ */


    usleep(100000);
    clock += 0.1;
    printf("clock: %lf | processo: %s | d0: %lf\n", clock, exec.nome, exec.d0);
      
    
  }

  fclose(fp);
  return 0;
}

// Write numbers 1-3 and 8-10 as permitted by functionCount2()

/*void *functionCount1()
{
   for(;;)
   {
      // Lock mutex and then wait for signal to relase mutex
      pthread_mutex_lock( &count_mutex );

      // Wait while functionCount2() operates on count
      // mutex unlocked if condition varialbe in functionCount2() signaled.
      pthread_cond_wait( &condition_var, &count_mutex );
      count++;
      printf("Counter value functionCount1: %d\n",count);

      pthread_mutex_unlock( &count_mutex );

      if(count >= COUNT_DONE) return(NULL);
    }
}*/

// Write numbers 4-7

void* processo(int sleep)
{
    usleep(sleep);
}

FILE* openfile(char* file)
{
  FILE *fp;
  fp = fopen(file, "r");
  return fp;
}

int read_process(FILE* fp, Execucao* exec)
{ 
  if (fscanf(fp, "%lf %lf %lf %s", &exec->d0, &exec->dt, &exec->deadline, exec->nome) == EOF)
    return -1;
  return 0;
}
