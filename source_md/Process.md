

## Process

***

[TOC]

***

### OS View Of Process

> OS == Operating System
>
> PID == Process ID
>
> PC == Program Counter
>
> PCB == Program Conrol Block
>
> Proc-struct == Process-Struct
>
> Encapsulate [封装]
>
> handler [处理程序]
>
> restore [恢复]
>
> routine [程序]

#### Process Structure

*  A process contains all of the state for a program in execution
* Usually called PCB

* **Structure**

  * PID

  * Address Space
    * code + data for executing program
    * Heap
    * register
      * general-purpose with current value
  * Execution State 
    * stack + stack pointer
      * encapsulated the state of procedure calls
    * PC
      * indicating the next instruction
  * OS resource
    * open files
    * open file table
    * network connection
    * signal

* **Linux Process Control Blk : task_struct **

  * ```c
    long state;
    ```
  * ```c
    long counter;
    ```
  * ```c
    unsigned long flags;
    ```
  * ```c
    struct task_struct *next_run, *prev_run;
    ```
  * ```c
    unsigned long blocked;
    ```
  * ```c
    unsigned long saved_kernel_stack;
    ```
  * $\cdots$ 

#### Process Life Cycle

* **State**
  * New
  * Ready
  * Blocked
  * Running ( CPU working env )
  * Exit
* **Cycle**
  * New $\stackrel{Admit}{\longrightarrow}$ Ready  $\stackrel{Dispatch}{\longrightarrow}$  Running( **CPU Burst** ) $$ \left\{\begin{matrix} ---------\stackrel{Time-out}{\longrightarrow} \text{Ready} \\ \\ \stackrel{Event-Wait}{\longrightarrow} \text{Blocked }\textbf{(I/O Burst)} \stackrel{Event-Occur}{\longrightarrow} \text{Ready}   \\  \\ -----------\stackrel{Release}{\longrightarrow} \text{Exit}  \\ \end{matrix} \right. $$

#### Keep Track Of Process

* a collection of **State Queue**
  * represent the state of all processes in the sys
* one queue for each state
  * ready queue
  * waiting queue for event X
  * $\cdots$
* a process changes state by unlink PCB from one queue and link into another

#### From Program to Process (Initial Process)

* Initial Process ( New $\stackrel{Admit}{\longrightarrow}$ Ready  $\stackrel{Dispatch}{\longrightarrow}$  Running )
  1. Create new process
     * Create new PCB
     * Create user address space structure
     * Allocate memory
  2. Load executable
     * Initialize start state for process
     * Change/link state to *Ready*
  3. Dispatch process
     * Change state to *Running*

#### State Change: Ready to Running

* Context Switich
  * switch the CPU to another process
    1. save the state of the old process
    2. load the saved state for the new process
  
* kernel mode working process
  1. Process voluntarily call ```yield()``` system call
  2. Proces makes other system call and block itself
  3. Time interrupt handler decide to switch process
  
* Example Switch Detail: Process A $\to$ Process B

  | USER Level            | HARDWARE Level                              | OS KERNEL MODE Level                          |
  | --------------------- | ------------------------------------------- | --------------------------------------------- |
  | Process A (U $\to$ H) |                                             |                                               |
  |                       | 1. ```TimerInterruptHandler.interrupt(A)``` |                                               |
  |                       | 2. ```KernelStack(A).save(Reg(A))```        |                                               |
  |                       | 3. Move to kernel mode (H $\to$ K)          |                                               |
  |                       | 4. Jump to **Trap Handler**                 |                                               |
  |                       |                                             | 1. Handle **Trap**                            |
  |                       |                                             | 2. Call ```switch()``` routine                |
  |                       |                                             | 3. ```Proc-struct(A).save(Reg(A))```          |
  |                       |                                             | 4. ```Proc-struct(B).restore(Reg(B))```       |
  |                       |                                             | 5. Switch to KernelStack B                    |
  |                       |                                             | 6. Return from Trap (now B ready + K $\to$ H) |
  |                       | 1. ```KernelStack(B).restore(Reg(B))```     |                                               |
  |                       | 2. Change/Move to user mode (H $\to$ U)     |                                               |
  |                       | 3. Jump to B's PC                           |                                               |
  | Process B             |                                             |                                               |

  

#### Process Destruction

* SysCall```exit()``` Problem

  * a process voluntarily release all resources when invoking ```exit()```  
  * but OS can't discard everything immediately
    * must stop running the process to free everything
    * require context switch to another process
    * parent may be waiting or asking for return value
  * result : not all data free

* Zombie

  * - [ ] PID retain
    - [ ] Some DS retain the process's exit state
    - [x] Address space free
    - [x] Files closed
    - [x] resource deallocated

  * until parent clean up

#### Process Creation

* Type

  * the $1^{st}$ process
    * ```init(PID 1)``
  * $>1$ process == child process
    * created by parent ( i.e. parent is creator)
  * a process is always created by another process

* Linux Parent ID Commend line

  * ```shell
    grep 'PPID' `ps -f`
    ```

* Inheritance

  * some OS like Unix
    * parent Process define/donate resource + privilege for children process

* State Change Between Parent And Childern

  * parent wait created child to finish its task
  * parent parallel process with child
  * both

### Process Creation In Unix

> PCB == Process Control Block

#### Inherirance Real Creation SysCall

* function
  * ```c
    int fork()
    ```
  
* feature
  
  * Notice that
    * The child does not inherit its parent’s memory locks and semaphore adjustments
    * The child does not inherit outstanding asynchronous I/O operations  and contexts from its parent
  
  1. create and init a new PCB
  2. create a new address space
  3. init the address space with ==a copy of the entire contents of the address space of the Parent==
  4. init the kernel resource to point to the resources ( like open files ) ==used by Parent==
  5. place the PCB on the ready queue
  
* return value
  * in Parent
    * child's PID
  * in Child
    * 0
  
* code example
  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <unistd.h> // necessary in C99
  
  int main(int argc, char *argv[]){
    
    char *name = argv[0];
    int child_pid = fork();
    if(child_pid == 0){
      printf("child of %s is %d \n", name, getpid());
      return 3;
    }else{
      printf("my child is %d\n", child_pid);
      return 0;
    }
  }
  //output1:
  //my child is 88988
  //child of ./a.out is 88988
  //or
  //output2:
  //child of ./a.out is 88988
  //my child is 88988
  // SINCE Both parent and child processes are executed simultaneously
  ```
  
#### New Program Replacement SysCall

* function
  * ```c
    int exec(char *program, char * argv[])
    ```
* feature

  * Notice that **it does not create a new process**

  1. stop the current process
  2. load program ```program``` into the process' address space
  3. init hardware context and arguments for the new program
  4. place the PCB onto the ready queue

* return value

  * The ```exec()``` functions return -1 if and only if an error has occurred. 
  



### System Call

> OS == Operating System
>
> IO == Input + Output
>
> DMA ==
>
> TLB == 
>
> Explicit [清晰明确]
>
> poll [投掷]
>
> halt [终止]
>
> idle [闲置]
>
> routine [程序]

#### What Is A System Call

* Definition
  * a function call that invokes the OS
* Permission Requirement
  * whenever an app wanna use a OS resource it should ask permission
* Goal
  * Actually invoke the OS
  * Keep app from using a resource without asking permission

#### Interrupt

* Interrupt can be caused by hardware or software
  * hardware runs in either User Mode or System/Kernel Mode
  * CPU has at least 2 protection level: Kernel Mode and User Mode
* OS is an event-driven program
  * OS respond to request
* Interrupt Signal Object + Circumstance
  * CPU 
    * a hardware device has an event that needs attention
    * like Disk I/O completes
  * error/request
    * for OS intervention/sysCall
    * often called an exception/trap
  * the interrupt handler
    * CPU jumps to a pre-defined routine
  * enforcing restriction
    * the hardware mode bit is swithced to allow privileged instrunction to occur 

#### Boundary/Mode Crossing

* Getting/Moving/Changing To Kernel Mode (U $\to$ H $\to$ K)

  1. Explicit System Call

     * request for service by app

  2. Hardware interrupt

     * ```TimerInterruptHandler.interrupt(old_proscess)```
     * ```KernelStack(old_process).save(Reg(old_proscess))```
     * Move to kernel mode (H $\to$ K)

  3. Software Trap/exception

     * Jump to Trap Handler
     * Handle Trap

     * Hardware has table of "Interrupt Service Routine"
       * Call ```switch()``` routine
         * ```Proc-struct(old_proscess).save(Reg(old_proscess))```
         * ```Proc-struct(new_process).restore(Reg(new_process))```
         * ```Switch to KernelStack(new_process)```
         *  ```Return from Trap (now into new_process)```

* Kernal Mode To User (K $\to$ H $\to$ U)

  1. Set up registers for app
     * ```KernelStack(new_process).restore(Reg(new_process))```
  2. Set up MMU for app
  3. Set up mode for app
     * Change/Move to user mode (H $\to$ U)
  4. Jump to next app instruction
     * ```Jump to new_process' PC```

#### Privileged Instruction

* Cause
  * hardware runs in either User Mode or System/Kernel Mode
  * privileged Instruction can only run in hardware's system/kernel mode
  
* Type
  * Access I/O Device
    * poll for IO
    * perform DMA
    * catch hardware interrupt
  * Manipulate Memory Management
    * set up page tables
    * load/flush the TLB and CPU caches
  * Configure Various Mode Bit
    * interrupt priority level
    * software trap vector
  * Call halt instruction
    * put CPU into low-power
    * idle state until next interrupt
  * Instruction Enforced by the CPU Hardware Itself
    * CPU check current protection level on each instruction
  
* When user program try to execute a privileged instruction
  
  * Invoking System Call Interface 
    1. user program 
       * call C library function with arguments
    2. C library function 
       * pass arguments to OS ==including a sysCall Identifier==
    3. execute special instruction (x86: INT) to **trap** to system/kernel mode
       * interrupt/trap vector transfers "control" to a "syscall handler routine"
    4. sys call handler
       * figure out which system call is needed
       * call a routine for that operation
  * Goal
    * Keep user program from using a resource without asking permission

#### System Call Dispatch

* Precondition

  * kernel must verify arguments that it is passed
    * may wrongly call system function then change the OS resource
  * a fixed number of arguments can be passed in registers
    * often pass the adddress of a user data buffer
    * kernel must copy data from user space into ites own buffers
  * result of sysCall should returned in register **EAX**

* SysCall (Identify) Number
  * Each system call is identified by a unique umber
  * Passed into register ```%eax```
  * Offer the index into sys_call_table
  
* sys_call_table
  * Array < sys_call_num: function_porinters >
  * each entry ```sys_call_table[ __NR_syscalls ]```
  * approximately 300 sysCall
  
* Passing SysCall Prarmeters

  * param[0] == sysCall_NUM
    * stored in EAX
  * param amount limitation
    * can directly pass up to 6 param (exclude EAX)
      * ebx
      * ecx
      * edx
      * esi
      * edi
      * ebp
    * if more than 6 params are needed
      * package the rest $\ge$ 6 params in a struct and pass a pointer to the $6^{th}$ param
      * problem
        * must validate user pointers to keep out error
      * solution
        * safe functions to access user pointers
          * ```copy_from_user()```
          * ```copy_to_user()```

* Dispatch Process
  1. Kernel
     * assign each sysCall type := sys_call_num (unique)
     * init sys_call_table
       * mapping each sys_call_num to a sysCall function
  2. User Process
     * set up specific sysCall num + argument
     * run ```int N``` (N = 0x80 on Linux)
  3. Hardware
     * switch to kernel mode
     * interrupt dispatch
       * invoke kernel's interrupt handler for X 
  4. Kernel
     * look up sys_call_table using specific sys_call_num
     * invoke the corresponding function
     * return by running inrerrupt return: ```iret``` 

* Example: ```write()``` in Linux
  1. Start as User Mode
     
  2. User program code
  
     * ```c
       printf("hello world\n"); // before compiling
       ```
  
  3. Compiling call the C library function
     * ```c
       %eax = sys_write; // After compiling
       int 0x80 // sysCall Identifier
       ```
  
  4. C library function pass argument ```%eax``` and sysCall identifier ```int 0x80``` to OS
  
  5. Execute special instruction ```int 0x80``` to trap User Mode $\to$ Kernel Mode
  
  6. Interrupt Descriptor Table store vector ```int 0x80``` for transfer
  
  7. Interrupt Descriptor vector  transfer ```int 0x80``` to ```system_call()``` handler routine
  
     * ```c
       int system_call(){
         sc = sys_call_table[%eax] \\ now %eax = int 0x80
       }
       ```
  
  8.  ```system_call()``` handler figure out  ```int 0x80``` identify ```sys_write()``` by looking up in sys_call_table
  
  9.  ```system_call()``` handler call ```sys_write()```
  
  10. return result into ```%eax```
  
* Intel Fast System Call

  * Linux used to implement system calls using ```int 0x80```
    * too slow on Pentium IV+
  * New way SYSENTER/SYSEXIT instruction




### System Call In Linux

> params == parameter

#### SysCall Defining In Linux
* Macro/Kernal difined as fllows (X = number of params in sys_call_table)
  * ```pseudocode
    SYSCALL_DEFINEX(name, arg1type, arg1name, ...)
    ```
* Example Defining ```write()```
  
  * number 4 in the sys_call_table
  
  * ```c
    long write(unsigned int fd, const char* buf, size_t count); // in C/C library func
      
    SYSCALL_DEFINE3(write, unsigned int, fd, const char*, buf, size_t, count) // in Kernel
      
    asmlinkage long sys_write(unsigned int fd, const char* buf, size_t count); // Naming
    ```
  
      

#### SysCall Invoking In Linux

* Can invoke any system call from userspace using the ```syscall()``` function

  * ```pseudocode
    syscall(syscall_num_of_param, arg1, arg2, arg3, ...)
    ```

* Example Invoking  ```write()```

  * number 4 in the sys_call_table

  * ```c
    const char msg[] = "hello world";
    syscall(4, STDOUT_FILEDESCRIPTOR, msg, sizeof(msg)-1);
    // Equivalent to
    write(STDOUT_FILEDESCRIPTOR, msg, sizeof(msg)-1);
    ```

#### Tracing SysCall In Linux

* Powerful mechanism
  * trace sysCall execution for an app
* ```strace``` command
* ```ptrace()```system call
  * it is used to implement ```strace```
  * it is also used by ```gdb```
* Library calls can be traced using ```ltrace``` command



### Process And Thread

> PCB == Process Control Block
>
> DS == Data Structure
>
> IPC == Inter Process Communication
>
> SP == Stack Pointer
>
> TCB == Thread Control Block
>
> Thread [线程]
>
> Media [介质]
>
> Procedure [程序]
>
> Integrated [集合]

#### Interprocess Communication

* Indirect Communication Media

  * signal
  * pipe
  * socket
  * file

* Shared-Memory Communication

  * ```shmget() + shmat()```
    * ```shmget()```
      * sysCall: tell OS to allocate a Shared Memory Region
    * ```shmat()```
      * sysCall: map Shared memory to local address
  * ```mmap()```
    * create Shared Memory Region

* Example

  * ```c
    int * A;
    shared_mem_id = shmget(key, size, flags); // shared memory created
    A = (int *)shmat(shard_mem_id, NULL, 0); // local addr A now map with shared_mem_id
    fork(); // can do parallel process work now
    ```

#### Parallel Program

* Traditional Creation Of Parallel Program ( i.e. single thread )

  * Common Case Of Using Parallel Program
    * execute a web server
    * any program using ```fork()```
  * Creation Process
    1. create multiple isolated process that execute in parallel
    2. map each to the same address space/shared memory region to share data
       * notice that all of them must be the same computation
    3. OS schedule these processes in parallel logically/physically
    * Problem: Inefficient
      * space complexity &uparrow;
        * PCB 
        *  Page Table 
        * $\cdots$
      * time comlexity &uparrow;
        * create DS
        * fork and copy addr space
        * $\cdots$
      * IPC
        * extra work is needed to share and communicate across isolated processes
          * since ```fork()``` create isolated process

#### From Process to Thread

* Key Idea

  * separate the address space of Process from the execution state of Process
  * let the execution state of Process be one thread of execution
  * then multple threads of execution can execute in a single address space

* Process Structure Update Image

  | Single Thread Process                   | Muti Thread Process                                      |
  | --------------------------------------- | -------------------------------------------------------- |
  | Address Space                           | Address Space                                            |
  |                                         | Execution State 1 (Thread of execution 1)                |
  | Execution State (Thread of execution 1) | Execution State 2 (Thread of execution 2)                |
  |                                         | $\cdots$                         (Thread of execution n) |
  | OS resources                            | OS resources                                             |

* Updated Pro

  * sharing
    * threads can both solve a single problem concurrently and can easily share  share code/heap/gloable vars
  * lighter weight
    * faster to create and destroy
    * potentially faster context switch times
  * concurrent programming performance gain
    * overlapping computation and I/O

#### What Is A Thread

* Definition

  * a single control flow through a program
  
* Multithread == Multiple control flows

  * OS interact with multiple running programs $\Rightarrow$ multithreaded

* Multithreaded Process Address Space

  * |              | Thread                                | PC                      | SP     |
    | ------------ | ------------------------------------- | ----------------------- | ------ |
    | Stack (T1)   | connect PC(T1) and SP(T1) by Thread 1 |                         | SP(T1) |
    | Guard Region |                                       |                         |        |
    | Stack (T2)   | connect PC(T2) and SP(T2) by Thread 2 |                         | SP(T2) |
    | Guard Region |                                       |                         |        |
    | Stack (T3)   | connect PC(T3) and SP(T3) by Thread 3 |                         | SP(T2) |
    | Guard Region |                                       |                         |        |
    | $\cdots$     |                                       |                         |        |
    | Heap         |                                       |                         |        |
    | Static Data  |                                       |                         |        |
    | Code         |                                       | PC(T1) , PC(T2), PC(T3) |        |

#### Kernel-Level Thread
* Thread In kernel Level
  
  * aka Kernel-Level Thread / Lightweight Process
  
  * Kernel-Level threads are managed and constructured by OS 
    * OS separate the execution state of process into a thread abstraction
      * make concurrency cheaper
        * Less state to allocate and initialize
    * OS manage thread + process
      * all thread op are implemented in the kernel
      * OS schedule all of the threads in the system
  * Abstraction Name Example
    * Linux : tasks
    * NTFS: threads
    * Solaris: lightweight processes
  
* Kernel Thread Limitation

  * suffer from too much overhead for a fine-grained concurrency
    * thread op still require sysCall
      * ideally want thread op to be as fast as a procedure call
    * kernel level threads have to be general
      * to support the needs of all programmers/language/runtimes
  * Solution
    * need even cheaper threads (User-Level Thread) for a fine-grained concurrency

#### User-Level Threads

* Characteristic
  * User-Level Threads are managed ==entirely== by the run-time system/user-level library
  * small and fast
    * simple structure
      * PC
      * registers
      * stacks
      * small TCB
    * no kernel involvement when creating a new thread
      * switching between thread
      * synchronizing threads are done via procedure call
    * up to 100x faster than kernel thread
      * notice that still depend on the quality of both implementation
* Limitation
  * Cause
    * invisible to the OS 
      * they are not will integrated with OS
  * Result
    * OS can make poor decision
      * scheduling a process with idle thread
      * block a process whose thread initated an I/O though the porcess has other threads that can execute
      * de-scheduling a process with a thread holding a lock
  * Solution
    * require communication between the kernel and the use-level thread manager

#### POSIX Thread

* Definition
  * Standarized C Language Threads Programming API
* aka **pthread**
* Specify interface ==( not implementation )==
  * Implementation may be kernel-level threads/ user-level threads/hybrid
* Linux pthreads implementation use kernel-level thread

#### Trade-Off

* Choice List

  * sequential VS parallel
  * pthreads VS processs (single thread)

* Bottomline In General

  * disadvantage of process
    * more heavy-weight than threads
    * have a higher startup/shutdown cost than threads

  * advantage of process
    * safer and more secure
      * each process has its own address space
  * disadvantage of thread
    * a thread crash $\to$ take down all other threads
    * a thread's buffer overrun $\to$ create security problem for all

* Choice depends on application type



### Synchronization

> CS == Critical Section
>
> HW == Hardware
>
> RS == Remainder Section
>
> T == Thread
>
> pre-emptive [先发制人]
>
> anticipate [优先行动]
>
> interleaving [交错]
>
> exclusion [拒绝]
>
> postponed [延期]
>
> indefinitely [无限期]
>
> granted [被允许]
>
> atomic [原子性]

#### Scheduling Precondition

* Multiple threads/process ready to run
* Mechanism for switching between threads/process
  * context switch
* Pre-emptive policy for choosing the next process to run
  * thread/process can't anticipate when forced to yield the CPU
  * it is not easy to detect anticipatation and yielding since only timing changes

#### What Is Synchronization

* Cause

  * arbitrary interleaving of thread execution can have unexpected consequence
  * scheduling it invisible to the app

* Definition

  * the way to restrict the possible interleavings of execution
  * the mechanism that give us the scheduling control

* Role

  1. Enforce single use of a shared resource

     * aka Critical Section Problem

     * example

       * use a lock so that only one thread can print output to console at a time

       * ```c
         printf("hello"); //T1
         printf("Goodbye"); //T2
         // possible output:
         // "helloGoodbye" or "GoodbyeHello"
         // all other mixture like "heGooldbloye" should be impossible
         ```

  2. Control oreder of thread execution

     * exemple
       * parent wait for child to finish
       * ensure menu prints prompt after all output from threadrunning program

* BadCase Example

  * bank account withdrawal + deposit without synchronization

    * Precondition

      * suppose initial shared account balance = \$1000
      * withdrawal (W) \$100 + deposit (D) \$100 at the same time

    * Interleaved Schedule Condition

      | last action | ```putBalence(W.balence)```                                  | ```putBalence(D.balence)```                                  |
      | ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
      | Schedule    | ```W.balence = getBalence();    //P1   W.balence -= 100;            //P1 //CONTEXT SWITCH                getBalence(D.balence);       //P2     //CONTEXT SWITCH                  D.balence += 100;            //P2 putBalence(D.balence);       //P2             //CONTEXT SWITCH putBalence(W.balence);       //P1``` | ```W.balence = getBalence();   //P1        W.balence -= 100;           //P1           //CONTEXT SWITCH      getBalence(D.balence);      //P2           D.balence += 100;           //P2         //CONTEXT SWITCH putBalence(W.balence);      //P1             //CONTEXT SWITCH putBalence(D.balence)       //P2``` |
      | Balence     | 900                                                          | 1100                                                         |

  * Cause: Race Condition

    * two concurrent threads manipulated a **shared resource** (account) without any synchronization
    * outcome depends on the order of the accesss take place
    * notice that race condition can occur on  a uniprocessor

  * Solution

    * should ensure that only one thread at a time can manipulate the **shared resource** (from begin to end)
      * so that we can reason about ptogram behaviour 
      * synchronization

#### Critical Section

* Annotaion
  * $T $ = A set of $n$ threads: $T_0, T_1, \cdots, T_n$
  * $\R$ = A set of resources shared between threads $T$
* Definition
  * A segment of code which access $\R$ 
  * aka CS
* Requirement
  * Mutual Exclusion
    * only one thread at a atime can execute in the CS
      * if one thread is in the CS then no other is
    * all other threads are forced to wait on entry
    * when a thread leaves the CS another can enter
  * Progress
    * if no thread in CS and some threads wnat to enter CS
      * only **not-in-RS** threads can decide the choice of next enter thread
        * note that remainder section ```== !CS```
      * choice cannot be postponed indefinitely
  * Bounded Waiting / No Starvation
    * if  $T_i$ is waiting on the CS and before $T_i$ is granted access
      * there is a limit on the number of **times/frequency about other threads enter CS **
* Performance
  * the overhead of entering / exiting the CS is small
    * depend on the work being done within it

#### CS 2-Thread Solution

* Assumption

  * no HW support/special HW instruction
  * no restriction on the #processors
  * basic machine language instructions (like LOAD, STORE) are atomic
    * on modern architecture this assumption false
    * if two such instructions are executed concurrently
      * result == their sequential executions in some unknown order

*  Annotation

  * $T = \{T_0, T_1\}$
  * use $T_i$ and $T_j$ when $j=i-1$ if the exact number doesn't matter

* Try 1: used one turn var (Not enough)

  * precondition

    * shared var inital ```int turn```

    * if ```turn = i``` $\Rightarrow$ $T_i$ is granted into its CS

  * scheduling code
  
    * ```c
      void My_work(id_t id){ // id can be 0 or 1
        // ...
        while(turn != id); // entry section
        // CS
        turn = 1 - id; // exit section
        // RS          
      }
      ```
    
  * check for CS requirement
    
    - [x] mutual exclusion 
        * |               | T0 ```(id = i)```                                            | T1 ```(id = 1-i)```                                          |
        | ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
      | init turn = i | turn == id                                                                         $\Rightarrow$ ```while(turn != id)``` statement ```false```                           $\Rightarrow$  pass entry section                                                                   $\Rightarrow$ now in CS | turn != id                                                              $\Rightarrow$ ```while(turn != id)```statement ```True```                                  $\Rightarrow$ Infinite iteration in ```  ;```                                      $\Rightarrow$ wait in entry section  until turn = 1 - i |
    - [ ] progress
      *  
      
        |              | T0 ```(id = i)```                                            | T1 ```(id = 1-i)```                                          |
        | ------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
        | $T_i$  in CS | pass CS  $\Rightarrow$ turn = 1 - id    $\Rightarrow$  pass exit section              $\Rightarrow$ now in RS | T1 may start enter after T0 in RS then may have chance influenced by RS of T0 (like T0 change turn value in T0 RS then T1 may never execute in to CS) |
      
        
      
      * Require strict alternation of threads in CS
        
        * if ```turn = i``` $T_j$ may not enter **imidiately** even if $T_i$ in RS (no notification to $T_j$ )
    
  * Problem Analysis
  
    * first attempt doesn't have enough info/ indicate about state of each process
    * it only remeber which process is allowed to enter its CS
  
* Try 2: used one flag var pair (Not enough)

  * precondition
    * shared var inital ```boolean flag[2] = {false, false}```
    * $T_i$ read ```flag[1-i]``` when entry
    * $T_i$ update ```flag[i]``` 
    * if ```flag[i]``` $\Rightarrow$ $T_i$ is granted into its CS
  
  * scheduling code
  
    * ```c
      void My_work(id_t id){ // id can be 0 or 1
        // ...
        while(flag[1-id]); // entry section
        flag[id] = true; // indicate entering CS
        // CS
        flag[id] = false; // indicate exit CS and pass exit section
        // RS          
      }
      ```
  * check for CS requirement
    
    - [x]  progress
        * |              | T0 ```(id = i)```                                            | T1 ```(id = 1-i)```                                          |
        | ------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
      | $T_i$  in CS | flag[id] = True                                                                         $\Rightarrow$ now in CS                                     $\Rightarrow$ flag[id] = false                                     $\Rightarrow$  pass exit section                                                                   $\Rightarrow$ now in RS | After T0 enter                                                             $\Rightarrow$ flag[i] = True                                                            $\Rightarrow$ ```while(flag[i-id])```statement ```True```                                  $\Rightarrow$ Infinite iteration in ```  ;```                                             $\Rightarrow$ wait in entry section  until flag[i] = False   $\Rightarrow$ Before T0 in RS |
    - [ ] mutual exclusion
      *  
      
        |                   | T0 ```(id = i)```                                            | T1 ```(id = 1-i)```                                          |
        | ----------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
        | ```flag {f, f}``` | flag[1-i] = false                                                                       $\Rightarrow$ ```while(flag[1-id])``` statement ```false```                           $\Rightarrow$  pass entry section                                                                   $\Rightarrow$ now in CS | flag[i] = false                                                                       $\Rightarrow$ ```while(flag[1-id])``` statement ```false```                           $\Rightarrow$  pass entry section                                                                   $\Rightarrow$ now in CS |
      
        
      
      * 2 threads will set their own flag to true and enter in CS
        
        * can't fix this by changing order of testing and setting flag var /lead to deadlock
        
        * | T0 ```(id = i)```                  | T1 ```(id = 1-i)```                  |
          | ---------------------------------- | ------------------------------------ |
          | ```while(flag[1-i]); //false```    |                                      |
          |                                    | ```while(flag[i]); //false```        |
          | ```flag[i] = true; // now in CS``` |                                      |
          |                                    | ```flag[1-i] = true; // now in CS``` |
          | ```flag[i] = false;```             |                                      |
          |                                    | ```flag[1-i] = false;```             |
        
      
    - [x] Starvation
  
* Try 3: combine try1 and try2 var ( Correct )

  * Precondition 

    * the threads share the var ```turn``` and ```flag[]```

  * Scheduling Algorithm Idea

    1. set $T_i$  own flag ```flag[i] = true``` and ```int turn = i```
       * notice that now flag is indicating interest to enter CS not the actual sign of entering
    2. spin waiting  ```while(turn = i && flag[1-i]);```
    3. if both threads try to enter their CS at the same time 
       * ```turn``` will be set to both ```i``` and ```1-i``` at the same time 
       * but ONLY ONE of this assign will be the last
       * the final value of turn decides which of the two threads allowed to enter CS first
    * Basis of Dekker's Algorithm and Peterson's Algorithm

#### Peterson's Algorithm

* Scheduling Code

  * ```c
    //shared var
    int turn = 0; 
    int flag[2] = {false, false}; 
    
    
    void My_work(id_t id){ // id can be 0 or 1
      // ...
      flag[id] = true; // indicate interesting of entering CS
      turn = id;
      while(turn == id && flag[1-id] ); // entry section
      // CS
      flag[id] = false; // indicate exit CS and pass exit section
      // RS          
    }
    ```

* Performance Analysis

  * |                                  | T0 ```(id = i)```                                            | T1 ```(id = 1-i)```                                          |
    | -------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
    |                                  | ```flag[i] = true;```                                        |                                                              |
    |                                  |                                                              | ```flag[1-i] = true;```                                      |
    |                                  |                                                              | ```turn = 1-i; ```                                           |
    | only final value of turn matters | ```turn = i; ```                                             |                                                              |
    |                                  | ```while(i == id && flag[1-i] ); // true --T0 spin waiting``` | ```while(i == id && flag[i] ); // false --T1 enter CS```     |
    |                                  |                                                              | ```flag[1-i] = false; // indicate T1 exit CS and pass exit section``` |
    | before T1 in RS T0 will enter CS | ```while(i == id && flag[1-i] ); // false --T0 enter CS as soon as T1 exit``` |                                                              |

* can be extended to **multiple threads**

#### Another Approach To Multi-Thread Solution

* Lamport's Bakery Algorithm
  * Upon entering 
    * each T gets a number
    * the lowest-num T enter CS next
  * No guarantee that 2 threads do not get same num
    * if case of a tie
      * T with the lowest id is served first
  * T id is unique and totally ordered



### Synchronization Hardware

> CS == Critical Section
>
> TAS = Test And Set
>
> inversion [倒置]
>
> 

#### Disabling Interrupts

* On a uniprocessor OS
  * disable interrupts before entering CS
* Pro
  * useful for short CS in OS
* Limitation
  * not sufficient on a multiprocessor
  * need some atomic instruction

#### Test And Set

* Semantics

  1. record the old value of the var
  2. set the var to some non-zero (new) value
  3. return the old value

* Flavour
  * HW executes this atomically
  * Can be used to implement simple lock variables
* Definition

  1. ```c
     bool TAS(bool *lock){
       bool old = *lock;
       *lock = true;
       return old;
     }
     ```

  2. ```c
     bool TAS(bool * lock){
       if(*lock ==false){
         *lock = true;
         return false;
       }else
         return true;
     }
     ```
  
  * Lock is always ```true``` on exit from TAS
    * either it was ```true``` (i.e. locked) already  $\Rightarrow$ nothing changed
    * it was ```false``` (i.e. available) $\Rightarrow$ the caller now hold it
  * Return value
    * ```true``` $\gets$ it was locked already
    * ```false``` $\gets$ it was previously available

#### Lock

* Spin Lock

  * operation

    * ```acquire()```

      * ```c
        bool lock;
        
        void acquire(bool *lock){
          while(TAS(lock)); // when false we know that we've acquired it (has set value)
        }
        ```

    * ```release()```

      * ```c
        bool lock;
        
        void release(bool *lock){
          *lock = false; // to release simply set to false
        }
        ```

  * Pro

    * with hardware support we create primitive locks, make it easier and more efficient to sychron

  * Limitation

    * Spinlocks are built on machine instructions
      * Uses Busy Waiting Problem
        * thread continually executes the ```while()``` loop in ```acquire()``` 
        * consuming CPY cycles
      * Starvation Is Possible
        * when a thread leave its CS, the next one to enter depends on scheduling
        * a waiting thread could be denied entry indefinitely by scheduling
      * Deadlock is possible through **priority inversion**

* Sleep lock

  * When waiting to acquire a lock
    * $\Rightarrow$ put T into a blocked state
  * Requirement
    * a queue for waiting threads
  * Linux implementation
    * wait_queue
    * operation
      * ```wait_event(wait_queue_head_t *queue, bool condition)```
      * ```wake_up(wait_queue_head_t *queue)```

### High-Level Abstraction Of Sychronization

> ADT == Abstract Data Type
>
> Semaphore [信号量]
>
> Rendezvous [约会]

#### Type Of Abstraction

* |                    | Pro                                              | Con                                    |
| ------------------ | ------------------------------------------------ | -------------------------------------- |
| Lock               | minimal semantics                                | very primitive                         |
| Semaphore          | basic and ez to understand                       | not always the right abstraction       |
| Condition Variable | stronger semantics and ez for diverse conditions |                                        |
| Monitor            | high-level                                       | ideally need language support ( Java ) |

#### Semaphore

* Definition
  
  * ADT for synchronization 
  
* Structure
  * an integer counting variable
    * only accessed by 2 atomic operations
    * represent the number of threads that can pass through ```wait()``` before it blocks
  
* Requirement

  * a queue of waiting threads
  * atomicity of ```wait()``` and ```signal()```
    * must ensure two threads cannot execute wait() and signal() at the same time
    * otherwise will cause the CS problem
    * solution
      * use lower-level primitves to implement wait and signal
        * for uniprocessor
          * disable interrupts / hardware instruction
        * for multiprocessor
          * hardware instruction

* Operation
  * ```wait()```
    
    * must atomic
    
    * aka P / Decrement
    
    * basic semantics
      
      * block while semaphore $<$ 0 
      * if not blocked $\Rightarrow$ decrement
      
    * code definition
    
      * ```c
        void wait(int* semaphore){
          while(*semaphore <= 0) ;
          *semaphore --;
        }
        ```
    
  * ```signal()```
  
    * must atomic
  
    * aka V / Increment
  
    * basic semantics
  
      * increment the variable to unblock a withing thread if there are any
  
    * code definition
  
      * ```c
        void signal(int* sem){
          *sem++;
        }
        ```
  
* POSIX/Pthread Semaphore API

  * ```c
    #include <semaphore.h>
    
    sem_t mysem;
    sem_init(&mysem, 0, VAULE); // init the semaphore count (will influence the type)
    sem_wait(&mysem); // s.wait or P()
    sem_post(&mysem);// s.signal or V()
    ```

  * 

* Type

  * |                                    | Mutex / Binary            | Counting                                                     |
    | ---------------------------------- | ------------------------- | ------------------------------------------------------------ |
    | #access to resource                | single                    | resource has many unites available                           |
    | max #threads to pass the semaphore | sem's initial value/count | sem's initial value/count                                    |
    | CS entering condition              | mutual exclusion to CS    | resource allows certain kinds of unsynchronized concurrent access ( like reading ) |
    | intial value() i.e. count)         | 0/1                       | N                                                            |

* Example of Using Mutex Sem (accounting ex)

  * method initializing structure

    * ```c
      \\ inital value of s = 1
      typedef struct account{
        double balence;
        semaphore s;
      }account_t;
      
      double withdraw(account_t * acct, amt){
        double bal;
        wait(acct->s);
        bal = acct->balence;
        bal = bal - amt;
        acct->balence = bal;
        signal(acct->s);
        return bal;
      }
      ```

  * 3 threads runing scheduling example

    * | T0                                                           | T1                                                           | T2                                                           |
      | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
      | ```double bal;                              wait(acct->s);        //acct->s = 1, pass wait()     then acct->s--``` | ```double bal;    ```                       wait() execution slower than T0 ```wait(acct->s);            // acct->s = 0    waitspining``` | ```double bal;    ```                       wait() execution slower than T0 ```wait(acct->s);            // acct->s = 0    waitspining``` |
      | IF T0 enter wait first, acct->s will change from 1 to 0, only T0 will pass the wait() | ```wait(acct->s);            // acct->s = 0    waitspining``` | ```wait(acct->s);            // acct->s = 0    waitspining``` |
      | ```bal = acct->balence;                         bal = bal - amt;                             acct->balence = bal;``` | ```wait(acct->s);            // acct->s = 0    waitspining``` | ```wait(acct->s);            // acct->s = 0    waitspining``` |
      | ```signal(acct->s);           // acct->s now = 1        T0 over and let orher thread to have chance to pass``` | wait() and signal() are atomic cannot execute same time      | wait() and signal() are atomic cannot execute same time      |
      | T0 over                                                      | wait() execution slower than T2    ```wait(acct->s);            // acct->s = 0    waitspining``` | ```wait(acct->s);            // acct->s = 1, pass wait()     then acct->s--``` |
      | T0 over                                                      | ```wait(acct->s);            // acct->s = 0    waitspining``` | IF T2 enter wait now, acct->s will change from 1 to 0, only T2 will pass the wait() |
      | T0 over                                                      | ```wait(acct->s);            // acct->s = 0    waitspining``` | ```bal = acct->balence;                         bal = bal - amt;                             acct->balence = bal;``` |
      | T0 over                                                      | wait() and signal() are atomic cannot execute same time      | ```signal(acct->s);           // acct->s now = 1        T2 over and let orher thread to have chance to pass``` |
      | T0 over                                                      | ```wait(acct->s);            // acct->s = 1, pass wait()     then acct->s--``` | T2 over                                                      |
      | T0 over                                                      | IF T1 enter wait third, acct->s will change from 1 to 0, only T1 will pass the wait() | T2 over                                                      |
      | T0 over                                                      | ```bal = acct->balence;                         bal = bal - amt;                             acct->balence = bal;``` | T2 over                                                      |
      | T0 over                                                      | ```signal(acct->s);           // acct->s now = 1        T1 over and let orher thread to have chance to pass``` | T2 over                                                      |
      | T0 over                                                      | T1 over and return the final value of ```bal```              | T2 over                                                      |

  

#### Lock VS Semaphore

* Similarity
  * a mutex semaphore with init value 1 == lock
* Diff
  * semantic difference 
    * a lock has an owner and can only be released by its owner
  * sem permit error checking
  * sem help reason about the correct behaviour 

#### Rendezvous Problem With Semaphore

* Problem Requirement

  * 2 process A B 
  * each process has 2 part
    * a1 + a2
    * b1 + b2
  * want
    * a1 $\to$ b2
    * b1 $\to$ a2

* Solution

  * num of semaphore

    * 2
    * let them be semA and semB

  * intial value

    * notcie that inital value cannot be nagetive and should end up with the it start with
    * also should use same num of ```wait()``` as ```signal()```
    * semA, sem B = 0, 0

  * code example

    * | A                  | B                  |
      | ------------------ | ------------------ |
      | a1                 | b1                 |
      | ```signal(semB)``` | ```singal(semA)``` |
      | ```wait(semA)```   | ```wait(semB)```   |
      | a2                 | b2                 |

#### Semaphore Limitation

* can be hard to reason about synchronization 
* reason for waiting is embedded in the wait() operation
* sometimes you want a more complex wait condition



#### Condition Variable

* Definition

  * ADT that encapsulates pattern of
    * release mutex
    * sleep
    * re-acquire mutex

* Requirement

  * a queue of waiting threads (cv)
  * atomicity of ```cv_wait()```, ```cv_signal()```, ```cv_broadcast()```

* Operation

  * ```cv_wait()```

    * must atomic

    * basic semantics

      * **release lock**
      * sleep lock
      * re-acquire lock when signal/broadcast
      * return

    * code definition

      * ```c
        cv_wait(struct cv *condition_var, struct lock *lock);
        ```
    
  * ```cv_signal()```
    
* must atomic
    
* basic semantics
    
      * wake ONE enqueued thread
      
    * code definition
    
      * ```c
        cv_signal(struct cv *condition_var, struct lock *lock);
        ```
    
  * ```cv_broadcast()```
  
  * must atomic
  
  * basic semantics
    
      * wake ALL enqueued thread
      
    * code definition
    
      * ```c
        cv_broadcast(struct cv *condition_var, struct lock *lock);
        ```
    

* Always use cv with locks

  * reason

    * the lock 
      1. protects the shared data that is modified 
      2.  tested when to decide op like wait/signal/broadcast

  * general usage

    * ```c
      lock_acquire(lock); 
      while(wait_statment)
        cv_wait(condition_var, lock); //enter
      // CS
      cv_signal(condition_var);
      lock_release(lock);
      ```

* ```cv_signal()``` VS ```cv_broadcast()```
  * signal
    * OS scheduler pick one thread to wakeup
  * broadcast
    * all thread wake up and wait but ==no longer in CV queue== 
    * only one of them re-acquire the mutex and thus go through
    * ==may not execute== the exclude condition that made the threads block on wait()

#### Locks + Condition Varible

* convenient way to 
  * provide mutual exclusion for CS
  * blcok a thread that has to wait for something to happen
* some modern language provide built-in support for automatic locking for objects
  * Java synchronized method
  * Monitor (ADT for synchronization )

#### Sychron Reader/Writer Problem

* Condition Vatriable Solution

  * ```c
    int reader_num = 0;
    bool writing = false;
    int i;
    
    pthread_cond_t cv_r_or_w = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
    
    int main(){
        pthread_t reader_thread[MULTI];
        while(i<MULTI){
          pthread_create(&(reader_thread[i]), null, reader, null);
          i++;
        }
        pthread_t writer_thread;
        pthread_create(&writer_thread, null, writer, null);
    }
    void * writer(){ // can only have writer and writing no reader
        pthread_mutex_lock(&mutex_lock);
        while(reader_num!=0 || writing)
            pthread_cond_wait(&cv_r_or_w, &mutex_lock);
        writing = true;
        pthread_mutex_unlock(&mutex_lock); //must here unlock?
        write();
        pthread_mutex_lock(&mutex_lock); //must here lock?
        writing = false;
        pthread_cond_broadcast(&cv_r_or_w, &mutex_lock);//also can use signal()
        pthread_mutex_unlock(&mutex_lock); 
    }
    
    
    
    void * reader(){ //cannot modify data but multiple readers
        pthread_mutex_lock(&mutex_lock);
        while(writing)
            pthread_cond_wait(&cv_r_or_w, &mutex_lock);
        reader_num++;
        pthread_mutex_unlock(&mutex_lock); //must here unlock?
        read();
        pthread_mutex_lock(&mutex_lock); //must here lock?
        reader_num--;
        pthread_cond_signal(&cv_r_or_w, &mutex_lock);
        pthread_mutex_unlock(&mutex_lock);
    }
    
    ```

* Semaphore Solution

  * ```c
    int reader_num = 0;
    bool writing = false;
    int sem_w_or_r_init = 1;
    int sem_mutex_init = 1;
    
    pthread_sem_t sem_w_or_r; // sem for writer vs read sync
    pthread_sem_t sem_mutex; // sem for multi reader not into CS at the same time
    int main(){
        int i;
        pthread_sem_init(sem_w_or_r, 0, sem_w_or_r_init);
        pthread_sem_init(sem_mutex, 0, sem_mutex_init);
        pthread_t reader_thread[MULTI];
        while(i<MULTI){
          pthread_create(&(reader_thread[i]), null, reader, null);
          i++;
        }
        pthread_t writer_thread;
        pthread_create(&writer_thread, null, writer, null);
    }
    void * writer(){ // can only have writer and writing no reader
        P(sem_w_or_r);
        write();
        V(sem_w_or_r);
    }
    
    
    
    void * reader(){ //cannot modify data but multiple readers
        P(mutex);
        read_num++;
      
        if(read_num==1)  // the last reader should consider if writing
          P(sem_w_or_r);
        V(mutex);
      
      
        read();
      
      
        P(mutex);
        reader_num--;
        if(read_num==0)   //no reader so signal writer can writing
            V(sem_w_or_r);
        V(mutex);
    }
    ```

#### Sychron Producer/Consumer Problem

* Condition Variable Solution

  * ```c
    pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
    pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
    
    int trunk[SIZE];  
    int size = 0;  
    int front,rear=0;  /* queue for truck */
    
    int main()  {
        pthread_t producer_thread; 
        pthread_t consumer_thread; 
        void *producer();
        void *consumer();
    
        pthread_create(&consumer_thread,NULL,consumer,NULL);
        pthread_create(&producer_thread,NULL,producer,NULL);
        pthread_join(consumer_thread,NULL);
    
        return 0;
    }
    
    void add(int i) {
        trunk[rear] = i;
        rear = (rear+1) % SIZE;
        size++;
    }
    
    int get() {
        int v;
        v = trunk[front];
        front= (front+1) % SIZE;
        size--;
        return v ;
    }
    
    void *producer() {
        int i = 0;
        while (1) {
            pthread_mutex_lock(&mutex_lock);
            if (size == SIZE) {
                pthread_cond_wait(&not_full,&mutex_lock);
            }
            add(i);
            pthread_cond_signal(&not_empty);
            pthread_mutex_unlock(&mutex_lock);
            i++;
        }
        pthread_exit(NULL);
    }
    
    
    void *consumer() {
        int i,take_away;
        for (i=0;i< MULTI ;i++) {
            pthread_mutex_lock(&mutex_lock);
            if (size == 0) {
                pthread_cond_wait(&not_empty,&mutex_lock); 
            }
            take_away = get();
            pthread_cond_signal(&not_full);
            pthread_mutex_unlock(&mutex_lock);
        }
        pthread_exit(NULL);
    }
    
    
    ```

* Semphore Solution

  * ```c
    int sem_mutex_init = 1;
    int sem_not_full_init = 1;
    int sem_nor_empty_init = 1;
    
    pthread_sem_t sem_not_full; // sem for producer waiting since full
    pthread_sem_t sem_not_empty; // sem for consumer waiting since empty
    pthread_sem_t sem_mutex; // sem for multi worker not into CS at the same time
    
    int trunk[SIZE];  
    int size = 0;  
    int front,rear=0;  /* queue for truck */
    
    int main()  {
        pthread_t producer_thread; 
        pthread_t consumer_thread; 
      
        pthread_sem_init(sem_not_full, 0, sem_not_full_init);
        pthread_sem_init(sem_mutex, 0, sem_mutex_init);
        pthread_sem_init(sem_not_empty, 0, sem_not_empty_init);
    
        void *producer();
        void *consumer();
      
    
        pthread_create(&consumer_thread,NULL,consumer,NULL);
        pthread_create(&producer_thread,NULL,producer,NULL);
        pthread_join(consumer_thread,NULL);
    
        return 0;
    }
    
    void add(int i) {
        trunk[rear] = i;
        rear = (rear+1) % SIZE;
        size++;
    }
    
    int get() {
        int v;
        v = trunk[front];
        front= (front+1) % SIZE;
        size--;
        return v ;
    }
    
    void *producer() {
        int i = 0;
        while (1) {
            P(mutex);
            if (size == SIZE)
                P(sem_not_full);
            add(i);
            V(sem_not_empty);
            V(mutex);
            i++;
        }
        pthread_exit(NULL);
    }
    
    
    void *consumer() {
        int i,take_away;
        for (i=0;i< MULTI ;i++) {
            P(mutex);
            if (size == 0)
                P(sem_not_empty); 
            take_away = get();
            V(sem_not_full);
            V(mutex);
        }
        pthread_exit(NULL);
    }
    ```

  * 