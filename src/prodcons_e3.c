#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

#define BUFF_SIZE   5      /* total number of slots */
#define NP          3      /* total number of producers */
#define NC          3      /* total number of consumers */
#define NITERS      4      /* number of items produced/consumed */

typedef struct {
    int buf[BUFF_SIZE];   /* shared var */
    int in;               /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;              /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full;           /* keep track of the number of full spots */
    sem_t empty;          /* keep track of the number of empty spots */
    sem_t mutex;          /* enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;

void *Producer(void *arg)
{
    int i, item, index;
    index = (int)(intptr_t)arg; /* Casting corregido para evitar warnings */

    for (i=0; i < NITERS; i++) {
        item = i;   
        /* Si no hay espacios vacios, esperar */
        sem_wait(&shared.empty);
        /* Si otro hilo usa el buffer, esperar (exclusion mutua) */
        sem_wait(&shared.mutex);
        
        shared.buf[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        printf("[P%d] Producing %d ...\n", index, item); 
        fflush(stdout);
        
        /* Liberar el buffer */
        sem_post(&shared.mutex);
        /* Incrementar contador de espacios llenos */
        sem_post(&shared.full);

        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

void *Consumer(void *arg)
{
    int i, item, index;
    index = (int)(intptr_t)arg;
    
    /* El numero total a consumir se divide entre los consumidores */
    for (i=0; i < (NP * NITERS)/NC; i++) {
        /* Si no hay elementos llenos, esperar */
        sem_wait(&shared.full);
        /* Bloquear acceso a los datos compartidos */
        sem_wait(&shared.mutex);
        
        item = shared.buf[shared.out];
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("------> [C%d] consumed %d\n", index, item); 
        fflush(stdout);
        
        /* Liberar el buffer */
        sem_post(&shared.mutex);
        /* Indicar que ahora hay un nuevo espacio vacio */
        sem_post(&shared.empty);

        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t idP[NP], idC[NC];
    int index;

    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    
    /* Inicializar el candado mutex a 1 (disponible) */
    sem_init(&shared.mutex, 0, 1);

    for (index = 0; index < NP; index++)
    {  
       /* Crear a los productores */
       pthread_create(&idP[index], NULL, Producer, (void*)(intptr_t)index);
    }

    /* Crear NC consumidores */
    for (index = 0; index < NC; index++)
    {
       pthread_create(&idC[index], NULL, Consumer, (void*)(intptr_t)index);
    }

    /* Esperar finalizacion o salir */
    pthread_exit(NULL);
}