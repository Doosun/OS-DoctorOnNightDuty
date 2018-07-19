#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 3

static int queue[BUFFER_SIZE];

struct patient {
    int id;
    int visits;
};

void *doctor(void* parameters)
{
    // I'm a doctor I will wait for patients and then will serve the most recently arriving patient

    return NULL;
}

void *patient(void* parameters)
{
    struct patient *p = parameters;
    printf("Patient with id [%d] needs [%d] visits\n", p->id, p->visits);
    /*
    while (needs more visits) {
        if (space in queue) {
            sit in queue
        } else {
            wait in lobby and have coffee
        }
    }
    */
    return NULL;
}

int main()
{
    time_t t;

    /* initializes a random number generator with the current time as the seed */
    srand((unsigned) time(&t));

    /* sets the number of patients to a number between 10 and 20 (inclusive) */
    int number_of_patients = 10 + (rand() % 11); 

    /* creates the array of patients */
    struct patient patients[number_of_patients];
    
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

    /* join doctor thread */
    int join_status = pthread_join(tid[0], NULL);
    if (join_status > 0) {
        printf("Error when joining doctor thread: %d\n", join_status);
        return join_status;
    }

    // TODO check from the return status of the threads (tid array)

    return 0;
}
