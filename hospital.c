#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 20

static int queue[BUFFER_SIZE];

void *doctor(void* parameters)
{
    // I'm a doctor I will wait for patients and then will serve the most recently arriving patient

    return NULL;
}

void *patient(void* parameters)
{
    // I'm a patient I will wait in the lobby until the doctor is read to server me

    return NULL;
}

int main()
{
    
    pthread_t tid[2]; 

    pthread_create(&tid[0], NULL, doctor, NULL); 
    pthread_create(&tid[1], NULL, patient, NULL);

    return 0;
}
