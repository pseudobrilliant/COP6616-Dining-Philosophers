#Running Solutions

###Requirements
     Linux Environment
     CMAKE 3.5 or >
        
###Building

     Building this project is quite simple once you have CMAKE installed.
     From the root directory run the following:
            - cmake CMakeLists.txt
            - make
     This should have built all the executable files: 
            PrimeNum, Dining_Philosophers_1, Dining_Philosophers_2, Dining_Philosophers_3, Dining_Philosophers_4
     
     To run any of these use ./Dining_Philosophers_1 (example)
        

#Prime Numbers Solution

        I tackled this problem by first coming up with a sequential solution that would form the basis of my concurrent
    solution and provide a good comparison for efficiency and accuracy. This sequential solution simply runs through
    all the numbers from 3 to 〖10〗^8 by a delta of 2 (since we know all prime numbers must be odd). As the solution
    runs through each number it checks whether the number is prime by using the Miller-Rabin probabilistic method.
    This method determines the probability of a number being prime by running k times with k random numbers as a base,
    leading to a proven error of 4^(-k). By default, I use an accuracy of 9 leading to an error of 0.000003814697265625.
    If the number is prime the solution adds the number to a prime sum, and places the number in an array.

        To deal with the concurrent solution, I reused the same basic concepts but in a parallel setting with several 
    other threads attempting to do the same work. To organize this chaos I attempted two solutions, a ticket counter like in
    Lamport’s Bakery algorithm, and a simpler method of separating the work each thread had to do. After much effort I
    found the ticket solution would lead to more blocking than desired. Instead I decided on a Silo solution were each
    thread assigns itself numbers based off it’s ID that no other thread can have. Essentially, the given range of prime
    numbers is broken up into blocks by the number of threads given (8). Each thread is in charge of their initial odd
    numbers going forward by a delta of 8 every block. In this base case thread 0 will start with the initial number 3
    then 11, 19, 27, etc while thread 1 starts with 5, 13, 21, etc. Each thread continues incrementing their number by
    the delta and storing whether it's prime (# or 0) till they pass the max. In this way each thread is doing the bulk
    of the calculation effort on numbers the thread is guaranteed to solely own since threadID + 3 + ∆ < (threadID+1) + 3 + ∆ .
    This same concept guarantees that as long as all numbers are recorded as the prime number or 0, all the thread
    numbers will be in order in relation to each other. Once all the threads have completed, their numbers are guaranteed
    to be in order amongst the threads so the main thread can fold them all in together; Summing all the sub totals,
    and creating a complete list of all the prime numbers in the range. This aggregation point is the main bottleneck of
    this solution as it must be done sequentially. However, this concurrent solution still provides greater efficiency 
    than the sequential solution. As you can see from one of the sample results below, the sum and values match exactly 
    between the solutions, but the concurrent solution shows a considerable speed up of 0.68%. Which while not exactly 
    an 8 fold increase (for 8 threads) is still a considerable improvement.

    Solution: Sequential
    Running...Completed!
    Execution_Time: 82.3305(seconds)
    # of Primes Found: 5761455
    Prime Total: 279209790387283
    Top 10 Prime Numbers: 99999989 99999971 99999959 99999941 99999931 99999847 99999839 99999827 99999821 99999787
    
    ------------------------------------------------------------------------
    
    Solution: Silo
    Running...Completed!
    Execution_Time: 25.9838(seconds)
    # of Primes Found: 5761455
    Prime Total: 279209790387283
    Top 10 Prime Numbers: 99999989 99999971 99999959 99999941 99999931 99999847 99999839 99999827 99999821 99999787 
    
    ------------------------------------------------------------------------

#Dining Philosophers

###Solution 1 - Deadlock
    
       5 philosopher threads are created, each with an assigned seat matching to their threadID. These 5 threads are vying
    for control of indexes in an array of atomic integers that represent the chopsticks in the theoretical problem. To be
    able to eat they must be able to claim the two chopsticks (indexes) nearest to them. It is important to note that this
    is a circular problem, since the last philosopher requires his chopstick (4), as well as the first chopstick (0).
    In my solution as soon as philosophers become hungry they attempt to atomically compare_exchange their left chopstick
    with their threadID. The thread waits as long as it finds any other number but -1, and then immediately swaps the -1
    for its own ID. In this way thread 0 first attempts to set chopstick 0 from -1 to 0 without interfering with another thread,
    before attempting the same with chopstick 1. It is important to recognize that a thread will continue to wait as long 
    as the next chopstick is in use. However, since this solution allows for a circular usage of the first and last sticks 
    this may result in deadlock where in 0-1, 1->2, 2->3, 3->4, and 4->0 all the threads will be waiting on each other. 
    Running this solution, the problem will be evident as the philosophers hang and proceed no further.

###Solution 2 – No Deadlock
    
       This solution attempts to address the problem of deadlock by structuring chopstick access through the order in which
    the philosopher access their required sticks. Essentially, when selecting the first chopstick a thread will consider
    the min of its two options before selecting the max as its second chopstick. Meaning chopstick 4 will now select 0
    as its first chopstick and then attempt to select chopstick 4. This method guards against our cyclic issue by
    forcing the last thread to wait until it has locked chopstick 0, while the other threads can grab resources as needed.
    Running this solution you can see that it does not hang as the first. However, due to their ordering
    (specifically the last and first thread), and processing intervals, certain threads may be forced to defer chopsticks
    more than others. Some runs of this solution have shown thread 4 stuck between waiting for chopstick 0 being used by
    thread 0 before waiting for chopstick 4 being used by thread 3 (and vice versa).  This is known as starvation, where 
    any given thread is unable to access a resource regularly as they are beat out by greedier threads. After at least a 
    1000 ‘eatings’, the following sample result of our solution shows a large span error of 25% to 30%+ between 
    the edge and center threads.
    
    --------------------------------------------------------------------------
    Philosopher #0 ate a total of 820 times.
    Philosopher #1 ate a total of 1057 times.
    Philosopher #2 ate a total of 1058 times.
    Philosopher #3 ate a total of 1058 times.
    Philosopher #4 ate a total of 820 times.
    Span = 237 Median =  938.5 
    Span Error = 25% of range
    --------------------------------------------------------------------------

###Solution 3 -  No Deadlock / No Starvation
    
        Tackling this solution I took the instructions in mind and made sure that my philosopher threads did not directly
    message or interact each other. However, to direct the resource usage properly to those with priority without such 
    communication, I needed a form of direct management. Enter the host class. The host class has been implemented as a
    single manager with resources it controls and shares with the philosopher threads as a form of indirect communication.
   
       The host is in charge of prioritizing resource usage based on the number of times a thread has successfully used 
    a resource. Those threads that have had less access are brought to the front of the line on each pass the host makes, 
    to ensure that they get equal attention. To facilitate this, 3 more shared resource arrays and one flag are introduced. 
    When hungry, Philosophers start out by placing the number of times they have eaten in the reservation table at the index 
    equal to their philosopherID. The host will eventually see their reservation and store it in a private vector sorted 
    by times eaten and the philosopher' ID, while resetting the value to -1. The host will then turn to assigning seats 
    to waiting philosophers. The host first locks the chopsticks to take stock of the empty seats, then it begins assigning 
    seats by priority in the queue. Seat assignments are reported to the philosopher through a shared seating resource, where 
    each philosopher has an index. Once a Philosopher notices their reservation has been reset to -1 this thread will 
    continuously read the seating array until a seat is assigned, which they then reset to -1. Having successfully 
    acquired a seat, the philosopher then attempts to grab the lowest chopstick related to their seat first, before grabbing 
    the highest. Once the philosopher has both chopsticks, he eats, and eventually leaves the table. Before he can leave the 
    table the thread must wait until the host has finished checking the chopsticks, after which he is free to reset the chopsticks 
    and mark the seat as empty. This lock ensures that the host assigns the right number of seats, while still allowing the
    other threads to pick up empty chopsticks as the host is managing reservations. This loop continues indefinitely as
    the host manages reservations, philosophers leave / pickup chopsticks, and the host continues assigning seats.
    
        This solution guarantees that threads have regular access to resources because their managed priority is dependent
    on their usage frequency. If a thread is assigned to an unfortunate seat, or blocked by threads that take more time, 
    it will be brought in at a higher priority the next time it needs a resource. This solution is not perfect since 
    it enforces a bottleneck of sequential logic through waiting and logical locks as the host manages the table and 
    threads leave/wait. It is important to note, however, that only sections of the solution are sequentially blocking,
    multiple threads can still leave the table when the host is away, and multiple threads can begin snatching chopsticks 
    when the host resturns. Also the host can still inefficiently assign seats, and does not provide a method for sharing
    resources within the queue before a new philosopher arrives. At the same time, this implementation does show a 
    considerable improvement over the non-host solution. This can be seen in the included result sample, where the Span 
    error is generally between 6% and 8%. Continued operation shows that this minimal starvation travels across the threads 
    as each thread gains and then loses priority. This method could be improved with the addition of resource sharing 
    within the seated threads. When done, and before leaving the table, a thread could check with it's neighbors if they
    need a resource. However, that would require more direct communication or even more managmeent on the side of the host, 
    and this solution still consistently shows a considerable advantage over the non-managed solution.
    
    --------------------------------------------------------------------------
    Philosopher #0 ate a total of 1023 times.
    Philosopher #1 ate a total of 1039 times.
    Philosopher #2 ate a total of 965 times.
    Philosopher #3 ate a total of 1027 times.
    Philosopher #4 ate a total of 992 times.
    Span = 74 Median = 1002
    Span Error = 7.4%
    
###Solution 4 -  No Deadlock / No Starvation With N Philosophers

        When first reading the assignment for solution 3 and 4, I made a rather large assumption. I read that the number 
    of philosophers N could be set dynamically, but I did not see anything mentioning a change in the number of chopsticks.
    This lead me to assume that we would be required to implement N philosophers with fixed chopstick resource at 5.
    This lead me towards the host solution where resources could be dynamically allocated to any number of threads, 
    while attempting to prioritize access for each thread. As a byproduct of this assumption, my host solution not only 
    supports N number of chopsticks, but C number of chopsticks. By having a central manager that can keep track of both
    threads and resources at once it becomes much easier to manage and prioritize access access between them. The implementation,
    impolications, and results of this are essentially the same as solution 3.
    
    --------------------------------------------------------------------------
    Welcome to the Dining Philosophers restaurant!
    Please enter the number of philosophers N.
    10
    Please enter the number of chopsticks C or press enter to use 5.
    8
    
    ......
    
    Philosopher #0 ate a total of 1009 times.
    Philosopher #1 ate a total of 1021 times.
    Philosopher #2 ate a total of 1006 times.
    Philosopher #3 ate a total of 971 times.
    Philosopher #4 ate a total of 997 times.
    Philosopher #5 ate a total of 989 times.
    Philosopher #6 ate a total of 1009 times.
    Philosopher #7 ate a total of 986 times.
    Philosopher #8 ate a total of 1020 times.
    Philosopher #9 ate a total of 1030 times.

