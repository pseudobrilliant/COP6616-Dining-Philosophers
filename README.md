Prime Numbers Solution

    I tackled this problem by first coming up with a sequential solution that would form the basis of my concurrent
	solution and provide a good comparison of efficiency and accuracy. This sequential solution simply runs through
	all the numbers from 3 to 〖10〗^8 by a delta of 2, since we know all prime numbers must be odd. As the solution
	runs through each number it checks whether it’s a prime by using the Miller-Rabin probabilistic method.
	This method determines the probability of a number being prime by running k times with k random numbers as a base,
	leading to a proven error of 4^(-k). By default, we use an accuracy of 9 leading to an error of 0.000003814697265625.
	If the number is prime the solution adds the number to a prime sum, and places the number in an array.

    To deal with the concurrent solution, we reuse the same basic concepts but in parallel to several other threads
	attempting to do the same work. To organize this chaos we attempted two solutions, a ticket counter similar to
	Lamport’s Bakery algorithm, and a simpler method of separating the work each thread had to do. After much effort I
	found the ticket solution would lead to more blocking than desired. Instead we decided on a Silo solution were each
	thread assigns itself numbers based off it’s ID that no other thread can have. Essentially, the given range of prime
	numbers is broken up into blocks by the number of threads given (8). Each thread is in charge of their initial odd
	numbers going forward by a delta of 8 every block. In this base case thread 0 will start with the initial number 3
	then 11,19, 27, etc while thread 1 starts with 5, 13, 21, etc. Each thread continues incrementing their number by
	the delta and storing whether it's prime (# or 0) till they pass the max. In this way each thread is doing the bulk
	of the calculation effort on numbers the thread is guaranteed to solely own since threadID+3+ ∆ <(threadID+1)+ 3+∆ .
	This same concept guarantees that as long as all numbers are recorded as the prime number or 0, all the thread
	numbers will be in order in relation to each other. Once all the threads have completed, their numbers are guaranteed
	to be in order amongst the threads so the main thread can fold them all in together. Summing all the sub totals,
	and creating a complete list of all the prime numbers in the range. In testing

Dining Philosophers

    **Solution 1 - Deadlock**
    
	5 philosopher threads are created, each with an assigned seat matching to their threadID. These 5 threads are vying
	for control of indexes in an array of atomic integer that represent the chopsticks in the theoretical problem. To be
	able to eat they must be able to claim the two chopsticks (indexes) nearest to them. It is important to note that this
	is a circular problem, since the last philosopher requires his chopstick (4) as well as the first chopstick (0).
	In our solution as soon as philosophers become hungry they attempt to atomically compare_exchange their left chopstick
	with their threadID. The threads waits as long as it finds any other number but -1, and then immediately swaps the -1
	for it’s own ID. In this way thread 0 first attempts to set chopstick 0 from -1 to 0, before attempting the same with
	chopstick 1. It is important to recognize that a thread will continue to wait as long as the next chopstick is in use.
	However, since this solution allows for a circular usage of the first and last sticks this may result in deadlock where
	in 0-1, 1->2, 2->3, 3->4, and 4->0 all the threads will be waiting on each other. Running this solution,
	the problem will be evident as the philosophers hang and proceed no further.

    **Solution 2 – No Deadlock**
    
	This solution attempts to address the problem of deadlock by structuring chopstick access through the order in which
	the philosopher access their required sticks. Essentially, when selecting the first chopstick a thread will consider
	the min of its two options before selecting the max as its second chopstick. Meaning chopstick 4 will now select 0
	as it’s first chopstick and then attempt to select chopstick 4. This method guards against our cyclic issue by
	forcing the last thread to wait until it has locked chopstick 0, while the other threads can grab resources as needed.
	Running this solution you can see that it does not hang as the first. However, due to their ordering
	(specifically the last and first thread) and processing intervals certain threads may be forced to defer chopsticks
	more than others. Some runs of this solution have shown thread 4 stuck between waiting for chopstick 0 being used by
	thread 0 before waiting for chopstick 4 being used by thread 3.  This is known as starvation, where any given thread
	is unable to access a resource regularly as they are beat out by greedier threads. After at least a 1000 ‘eatings’,
	the following representative result of our solution shows a large span error of 25% between the edge and center threads.
	--------------------------------------------------------------------------
	Philosopher #0 ate a total of 820 times.
	Philosopher #1 ate a total of 1057 times.
	Philosopher #2 ate a total of 1058 times.
	Philosopher #3 ate a total of 1058 times.
	Philosopher #4 ate a total of 820 times.
	Span = 237 Median =  938.5 
	Span Error = 25% of range
	--------------------------------------------------------------------------

    **Solution 3 -  No Deadlock / No Starvation**
    
	Tackling this solution I took the instructions in mind and made sure that my philosopher threads did not directly
	message or interact. However, to manage the resource usage properly


