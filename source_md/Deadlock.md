## Deadlock

***

[TOC]

***

### What Is Deadlock

> OS == Operating System
>
> HW == HardWare
>
> DD == Disk Drive
>
> CS == Critical Section

#### Deadlock In Non-OS World

* Train Deadlock Problem
  * 2 trains stop and neither shall start upon agin until the other has gone
* 狭路相逢没人走/没路走
* No Trafic Light

#### Resource Deadlock

* ==Root Cause==
  * Resources are **finite**
  *  Resource is unavailable \&\&  **the processes need it** $\Rightarrow$ processes wait  
  * Resource may be held **by other waiting processes**
* Resource
  * any object that might be needed by a process to do its work
  * main type
    * HW
      * printer
      * mem
      * processor
      * DD
    * Data
      * shared var
      * recode in db
      * file
    * Synchroniazation Objects + CS (only resource that can cause deadlock)
      * Lock
      * Semaphore
      * Moniter
* Reusable Resource
  * can be used by one process at a time then released

### When Does Deadlock Occur

#### Necessary Condition For Deadlock

* IF Deadlock Occur $\Rightarrow$
  1. Mutal Exclusion
     * only one process may use a resource at a time
  2. Hold And Wait
     * a process may hold allocated resources while awaiting assignment of others ( i.e. haven't release )
  3. No Preemption
     * No resource can be forcibly removed from a process holding it ( i.e. no release enforcment )

#### Necessary \&\& Sufficient Condition For Deadlock

* IF 1. \&\& 2. \&\& 3. \&\& 4. Occur $\iff$  Deadlock

  1. \&\&  2.  \&\&  3. are said above (necessary condition for deadlock)

  4. Circular Wait
     * $\exist$ a closed chain of processes S.T.
       1. each process holds $\ge$ 1 resource itself
       2. such resource is needed by the next process in the chain

* Notice that 4. Circular Wait $\Rightarrow$ 2. Hold And Wait 

  * Circular Wait result from a sequence of event
  * Hold And Wait is a policy decision

* Example: Dining Philosophers



### How to Deal With Deadlock

>concurrency [一致]

#### Deadlock Prevention

* Idea

  * Break 1 of 4 N\&\&S Condition and deadlock cannot occur

* Option 1: Break Hold + Wait

  * **Problem**

    * may wait a long time for all resources to be available at the same time
    * must acquire all locks at the start other than when they are really needed
      * $\Rightarrow$ **limit concurrency**
    * some longer processs may hold locks for a long time before they end up using them 
      * $\Rightarrow$ blocking other processes 
    * may not know all resource requirement in advance

  * Alternative: ```try_lock()```

    * lock available? grab a lock : try later

    * code example

      * ``` c
        ready = false;
        while(! ready){
          lock(Li);
          if (tryLock(L2) == -1){
            unlock(L1);
            ready = false;
          }else
            ready = false;
        }
        ```

    * Problem : **Livelock**

      * 狭路相逢等对方通过 = livelock
      * 狭路相撞没路走 = deadlock

* Option 2: Break No-Preemption

  * Idea
    * take a resource away from another thread
    * a thread preempt a thread in CS so that it can enter the CS
  * Problem
    * not feasible
    * too complex to achieve

* Option 3: Preventing Circular Wait

  * Idea
    1. Assign a Linear Ordering ( list R ) to resource types
    2. Requirement
       * IF a process P holding one type of resource $R_0$ in R 
         * $\Rightarrow$ P can ONLY request resources that follow  $R_0$ as the oder in R 
  * Problem
    * hard to come up with total order when there are lots of resource types
  * Solution
    * Partial Order
      * groups of locks with internal ordering

#### Deadlock Avoidance

* use knowledge about the resource that each process might request 
* to avoid moving into a state that might deadlock

#### Deadlock Detection

* use graph algorithms to identify deadlock

#### Deadlock Recovery

* tricky
* selective kiil process

#### OS Approach To Resolve Deadlock

* Ostrich Algorithm
  * Ignore the problem and hope it doesn't happen often
* Reason
  * no single strategy for dealing with deadlock is appropriate for all resources in all situation
  * ==all strategies are costly in terms of==
    * computation overhead
    * restricting use of resources
* If a lock guarantee progress $\Rightarrow$ Break Circular Wait $\Rightarrow$ dead-lock free
* Modern OS virtualize physical resources $\Rightarrow$ resources can be infinite
* finite resources are protected by synchronization objects

