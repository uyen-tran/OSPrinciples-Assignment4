#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

sem_t writing;
sem_t mutex;
int nReader = 0;
sem_t thrd_ord[10];

void* reader(void* param) 
{
    int id = *(int*) param;

    sem_wait(&thrd_ord[id]);                    // wait until it is turn for this reader.
    if (id < 9) sem_post(&thrd_ord[id+1]);      // the next thread is now allowed to execute.

    sem_wait(&mutex);                           // acquire lock to access and make change on the global variables.
    nReader++;
    if (nReader == 1) {
        sem_wait(&writing);                     // if this is the first reader back in the file, wait until the previous writer is done.
    }
    sem_post(&mutex);                           // release lock after edit.

    //simulate reading process.
    printf("Reader %d starts reading.\n", id+1);
    usleep(1000);
    printf("Reader %d ends reading.\n", id+1);

    sem_wait(&mutex);
    nReader--;
    if (nReader == 0) {
        sem_post(&writing);                     // if there is no more reader, a writing lock is available.
    }
    sem_post(&mutex);
}

void* writer(void* param) 
{
    int id = *(int*) param;

    sem_wait(&thrd_ord[id]);                    // wait until it is turn for this reader.

    sem_wait(&writing);                         // wait until the writing lock is released.

    // simulate the writing process.
    printf("Writer %d starts writing.\n", id+1);
    usleep(1000);
    printf("Writer %d ends writing.\n", id+1);

    sem_post(&writing);                         // release the writing lock.

    if (id < 9) sem_post(&thrd_ord[id+1]);      // the next thread is now allowed to execute.
}

int main(int argc, char const* argv[])
{
    // semaphores to control thread order.
    for (int i = 0; i < 11; i++) {
        sem_init(&thrd_ord[i],0,0);
    }
    sem_post(&thrd_ord[0]);     /* only the first thread is unlock to execute first. 
                                   the first one will unlock the second when starting executing. */

    sem_init(&writing,0,1);
    sem_init(&mutex,0,1);

    pthread_t threads[10];
    int id[10];
    
    for (int i = 0; i < 10; i++) {
        id[i] = i;
        int user = atoi(argv[i+1]);
        if (user == 0) {
            pthread_create(&threads[i],NULL,reader,&id[i]);
        }
        else {
            pthread_create(&threads[i],NULL,writer,&id[i]);
        }
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i],NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&writing);
    sem_destroy(thrd_ord);

    return 0;
}