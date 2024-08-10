# :wave: C / C ++ CSCE 489 Operating Systems Project 2: Threads/Semaphores

# This project demonstrates the use of simple synchronization techniques (mutexes and semaphores) for managing a multithreaded process.

A "store" is run by a storeowner (a producer thread) who creates merchandize out of thin air (baby yodas in this case) and places them on the store on the shlelves (in a bounded-buffer). Customers (consumer threads) eagerly flood the shop to purchase (remove from the buffer) the baby yodas as soon as they can.  Once the baby yodas are gone (buffer is empty) the store (program) closes (exits.)


To run the program, open a terminal on a linux machine, navigate to the program location, and type the following following:

./babyyoda <buffer_size> <num_consumers> <max_items>

Buffer size, number of consumers, and max items should entered as whole integers.  

Be warned, the higher the numbers you enter for each parameter, the more taxing for your machine, and potentially the buggier the code.

The author notes, exhaustive end-user testing was not conducted, and consumer threads did not seem to exit semalessly, occassionally resulting in a deadlocked state.

The author chose to cancel consumer threads at program end as alternate resolutions were beyond his pressent skill level.

# Note: Commands are entered without quotations.
Use of quotations may cause errors and unpredictable outcomes with the update command based on user command input parsing limitations

_________________________________________________________________________________________________
# Under the hood: 🚙☁️☁️☁️

This program was programmed in c++ using a template test function (babyyoda.cpp), header file, and semaphore provided in this class. 

Special thanks to CodeVault (https://code-vault.net/) and the following video for an overview of how to approach this problem:
https://www.youtube.com/watch?v=l6zkaJFjUbM

https://www.tutorialspoint.com/index.htm was also useful utilized for learning thread functions in C++

Finally, special thanks to a classmate for recommending use of thread_cancel over thread_join, as after several hours of "troubleshooting" the author had no good ideas about how to deal with "hung" consumer threads and had resigned himself to warning the user to stay under a 3:2 ratio for number of consumers to buffer size.

Comments are used extensively in the file to acknowledge sources and references, but is by no means all inclusive.

If you have any questions, comments, or concerns, please feel free to reach out at anytime to reidlearnstocode@github.com