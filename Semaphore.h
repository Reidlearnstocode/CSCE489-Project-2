//if the following is not defined
//template frrom class
//https://learn.microsoft.com/en-us/cpp/preprocessor/hash-if-hash-elif-hash-else-and-hash-endif-directives-c-cpp?view=msvc-170
//https://www.w3schools.com/cpp/cpp_access_specifiers.asp#:~:text=public%20%2D%20members%20are%20accessible%20from,viewed)%20from%20outside%20the%20class
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>

class Semaphore 
{
public:

	Semaphore(int count);
	~Semaphore();

	void wait();
	void signal();

private:

//private members - cannot be accessed or viewed outside the class
//semaphore value
int value;

//the mutex lock
pthread_mutex_t mutex;

//the condition variable
pthread_cond_t cond;

};

//ends the ifndef directive
#endif