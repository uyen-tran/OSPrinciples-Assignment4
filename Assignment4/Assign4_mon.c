#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

int nReader, nWriter;
pthread_cond_t canRead, canWrite;
pthread_mutex_t mutex;
sem_t thrd_ord[10];

void startRead(int id)
{
    pthread_mutex_lock(&mutex);     // acquire lock to access and make change on the global variables.

    // check to see there is any writer writing in the file.
    if (nWriter > 0) {
        pthread_cond_wait(&canRead,&mutex);
    }

    nReader++;
    
    pthread_mutex_unlock(&mutex);   // release lock after edit.
    pthread_cond_signal(&canRead);  // wake up other reader threads
}

void endRead(int id)
{
    pthread_mutex_lock(&mutex);

    nReader--;
    // if there is no more reader in the file, we can turn on the signal for canWrite.
    if (nReader == 0) {
        pthread_cond_signal(&canWrite);
    }

    pthread_mutex_unlock(&mutex);
}

void startWrite(int id)
{
    pthread_mutex_lock(&mutex);

    // check if there is any reader in the file or any writer in the file.
    if (nReader > 0 || nWriter > 0) {
        pthread_cond_wait(&canWrite,&mutex);
    }

    nWriter++;

    pthread_mutex_unlock(&mutex);
}

void endWrite(int id)
{
    pthread_mutex_lock(&mutex);     

    nWriter--;

    // signal for the next thread to read or write.
    pthread_cond_signal(&canRead);
    pthread_cond_signal(&canWrite);

    pthread_mutex_unlock(&mutex);  
}

void* reader(void* param)
{
    int id = *(int*) param;

    sem_wait(&thrd_ord[id]);                // wait until it is turn for this reader.
    if (id < 9) sem_post(&thrd_ord[id+1]);  // the next thread is now allowed to execute.

    startRead(id);
    //simulate the reading process
    printf("Reader %d starts reading.\n", id+1);
    usleep(1000);
    printf("Reader %d ends reading.\n", id+1);

    endRead(id);
}

void* writer(void* param)
{
    int id = *(int*) param;

    sem_wait(&thrd_ord[id]);                //wait until it is turn for this writer.

    startWrite(id);

    // simulate the writing process.
    printf("Writer %d starts writing.\n", id+1);    
    usleep(1000);
    printf("Writer %d ends writing.\n", id+1);

    endWrite(id);

    if (id < 9) sem_post(&thrd_ord[id+1]);  // the next thread is now allowed to execute.
}

void initialize()
{
    nReader = 0;
    nWriter = 0;

    pthread_cond_init(&canRead,NULL);
    pthread_cond_init(&canWrite,NULL);
    pthread_mutex_init(&mutex,NULL);
}

int main(int argc, char const* argv[])
{
    initialize();

    // semaphores to control thread order.
    for (int i = 0; i < 11; i++) {
        sem_init(&thrd_ord[i],0,0);
    }
    sem_post(&thrd_ord[0]);     /* only the first thread is unlock to execute first. 
                                   the first one will unlock the second when starting executing. */

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

    sem_destroy(thrd_ord);

    return 0;
}