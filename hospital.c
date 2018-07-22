#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#define QUEUE_SIZE 3

pthread_mutex_t lock;
struct Queue* queue;

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    struct Patient** array;
};

struct Patient {
    int id;
    int visits;
    sem_t semaphore;
};
 
// function to create a queue of given capacity. 
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0; 
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = malloc(queue->capacity * sizeof(struct Patient*));
    return queue;
}
 
// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }
 
// Function to add an item to the queue.  
// It changes rear and size
void enqueue(struct Queue* queue, struct Patient *patient)
{
    // not needex i think
    /*if (isFull(queue))
        return;*/
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = patient;
    queue->size = queue->size + 1;
    printf("\t\tPatient %d waiting. Seats occupied = %d\n", patient->id, queue->size);
}
 
// Function to remove an item from queue. 
// It changes front and size
struct Patient* dequeue(struct Queue* queue)
{
    // no needed
    /*if (isEmpty(queue))
        return INT_MIN;*/
    struct Patient* patient = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return patient;
}

void *doctor(void* parameters)
{
    // I'm a doctor I will wait for patients and then will serve the most recently arriving patient
    while(1)
    {
        if (! isEmpty(queue) )
        {
            struct Patient *patient = dequeue(queue);
            //printf("\tBefore patient %d visits %d\n", patient->id, patient->visits);
            patient->visits = patient->visits - 1;
            //printf("\tAfter patient %d visits %d\n", patient->id, patient->visits);
            // Doctor takes some time to treat the patient
            int treatmentTime = 1 + (rand() % 3);
            printf("Doctor treating patient %d for %d seconds. Seats occupied = %d\n", patient->id, treatmentTime, queue->size);
            usleep(treatmentTime*1000000);
            sem_post(&(patient->semaphore));
        }
    }
    return NULL;
}

void *patient(void* parameters)
{
    struct Patient *p = parameters;
    printf("Patient with id [%d] needs [%d] visits\n", p->id, p->visits);

    while (1)
    {
        // lock
        pthread_mutex_lock(&lock);

        sem_init(&(p->semaphore), 0, 1);
        sem_wait(&(p->semaphore));
        if (! isFull(queue)) 
        {
            //printf("Queueing Patient %d\n", p->id);
            enqueue(queue, p);
            pthread_mutex_unlock(&lock);
            
            // Wait for the doctor thread to resume this thread;
            sem_wait(&(p->semaphore));
            //printf("\t\tAfter sem_wait for patient %d\n", p->id);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            sem_post(&(p->semaphore));
            // generate this randomly
            int waitTime = 1 + (rand() %3);
            printf("\t\tPatient %d drinking coffee for %d seconds\n", p->id, waitTime);
            usleep(waitTime*1000000);    
        }
        if((p->visits) < 1)
        {  
            break; 
        }
    }

    printf("Patient %d is done being treated and has completed all doctor visits\n", p->id);
    
    return NULL;
}

int main()
{

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

    // create queue
    queue = createQueue(QUEUE_SIZE);

    time_t t;

    /* initializes a random number generator with the current time as the seed */
    srand((unsigned) time(&t));

    /* sets the number of patients to a number between 10 and 20 (inclusive) */
    //int number_of_patients = 4;
    int number_of_patients = 5 + (rand() % 6); 

    /* creates the array of patients */
    struct Patient patients[number_of_patients];
    
    /* 1 doctor pthread, and the patient pthreads */
    pthread_t tid[1 + number_of_patients]; 

    int create_status = pthread_create(&tid[0], NULL, doctor, NULL);
    if (create_status > 0) {
        printf("Error when creating doctor thread: %d\n", create_status);
        return create_status;
    }

    int i;
    /* create patient threads */
    for (i = 1; i <= number_of_patients ; i++) {
        patients[i].id = i;
        /* assign the patient 1 to 5 visits */
        patients[i].visits = 1 + (rand() % 5); 
        int create_status = pthread_create(&tid[i], NULL, patient, &patients[i]);
        if (create_status > 0) {
            printf("Error when creating patient thread[%d]: %d\n", i, create_status);
            return create_status;
        }
    }

    /* join patient threads */
    for (i = 1; i <= number_of_patients; i++) {
        int join_status = pthread_join(tid[i], NULL);
        if (join_status > 0) {
            printf("Error when joining patient thread[%d]: %d\n", i, join_status);
            return join_status;
        }
    }
    // TODO kill doctor thread once all patient threads have returned
    /* join doctor thread */
    int join_status = pthread_join(tid[0], NULL);
    if (join_status > 0) {
        printf("Error when joining doctor thread: %d\n", join_status);
        return join_status;
    }

    // TODO check from the return status of the threads (tid array)


    pthread_mutex_destroy(&lock);

    return 0;
}
