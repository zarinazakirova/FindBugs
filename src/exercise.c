#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE];
int itemCount = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t full;
pthread_cond_t empty;

void produceItem(int);
int consumeItem();
void *producerFunc(void*);
void *consumerFunc(void*);

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producerArgs[NUM_PRODUCERS];
    int consumerArgs[NUM_CONSUMERS];
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);
    
    // Create producers
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producerArgs[i] = i;
        pthread_create(&producers[i], NULL, producerFunc, &producerArgs[i]);
    }
    
    // Create consumers
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumerArgs[i] = i;
        pthread_create(&consumers[i], NULL, consumerFunc, &consumerArgs[i]);
    }
    
    // Wait for producers to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    
    // Wait for consumers to finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);
    
    printf("Main thread exiting.\n");
    
    return 0;
}

void produceItem(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    itemCount++;
}

int consumeItem() {
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    itemCount--;
    return item;
}

void *producerFunc(void *arg) {
    int threadNum = *(int*)arg;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&mutex);
        
        while (itemCount == BUFFER_SIZE) {
            pthread_cond_wait(&full, &mutex);
        }
        int position = in;
        produceItem(i);
        printf("Producer %d produced item %d at the position %d in the buffer.\n", threadNum, i, position);
        
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    
    printf("Producer %d is exiting.\n", threadNum);
    pthread_exit(NULL);
}

void *consumerFunc(void *arg) {
    int threadNum = *(int*)arg;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&mutex);
        
        while (itemCount == 0) {
            pthread_cond_wait(&empty, &mutex);
        }
        int position = out;
        int item = consumeItem();
        printf("Consumer %d consumed item %d at the position %d in the buffer.\n", threadNum, item, position);
        
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }
    
    printf("Consumer %d is exiting.\n", threadNum);
    pthread_exit(NULL);
}
