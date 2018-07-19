#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 3

static int queue[BUFFER_SIZE];

void *doctor(void* parameters)
{
    // I'm a doctor I will wait for patients and then will serve the most recently arriving patient

    return NULL;
}

void *patient(void* parameters)
{
    // I'm a patient I will sit down and wait if there is space available (queue), otherwise
    // I will go to the lobby and drink coffee and come back and check if there is space available in the queue
    // If there is space available in the queue I will sit down in the queue

    return NULL;
}

int main()
{
    
    pthread_t tid[2]; 

    pthread_create(&tid[0], NULL, doctor, NULL);

    // TODO create a random number of patient arrays
    // TODO create a structure with the patient id in it, so that the patient can advertise what it is doing
        // pass this in and it will be the parameters in the patient thread
    pthread_create(&tid[1], NULL, patient, NULL);

    // TODO check from the return status of the threads (tid array)

    return 0;
}
