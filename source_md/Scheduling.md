## Scheduling

***

[TOC]

***

### What Is Processor Scheduling

> throughput [生产量]
>
> batch [批量]
>
> proportionality [匀称]

#### Definition

* the allocation of processors to threads over time

#### Role

* the key to multiprogramming
  * increase CPU utilization and job throughput
  * overlap I/O and computation

#### Mechanism

* thread state
* thread queue

#### Policy

* how to choose the next runnable thread
* when to make decision

#### Thread Life Cycle

* Repeatedly alternate between computation and I/O

  * CPU Burst
    * Last CPU Burst terminate the thread ( i.e. ```thread_exit()``` )

  * I/O Burst
    * No need for CPU during I/O Burst 
    * opportunity to execute another thread

* Bound Type

  * CPU-Bound

    * Very long CPU Burst + Infrequent I/O Burst

  * I/O-Bound

    * Short CPU burst + Frequent/long I/O Burst

* Recall State Diagam

  * see as Process-Process Cycle
  * New $\stackrel{Admit}{\longrightarrow}$ Ready  $\stackrel{Dispatch}{\longrightarrow}$  Running( **CPU Burst** ) $$ \left\{\begin{matrix} ---------\stackrel{Time-out}{\longrightarrow} \text{Ready} \\ \\ \stackrel{Event-Wait}{\longrightarrow} \text{Blocked }\textbf{(I/O Burst)} \stackrel{Event-Occur}{\longrightarrow} \text{Ready}   \\  \\ -----------\stackrel{Release}{\longrightarrow} \text{Exit}  \\ \end{matrix} \right. $$
  * Thread/Process is blocked during I/O Burst not using CPU
  
#### Goal

==somtimes conflict with each other==

* For All System

  * Fairness
    * each process receive fair share of CPU
  * Avoid Starvation
  * Policy Enforcement
    * usage policies should be met
  * Balence
    * all parts of the system should be busy

* For Batch System

  * Throughput
    * maximize per-hour-completed jobs
  * Turnaroung Time
    * minimize time between submission and completion
  * CPU Utilization
    * keep the CPU busy all the time

* For Interactive System

  * Response Time
    * minimize time between receiving request and **starting** to produce output
  * Proportionality
    * simple tesks complete quickly

* For Real-time System

  * Meet deadlines
  * Predictablity

  

#### Short Term Scheduling

* Definition
  * aka Dispatching
    * dispatching a process from the ready queue == context switching
  * happen frequently
  * need to be efficient
    * fast context switch
    * fast queue manipulation
* Other Type Of CPU Scheduling (in terms of Time)
  * Long-term Scheduling 
    * aka Admission Scheduling / Admission Control
    * used in Batch System
    * not common today
  * Medium-term Scheduling
    * aka Memory Scheduling
    * sometimes called long-term with admission control ignored
    * decide which processs are swapped out to disk
* Recall Dispatch Process
  1. save currently running process state
     * unless the current process is exiting
  2. select next process form ready queue
  3. restore state of next process
     * restore registers
     * restore OS control structures
     * switch to user mode
     * set PC to next instruction in this process

#### When To Schedule

* enter Ready state
  * I/O Interrupt
  * Signal
  * Thread creation/admission
* thread blocks/exits
  * OS SysCall
  * Signal
* At fixed intervals
  * Clock interrupt

#### Type Of Scheduling

* Non-preemptive Scheduling
  * once CPU has been allocated to a process
    * it keep the CPU until it terminates/block
  * suitable for batch system
  * common for user-level thread package
* Preemptive Scheduling
  * CPU can be taken from a running process and allocated to another
  * Needed in interactive or real-time system



### Scheduling Algorithm

> FCFS == First Come First Server
>
> RR == Round Robin
>
> convoy [护航]

#### FCFS

* Characteristic 
  * first come first served
  * non-preemptive
* Functionality
  * choose the process at the head of the FIFO queue of ready process
* Limitation
  * avg waiting itme under FCFS  is often long
    * cause convoy effect
      * all other processes wait for the big process to release the CPU

####  Shortest-Job-First

* aka Shortest-Process-Next / SJF / SPN
* Characteristic
  * provably optimal w.r.t ==avg waiting time==
  * non-preemptive
    * notice that preemptive version is "Shortest-Remaining-Time"
* Functionality
  * choose the process with the shortest expected procesing time
    * programmer estimate
    * history statistic
    * shortest-next-CPU-burst for interactive jobs

#### Round Robin

* Characteristic

  * designed for time-sharing system
  * preemptive
  * ready queue is circular
* functionality 

  1. Each process is allowed to run for a time quantum $q$ 

  2. Then preempt this process and put it back on queue
  * notice that Critical Choice of Quantum ( i.e. time slice )
    * As $q \to \infin $ $\Rightarrow$ RR $\to$ FCFS
    * As $q \to 0 $ $\Rightarrow$ Overhead of switching overwhelms useful time
  * so we wnat $q$ to be large depends on the Context Switch Time (==WHY?==)

#### Round Robin Efficiency

* Example Problem
  * Measurements of a certain system have shown that 
    * the average process runs for a time T before blocking on I/O.
    *  A process context switch requires a time S, which is effectively wasted (overhead). 
    * The performance measure of interest is CPU efficiency, defined as
      *  the ratio of useful CPU time over total CPU time. 
    * Round-robin (RR) scheduling with a quantum of length Q
  * Give a formula for CPU efficiency in each of the following cases  for RR scheduling
  
* Q = $\infin$ 

  * No involuntary context switches will occur
  * Each process will pay 1 context switch per CPU burst
  * Useful = T
  * Total = T + S
  *  Efficiency = $\frac{T}{T+S}$

* Q > T

  * As long as the Q > T $\Rightarrow$ No involuntary context switches will occur
  * Useful = T
  * Total = T + S
  *  Efficiency = $\frac{T}{T+S}$

* S < Q < T

  * Since T > Q $\Rightarrow$ each process will cannot finish the task in one quantum 

  * So calculate the number of time quantum a process need 

    * m = $\frac{T}{Q}$ (a process finish need T and each time only have Q time to work so T/Q should be the slice we seperate T by Q)

  * since each process need to do the context switch for each time quantum they need to use based on m

    * a process need to do m times context switch in its cycle

    * we know context switch per each time cost S

  * So calculate the total cost of context switch for each process

    * costS = m$\times$ S = $\frac{ST}{Q}$

  * Useful = T

  * Total = T + $\frac{ST}{Q}$

  * Efficiency =  T / (T + $\frac{ST}{Q}$) = $\frac{1}{1 + \frac{S}{Q}}$

* Q = S

  * if Q < T 0.5 (see equation above)
  * if Q > T same as Q > T

* Q nearly 0

  * efficiency approaches to 0  (see equation above)

### Priority Scheduling 

> MLQ == Multi=Level Queue
>
> dk == don't know
>
> criteria [评判标准]

#### Priority Policy

* Definition
  * A priority is associated with each thread
  * Highest prority job is selected form Ready queue
    * ready queue can be  preemptive or non-preemptive
* Key Enforcment Problem
  * Priority Inversion
    * a low priority taks may prevent a high priority task from making progress by holding a resource
  * Starvation
    * a low priority task may never get to run

#### Multi-Level Queue Scheduling

* Definition
  * multiple ready queue
  * each runnable provess is on only one queue
  * threads are permanently assigned to a queue
    * criteria
      * job class
      * priority 

* Characteristic
  * Each queue has its own scheduling algorithm
    * feedback scheduling decides which queue to choos next
    * usually priority -based

#### Feedback Scheduling

* Definition

  * adjust criteria for choosing a particular thread based on past history
    * boost priority of threads that have waited a long time / ==aging==
    * prefer threads that do not use full time quantum
    * boost priority following a user-input event
    * adjust expected next CPU-Burst
* Usage
  * combine with MLQ to move threads between queues

#### Multi-Level Feedback Queue

* Structure
  * multiple distinct queues
    * each one assigned a different priority level
    * each one heas multiple ready-to-run jobs
* Functionality
  * always choose to run the jobs in the highest-priority queue
  * always put jobs start in the highest priority queue
  * check FEEDBACK to update job in the correct priority of the queue
* FEEDBACK
  1. A job uses an entire time quantum $\Rightarrow$ priority &downarrow; move to lower-priority queue
  2. A job give up the CPU before using up the time quantum $\Rightarrow$ priority -- stay at the same-priority queue

#### Priority Inversion

* Definition

  * when a lower priority thread prevents a high priority thread from running

    1. a low priority thread holds a lock and ti preempted
    2. a high priority thread who wants that lock 

    * $\Rightarrow$ cannot make progress until the low priority thread runs again

* Example: Pathfinder

  * Precondition

    * 3 thread

      * | Priority Level | Thread Type         | Acquire the Information Bus? |
      | -------------- | ------------------- | ---------------------------- |
      | High           | Bus Management Task | Yes                          |
      | Medium         | Communication Task  | No                           |
      | Low            | Data Gathering Task | Yes                          |
      
    * Scheduling
      
      * a mutex lock synchronize the information bus
      * higher priority preempt lower priority
      * watchdog reset the system if a thread hasn't run recently
    
  * Result
  
    1. low come$\Rightarrow$preempt?false$\Rightarrow$self start$\Rightarrow$request lock?true$\Rightarrow$lock-available?true$\Rightarrow$self obtain+hold lock exe
    2. med come$\Rightarrow$preempt?true$\Rightarrow$previous task: low completed? false$\Rightarrow$self start + low hold-lock block $\Rightarrow$request lock? false$\Rightarrow$self exe + low hold-lock block
    3. high come $\Rightarrow$ preempt? true$\Rightarrow$previous task: med completed? false$\Rightarrow$self start + med block (notice that high don't know low blocked ) $\Rightarrow$request lock? true $\Rightarrow$lock-available?**FALSE**  (**low hold lock but high dk **)$\Rightarrow$self **waiting-accuire-lock block** by med  $\Rightarrow$ enqueue blocked exe [**Priority Inversion**]
    4. queue.pop(): med exe + low hold-lock block + high waiting-lock block$\Rightarrow$self exit $\Rightarrow$ enqueue blocked start
    5. queue.pop(): low exe + high waiting-lock block $\Rightarrow$ self exit + self release lock $\Rightarrow$ enqueue blocked start
    6.  queue.pop(): high exe $\Rightarrow$lock-available?true$\Rightarrow$self obtain+hold lock exe$\Rightarrow$self exit 
  
  * Cause
  
    * data gathering task lock the bus 
    * if preempted by bus management task, data gethering task will blocks but self hold the lock
    * if communications task becomes runnable, data gathering task can't complete and release the lock
    * high priority task stays blocked
  
  * Solution sample
  
    * Priority inheritance
      * when high priority task request the lock, the priority of holding-mutex tasks inherits this high priority 
      * notice that this allow low priority task to preempt the medium priority task
  
#### Fair Share Scheduling

* Functionality
  * group processes by user/department
  * each group receives a proportional share of CPU ( not enforced equal )
  * priority of a process depends on own priority and past history of the entire group
  * lottery scheduling 
    * each group is assigned "ticket" according to its share
    * hold a lottery to dind next process to run



### Scheduling Exampe In Real World OS

> RR == Round Robin
>
> STS == 
>
> CFS = Completely Fair Scheduler

#### Unix CPU Scheduling

* Favoured by interactive process

* Functionality

  * small CPU time quantum is given to processs by a priority algorithm that reduce to RR for CPU-Bounding jobs
    * MLFQ with RR within each priority queue
    * priority is based on process type and execution history
  * spend more CPU time == lower priority
  * use process aging to prevent starvation

* Frequency

  * resechedule process every 0.1 sec
  * recompute priority every sec

* RR scheduling results from a timeout mechanism(?)

* Priority Formula

  * Annotation

    * Let $j$ be a process
* Let $i =[s, t]$ be the time quantum interval of process can run
    * Let $P_j(i)$ be the priority of the process $j$ at the time quantum interval $i$ 

      * notice that lower value == hight priority
* Let $Base_j$ be the base priority of $j$
    * Let $U_j(i)$ be the processor utilization of $j$ in $i$
* Let $CPU_j(i)$ be the average processor utilization of $j$ in $i$
    * exponentially weighted
* Let $User_j$ be the user-controllable adjustment factor
  * Formula
*  $CPU_j(i) = \frac{CPU_j(i-1)}{2} + \frac{U_j(i)}{2}$
    * $P_j(i) = Base_j + \frac{CPU_j(i)}{2} + User_j$

#### Linux 2.4

* Scheduling Algorithm

  * Time-Sharing
  * Real-Time Task
* Time-Sharing Functionality
  * a prioritized credit-based algorithm
    1. STS choose process with the most credit
    2. running process lose 1 credit / timer interupt
    3. suspended if credit == 0
    4. if all runing process credit == 0 $\Rightarrow$ re-crediting
       * For **every** process $j$ 
         * credit $_j$ += $\frac{1}{2}$credit $_j$  + priority $_j$ 
* Limitation
  * Re-Crediting step Time Complexity $\in$ O(n)
  * Too much time making scheduling decision for large scale system 

#### Linux 2.6 (nice)

* O(1) Scheduler

  * Introduced by Linux 2.5 kernel

  * Each process gets a time quantum boased on its priority
  * 2 arrays of runnavle process queue
    * Active
      * select processes in this queue to run
    * Expired
      * put the exhausted-quatum process into this queue
  * when Active queue is empty
    * swap the two arrays

* Structure

  * Active Run Queue
  * Expired Run Queue
  * 140 priority queue level PQ  For each Run Queue
    * present for different priority level
      * each priority level set up its own time quantum
    * PQ[ 0 : 100] = Real Time Task
    * PQ[100:140] = Time Sharing

* Functionality 

  1. Thread T come in
  2. Schedule T as one priority queue PQ[a]
  3. Put T in PQ[a] of Active Queue
  4. Start running until it uses up time slice of PQ[a]
     * Notice that T will change to other PQ during running since been awaken or preempted
  5. Recalculate priority and time slice for T as PQ[z]
  6. Put T in PQ[z] of Expired Queue
  7. When Activity Queue is empty $\Rightarrow$ Swap Activity Queue with Expired Queue

* Scheduling Algorithm

  1. O(1) Scheduling algorithm
     * use Bitmap
       * indicate the loction of non-empty priority queues in Activity Queue
     * use Instruction ```find-first-bit-set``` 
       * find the firest non-empty priority queue in Activity Queue
     * O(1) Scheduler
  2. Dynamic Priority
     * Initial priority of Thread $P_T$= 120
     * If  awaken $\Rightarrow$ $P_T$++ 
       *  max = +5
     * If preempted by other $\Rightarrow$ $P_T$ --
       * max = -5

* Pro

  * Get the threads out of the kernel fast

#### Linux 2.6.3

* CFS
  * Weighted fair queuing
    * divid CPU cycles among threads in proportion to their weights
* Algorithm
  1. assign T a time slice Q
     * Q based on the #threads in the system
  2. v_runtime ++ when T runs
  3. build the run queue as Red-Black Tree 
  4. insert T in the run queue by v_runtime
  5. pop the lowest v_runtime thread in the run queue to run next

#### Windows CPU Sheduling

* aka Win NT Schduler
  * preemptive scheduler
* Scheme
  * 32-Level MLFQ
    * Q [ 0 : 16 ] = variable class
      * threads priority begin at some initial assigned value then change
      * each priority are in a FIFO queue
    * Q [ 16 : 31 ] = real-time class
      * threads get a fixed priority 
      * given priority are in a RR queue
* Next Run
  * Dispatcher reacerses the queues from highest to lowest until it finds a ready thread
* Priority Change
  * variable class thread's time quantum == 0 $\Rightarrow$ priority &downarrow;
  * variable class thread is released from a wait $\Rightarrow$ priority &uparrow;

### Summary

#### Definition

##### Starvation

* a condition in which a process is blocked indefinitely because another process is always given preference

* may occure because of priority scheduling or unfair synchron

##### Turnarround Time

* the difference between the time at which a process arrives and the time at which it completes