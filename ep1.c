#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread1;
/* CPU sendo usada = 1 */
int cpu = 0; 

#define MAX_STRING 32
#define EPSILON 0.0000000001

typedef struct sExecucao {
    double t0;
    double dt;
    double deadline;
    char nome[MAX_STRING];
    int rank;
} Execucao;

typedef struct cel {
    Execucao exec;
    struct cel* prox;

} Celula;

typedef Celula* Fila;
typedef Celula* Lista;

/* Funcoes para manipular as listas */
Lista LISTAinicia (void);
Lista LISTAinsere (Lista p, Execucao x);
Lista LISTAinsereRobin (Lista p, Execucao x);
Execucao LISTAtopo (Lista *l);
void LISTAremove (Lista *l, Execucao x);
void LISTAdump (Lista p);


FILE* open_file (char* file);
int read_process (FILE* fp, Execucao* exec);
void* processo (void* sleep);

int add_process (int escalonador, Lista* fila, Execucao exec);
int check_process (int escalonador, Lista* fila, double clock, FILE* fout, int* mudanca_contexto, int debug);

/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    FILE *fp, *fout;
    /* "relogio" do escalonador */
    double clock = 0;
    /* contador de processos */
    int cont_processos = 0;
    int lendo_arquivo = 1;
    int debug = 0;
    /* tipo do Escalonador: 1) SJF; 2) RoundRobin; 3) Prioridade; */
    int escalonador;
    int mudanca_contexto = 0;
    
    Execucao exec;
    Lista lista;

    /* argumentos */
    if (argc  < 4) {
        exit(1);
    }
    else {
        /*seleciona o escalonador*/
        escalonador = atoi(argv[1]);
        /* E/S */
        fp = fopen (argv[2],"r");
        fout = fopen (argv[3], "w");
        if (fout == NULL || fp == NULL) {
            printf ("Erro E/S!\n");
            exit(1);
        }
        if (argc == 5) debug=1;
    }

    /*lista de processos*/
    lista = LISTAinicia ();

    /* le a primeira linha do arquivo trace */
    read_process (fp, &exec);
    
    printf ("Escalonador escolhido: %s\n", argv[1]);
    /* ESCALONADOR */
    while (1) {

        /* Adicionar o processo na fila */
        if ((exec.t0 - clock) < EPSILON && lendo_arquivo == 1) {
            /*DEBUG*/
            /*fprintf (stderr, "\nChegou o %s:\t em t0: %.2f\t com dt: %.2f \n", exec.nome, exec.t0, exec.dt);*/
            add_process (escalonador, &lista, exec);
            cont_processos++;
            /* le o proximo processo */
            if (read_process (fp, &exec) == -1) {
                lendo_arquivo = 0;
            }
        }
        
        if (lista != NULL)
            check_process (escalonador, &lista, clock, fout, &mudanca_contexto, debug);
        else 
            if (lendo_arquivo == 0 && cpu == 0) {
                /*terminou a lista de processos e liberou a cpu*/
                break; 
            }
        
        /* 0.1 segundos */
        usleep (100000);
        clock += 0.1;
        /*fprintf (stderr,"clock: %lf | processo: %s | t0: %.2f\n", clock, exec.nome, exec.t0);*/
    }
    
    fprintf (fout,"%d",mudanca_contexto);
    /*se tudo deu certo a lista estará vazia nesse ponto*/
    LISTAdump (lista);
    fclose (fp);
    fclose (fout);
    return 0;
}

/*---------------------------------------------------------------------------*/

void* processo (void* slep)
{
    double sleep = *((double*) slep);
    pthread_mutex_lock ( &mutex1 );
    cpu = 1;
    pthread_mutex_unlock ( &mutex1 );

    sleep = (int)sleep*1000000;
    usleep (sleep);

    pthread_mutex_lock( &mutex1 );
    cpu = 0;
    pthread_mutex_unlock( &mutex1 );
    return NULL;
}

FILE *open_file (char *file)
{
    FILE *fp;
    fp = fopen (file, "r");
    return fp;
}

int read_process (FILE *fp, Execucao *exec)
{ 
    if (fscanf (fp, "%lf %lf %lf %s", &exec->t0, &exec->dt, &exec->deadline, exec->nome) == EOF)
        return -1;
    return 0;
}

int add_process (int escalonador, Lista* fila, Execucao exec)
{
    switch (escalonador) {
        /* Shortest Job First */
        case 1:
            /*printf ("Insere %s na lista\n",exec.nome);*/
            /* busca e insere de acordo com o menor dt*/
            *fila = LISTAinsere (*fila, exec);
            break;
        /* Round Robin */
        case 2:
            /*FIFO com quantum */
            /* Colocar no fim apenas */
            *fila = LISTAinsereRobin (*fila, exec);
            break;
        /* Fila de prioridades */
        case 3:

            break;
    }
    return 0;
}

int check_process (int escalonador, Lista* fila, double clock, FILE *fout, int* mudanca_contexto, int debug) 
{
    Execucao exec;
    double quantum = 0.1; /*100ms*/

    switch (escalonador) {
        /* Shortest Job First */
        case 1:
            if (cpu == 0) {
                /* remove da fila */
                exec = LISTAtopo(fila);
                /*DEBUG*/
                /*fprintf(stderr,"Imprime Lista\n");
                LISTAdump(*fila);
                */
                if (debug)
                    fprintf (stderr,"%s\t chegou em: %.2f\t dt: %.2f\texecutou em: %.2f\t terminou em: %.2f\t esperou: %.2f\n",exec.nome,exec.t0,exec.dt,clock,(clock+exec.dt),(clock-exec.t0));
                fprintf (fout,"%s\t%.2f\t%.2f\t%.2f\n",exec.nome, (clock+exec.dt), (clock+exec.dt - exec.t0), exec.dt);
                pthread_create( &thread1, NULL, &processo, &exec.dt);
            }
            break;

            /* Round Robin */
        case 2:
            if (cpu == 0) {
                /* remove da fila */
                exec = LISTAtopo(fila);
                exec.dt = exec.dt - quantum;
                /*se nao terminou, volta pra lista*/
                if (exec.dt > 0) {
                    *fila = LISTAinsereRobin (*fila, exec);       
                    *mudanca_contexto = *mudanca_contexto + 1;
                }
                if (debug)
                    fprintf (stderr,"%s\t chegou em: %.2f\t dt: %.2f\texecutou em: %.2f\t terminou em: %.2f\t esperou: %.2f\n",exec.nome,exec.t0,exec.dt,clock,(clock+exec.dt),(clock-exec.t0));
                fprintf (fout,"%s\t%.2f\t%.2f\t%.2f\n",exec.nome, (clock+exec.dt), (clock+exec.dt - exec.t0), exec.dt);
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


/* inicia um ptr para celula (Lista) */
Lista LISTAinicia (void) {
    Lista l = NULL;
    return l;
}

/* insere processo por ordem de dt */
Lista LISTAinsere (Lista p, Execucao x) {
    Lista nova, q, ant;
    double dt = x.dt;
    double dt0;
    q = p;
    ant = NULL;
    nova = malloc (sizeof (Celula));
    nova->exec = x;
    nova->prox = NULL;
    if (q == NULL) 
        return nova;
    else {
        while (q != NULL) {
            dt0 = q->exec.dt;
            if (dt < dt0) 
                break;
            ant = q;
            q = q->prox;
        }
        nova->prox = q;
        if (ant == NULL) 
            p = nova;
        else 
            ant->prox = nova;
        return p;  
    }
}

/* insere processo por ordem de chegada */
Lista LISTAinsereRobin (Lista p, Execucao x) {
    Lista nova, q;
    q = p;
    nova = malloc (sizeof (Celula));
    nova->exec = x;
    nova->prox = NULL;
    if (q == NULL) 
        return nova;
    else {
        while (q->prox != NULL)
            q = q->prox;
        q->prox = nova;
        return p;  
    }
}

/* remove execucao da lista */
void LISTAremove (Lista *l, Execucao x)
{
    Lista ant, atual, aux;
    ant = NULL;
    atual = *l;

    while (atual != NULL && strcmp(atual->exec.nome,x.nome) != 0 ) {
        ant = atual;
        atual = atual->prox;
    }
    /*nao encontrou x*/
    if (atual == NULL) 
        printf ("nao encontrou nada\n");
    else {
        if (ant == NULL) {
            aux = atual->prox;
            *l = aux;
        }
        else ant->prox = atual->prox;
        free(atual);
    }
}

/* devolve a Execucao do topo da lista e a remove da lista */
Execucao LISTAtopo (Lista *l)
{
    Execucao topo;
    Lista atual;
    atual = *l;
    topo = atual->exec;
    *l = atual->prox;
    free (atual);
    return topo;
}


/* imprime lista de processos */
void LISTAdump (Lista p) {
     while (p != NULL) {
           printf ("%s: dt = %lf\n", p->exec.nome, p->exec.dt);
           p = p->prox;
     }
     printf ("\n");
}
