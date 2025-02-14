#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>


static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t stock_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t stock_empty = PTHREAD_COND_INITIALIZER;


static int stock = 0;


#define MAX_STOCK 20
#define TIME_PROD 5
#define TIME_CONS 7
#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define RED "\033[31m"


static void *produce_thread(void *arg)
{
    int id = *(int *)arg;
    free(arg);
    int delivery = (rand() % 20) + 1;
    printf(GREEN "\nCREATED Producer %d: Starting with %d item(s) to deliver.\n" RESET, id, delivery);


    while (delivery > 0)
    {
        pthread_mutex_lock(&mtx);
        while (stock >= MAX_STOCK)
        {
            printf(RED "Producer %d: Waiting, stock full at %d.\n" RESET, id, stock);
            pthread_cond_wait(&stock_full, &mtx);
        }
        while (stock < MAX_STOCK && delivery > 0)
        {
            ++stock;
            --delivery;
            printf(YELLOW "Producer %d: Added item (stock: %d, items left: %d)\n" RESET, id, stock, delivery);
        }
        printf(YELLOW "Producer %d: Finished delivery, current stock: %d.\n" RESET, id, stock);
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&stock_empty);
    }
    printf(GREEN "EXITING Producer %d.\n" RESET, id);
    return NULL;
}


static void *consume_thread(void *arg)
{
    int id = *(int *)arg;
    free(arg);
    int items = 0;
    int request = (rand() % 5) + 1;
    printf(GREEN "\nCREATED Consumer %d: Requesting %d item(s).\n" RESET, id, request);


    while (request > 0)
    {
        pthread_mutex_lock(&mtx);
        while (stock == 0)
        {
            printf(RED "Consumer %d: Waiting, stock empty.\n" RESET, id);
            pthread_cond_wait(&stock_empty, &mtx);
        }
        while (stock > 0 && request > 0)
        {
            ++items;
            --request;
            --stock;
            printf(CYAN "Consumer %d: Consumed item (stock: %d, items consumed: %d)\n" RESET, id, stock, items);
        }
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&stock_full);
    }
    printf(GREEN "EXITING Consumer %d\n" RESET, id);
    return NULL;
}


int main()
{
    srand(time(NULL));
    int counter = 0;
    int numProducers = 0;
    int numConsumers = 0;


    while (counter < 100)
    {
        if (counter % TIME_PROD == 0)
        {
            int *prod_id = malloc(sizeof(int));
            *prod_id = ++numProducers;
            pthread_t producer_thread;
            pthread_create(&producer_thread, NULL, produce_thread, prod_id);
            pthread_detach(producer_thread);
        }


        if (counter % TIME_CONS == 0)
        {
            int *cons_id = malloc(sizeof(int));
            *cons_id = ++numConsumers;
            pthread_t consumer_thread;
            pthread_create(&consumer_thread, NULL, consume_thread, cons_id);
            pthread_detach(consumer_thread);
        }


        sleep(1);
        ++counter;
    }


    return 0;
}
