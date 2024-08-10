//import libraries

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "Semaphore.h"

//Semaphores for buffer access - and how to use nullptrs: https://www.geeksforgeeks.org/understanding-nullptr-c/


//Empty spots
Semaphore *empty = nullptr;

//Full spots
Semaphore *full = nullptr;
Semaphore *mutex = nullptr;

//
int *buffer = nullptr;

//The initialized size of the buffer - later takess user inputs from command line
int buffer_size = 0;


int in = 0;
int out = 0;

//Variable to track the number of Baby Yodas produced
int serialnum = 0;

//Variable to track number of Baby Yodas consumed (also allows for tracking consumed vs max items for logical operation purposes)
int consumed = 0;

//Producer routine for the main function to call - template provided in class project
void *producer_routine(void *data) {
    
    //generate a random unsigned integer and assign it - provided in class, appears to run fine without as the other sleep functions generate their own time
    //time_t rand_seed;
    //srand((unsigned int)time(&rand_seed));

    //sets a variable for how many items are left to produce based on command line input on
    int left_to_produce = *((int *)data);


    //While there are still items left to produce...
    while (left_to_produce > 0) {
        printf("Producer wants to put Baby Yoda #%d into buffer...\n", serialnum + 1);

        //Wait for a spot in the buffer
        empty->wait();

        //lock mutex
        mutex->wait();

        //place in buffer at serial number incrememnt, 

        //https://stackoverflow.com/questions/47468473/producer-consumer-algorithm-to-use-full-buffer for format
        buffer[in] = ++serialnum;
        in = (in + 1) % buffer_size;

        //Once placed/produced decrease how many are left to produce
        left_to_produce--;

        //unlocks mutex
        mutex->signal();

        printf("   Shopkeeper put Baby Yoda #%d on the shelf.\n", serialnum);

        //Signal that a spot in the is available
        full->signal();

        //random sleep - from project
        usleep((useconds_t)(rand() % 200000));
    }
    return NULL;
}

//Consumer routine for the main function to call - template provided in class project
void *consumer_routine(void *data) {
    int max_items = *((int*)data);

    //While there are still items to consume....
    while (consumed < max_items) {


        //pthread_self() function from https://www.tutorialspoint.com/pthread-self-in-c - used a various points for troubleshooting.  Kepy for style.
        printf("Consumer #%ld wants to buy a Baby Yoda...\n", pthread_self());

        //wait for something in the buffer
        full->wait();

        //lock the mutex to allow a consumer to pull an available item out of the buffer
        mutex->wait();

        //if consumed items are equal to or greater than..... (intuitively if equals to would suffice, but running without greater than to meet ALL conditions produces bad behavior)
        if (consumed >= max_items) {

            //unlock the mutex and enter critical section
            mutex->signal();

            //signal that there is now a spot in the buffer avaialble
            full->signal();

            printf("   Consumer #%ld bought Baby Yoda #%d.\n", pthread_self(), serialnum);

            printf("Consumer #%ld exclaims in excitement \"I bought Baby Yoda #%d!!!\"\n", pthread_self(), serialnum);
            
            //Use for debugging - helpful to run and see output - author determined consumer threads were waiting (via join) and not exiting with this methodology
            //printf("Consumer is in the if the loop. Consumed: %d, Max Items: %d\n", consumed, max_items);

            //then break the loop 
            break;
        }

        int item = buffer[out];
        out = (out + 1) % buffer_size;
        printf("   Consumer exclaims #%ld bought Baby Yoda #%d.\n", pthread_self(), item);

        printf("Consumer #%ld exclaims in excitement \"I bought Baby Yoda #%d!!!\"\n", pthread_self(), item);

        consumed++;
        mutex->signal();
        empty->signal();

        // Simulate consumption time
        usleep((useconds_t)(rand() % 1000000));

		
        //Another print statement for debugging
        //printf("Consumer is in the while loop. Consumed: %d, Max Items: %d\n", consumed, max_items);
        
    }

    printf("Consumer #%ld went home because there were no Baby Yodas left.\n", pthread_self());
    return NULL;
}

//main function - template provided in class; better explanation of strtol function here: //https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm
int main(int argc, const char *argv[]) {
    if (argc < 4) {

        //if incorrectly formatted, print this error:
        printf("Invalid parameters. Format: %s <buffer_size> <num_consumers> <max_items>\n", argv[0]);
        exit(0);
    }

   
    //Set buffer size, number of consumers, and max items (number of items to produce) to values entered in command line
    buffer_size = strtol(argv[1], NULL, 10);
    int num_consumers = strtol(argv[2], NULL, 10);
    int max_items = strtol(argv[3], NULL, 10);

    //Initialize the buffer
    buffer = new int[buffer_size];

    //Buffer is empty at start
    empty = new Semaphore(buffer_size);

    //So no spots in the buffer are "full"
    full = new Semaphore(0);

    //initialize the mutex semaphore
    mutex = new Semaphore(1);

    //pthread_t provided in class template, additional reading at: https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
    pthread_t producer;
    pthread_t *consumers = new pthread_t[num_consumers];

    //create a producer thread
    pthread_create(&producer, NULL, producer_routine, (void *)&max_items);

    //create consumer threads until number of consumer threads entered in command line are created
    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer_routine, (void *)&max_items);
    }

    //allow producer thread to terminate
    pthread_join(producer, NULL);
    printf("The manufacturer has completed his work for the day.\n");
    printf("Waiting for consumers to buy up the rest of the Baby Yodas.\n");

    //wait for items to be consumed
    while (consumed < max_items)
        sleep(1);

    //template provided in class materials intially used "pthreads_join", which appeared to have the affect of waiting for created consumer threads to consume items that would not be produced, preventing the program from exiting.  
    //Therefore pthreads_cancel was chosen as an interim solution due to the author being unable to achieve a satisfactory way to gracefully terminate various consumer threads by the due date of the project.
    for (int i = 0; i < num_consumers; i++) {
        pthread_cancel(consumers[i]);
    }

    //Delete operators to destroy items - good housekeeping practice
    //https://www.geeksforgeeks.org/delete-in-c/#

    delete[] buffer;
    delete empty;
    delete full;
    delete mutex;
    delete[] consumers;

    //Print to tell the user the program is complete
    printf("Producer/Consumer simulation program is complete!\n");

    return 0;
}
