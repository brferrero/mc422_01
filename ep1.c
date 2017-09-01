#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread1;
int cpu; /* CPU sendo usada = 1 */

#define MAX_STRING 1024
#define EPSLON 0.0000000001

typedef struct sExecucao {
  double d0;
  double dt;
  double deadline;
  char nome[MAX_STRING];
} Execucao;

typedef struct cel {
  Execucao exec;
  struct cel *prox;

} Celula;

typedef Celula* Fila;

FILE* openfile(char* file);
int read_process(FILE* fp, Execucao* exec);
void* processo(void* sleep);
int add_process(int escalonador, Fila fila, Execucao exec);
int check_process(int escalonador, Fila fila);

void insere (Execucao exec, Celula *p) {
  Celula *nova;
  nova = malloc (sizeof (Celula));
  nova->exec = exec;
  nova->prox = p->prox;
  p->prox = nova;
}

void busca_e_insere (Execucao exec, double dt, Celula *le)
{
 Celula *p, *q, *nova;
 nova = malloc (sizeof (Celula));
 nova->exec = exec;
 p = le;
 q = le->prox;
 while (q != NULL && q->exec.dt <= dt ) {
    p = q;
    q = q->prox;
 }
 nova->prox = q;
 p->prox = nova;
}


void remove_celula (Celula *p)
{
 Celula *lixo;
 lixo = p->prox;
 p->prox = lixo->prox;
 free (lixo);
}

int main(int argc, char* argv[])
{
  
  FILE* fp;
  double clock = 0;
  Execucao exec;
  Fila fila;
  int escalonador = atoi(argv[1]);

  double* dt = malloc(sizeof(*dt));
  *dt = 1000000.0;
  
  /* ESCALONADOR */
  fp = openfile(argv[2]);
  read_process(fp, &exec);
  printf("Escalonador: %s\n", argv[1]);
  while(1) {

    /* Adicionar o processo na fila */
    if ((exec.d0 - clock) < EPSLON ) {
      printf("entrou!\n\n\n");
      /* Adiciona exec na fila de processos */
      add_process(escalonador, fila, exec);
      if (read_process(fp, &exec) == -1) /* pega o proximo da lista trace.txt */
        break;
    }
    
    check_process(escalonador,fila);

    usleep(100000);
    clock += 0.1;
    printf("clock: %lf | processo: %s | d0: %lf\n", clock, exec.nome, exec.d0);
  }

  fclose(fp);
  return 0;
}

void* processo(void* slep)
{

  double sleep = *((double*) slep);
  pthread_mutex_lock( &mutex1 );
  cpu = 1;
  pthread_mutex_unlock( &mutex1 );

  sleep = (int)sleep*1000000;
  usleep(sleep);

  pthread_mutex_lock( &mutex1 );
  cpu = 0;
  pthread_mutex_unlock( &mutex1 );
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



int add_process(int escalonador, Fila fila, Execucao exec)
{
  
  switch(escalonador) {
    /* Shortest Job First */
    case 1:
      /* busca e insere de acordo com o menor dt*/
      break;
    
    /* Round Robin */
    case 2:
      /* Colocar no fim apenas */

      break;
    
    /* Fila de prioridades */
    case 3:

      break;

  }

  return 0;
}

int check_process(int escalonador, Fila fila)
{
  Execucao exec; /* tirar da fila */
  double quantum = 0.1; /*100ms*/
  switch(escalonador) {
    /* Shortest Job First */
    case 1:
      if (cpu == 0)
        /* remove da fila */
        pthread_create( &thread1, NULL, &processo, &exec.dt);
      break;
    
    /* Round Robin */
    case 2:
      if (cpu == 0) {
        /* remove da fila */
        pthread_create(&thread1, NULL, &processo, &quantum);

        /* coloca de volta na fila de processos com exec.dt -= quantum */
        /* add_process(2, fila, exec_alterado) */
      }
      break;
    
    /* Fila de prioridades */
    case 3:
      /* Mesmo caso do Round Robin mas a lista deve ser uma MinPQ */
      /* remove da fila */
      pthread_create(&thread1, NULL, &processo, &quantum);

      /* coloca de volta na fila de processos com exec.dt -= quantum */
      /* add_process(3, fila, exec_alterado) */
      break;

  }

  return 0;
}