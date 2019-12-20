## Virtual Memory

***

[TOC]

***

### What Is Virtual Memory

> MMU == Memory Management Unit
>
> SP == Stack Pointer
>
> PC == Program Counter

#### Program Memory Abstraction

* Definition

  * Virual/Logical Address Space

* Generator

  * CPU generates logical/virtual address when program executes

* Requirment

  * Memory system must see physical address
    * **Need Translation done by MMU**

* Layout

  * | 0xFFFF FFFF    |                                  |                   |
    | -------------- | :------------------------------: | ----------------- |
    | System Address |          OS Code + Data          |                   |
    | 0x8000 0000    |                                  |                   |
    | 0X7FFF FFFF    |                                  |                   |
    |                |              Stack               | &leftarrow; SP    |
    |                |                                  | &leftarrow; Break |
    | User Address   | Heap / Dynamic Memory Allocation |                   |
    |                |       Static Data Segment        |                   |
    |                |       Code / Text Segment        | &leftarrow; PC    |
    | 0x0000 0000    |                                  |                   |

* Recall Memory Hirachy

  * Larger + Cheaper + Slower
    * Register &rightarrow; CPU ( within LV1 Cache) &rightarrow; LV2/LV3 Cache &rightarrow; Main Memory / DRAM &rightarrow; Disk

#### Goal

* Transparency
  * a program should not need to know how memory is virtualized
  * each program has the illusion of using all of physical memory
* Efficiency
  * Time
    * program shouldn't run much more slowly
  * Space
    * minimize the size of structure
* Protection
  * Isolation
    * a process should not be able to access another processes memory



#### Requirement

* Relocation
  * programmers don't know what physical men will be available
  * may swap processes in/out of memory so need to bring back to different region of mem
  * need address translation
* Protection
  * a process's mem should be protected from unwanted access by other process
  * require hardware support
* Sharing
  * process need to be able to access the same memory
  * need ways to specify and control whar sharing is allowed
* Logical Organization
  * Machine accesses/addresses memory as a 1D arrat of bytes
  * programmer organizae codes in modules
  * need to map between these views
* Physical organization
  * Mem and Disk form a 2-level hierachey
  * CPU can only access data in register or memory not disk

#### Policy Decision

* ==mechanism== Precondition
  * PT
  * MMU
  * TLB
* ==Policy== Precondition
  * Fetch Policy
    * when to fetch a page
    * Demand paging VS Prepaging
  * Placement Policy
    * where to put the page
    * Mem / Disk
    
      * Memory management HW can translate any virtual-to-physical mapping equally well
      * always prefer mapping
        * local memory is faster 
      * Except: cache performance 
        * choose physcical pages to minimize cache conflicts 
  * Replacement Policy
    
    * what page to evict to make room

### From Physical To Virtual

> PTBR == Page Table Base Register
>
> coalesce [联合]

#### Partitioning

* Contiguous Allocation

* Idea

  * Fixed Equal-Slice Partition
    * internal fragmentation
    * overlay
    * limit the number of active process
  * Fixed Increased-Size-Slice Partition
    * overlay
  * Dynamic Partition
    * external fragmentation
    * need compaction
    * require relocatable
    * need to know maximum size of process at load time
  
* Placement Heuristics

  * First-fit

    * algorithm idea
      * choose the first block that is large enough
      * search can start at beginning or where previous search ended /Next-fit
    * **Pro**
    
      * Simplest and often fastest and most efficient 
    * Con
      * **internal fragmentation**
    * free space becomes fragmented more rapidly
      * since next-fit variant allocate from end of memory
    
  * Best-fit
  
      * algorithm idea
  
        * choose the block that is closest in size to the request
  
      * Pro
  
      * **Left-over fragment tend to be small**
      * Con
        * **similar storage utilization with FF**
  * Worst-fit
    * algorithm idea
      * choose the largest block
  * Con
    
    * Not good
    
  * Quick-fit
    * algorithm idea
      * keep multiple free lists for common block size
    * Pro
      * **great for fast allocation**
    * Con
      * **generally harder to coalesce**
  
* Improvment performance of the allocation algorithm

  * keep the hole list sorted
    * Pro
      * when blocks are freed we would like the ability to coalesce adjacent free blocks into single larger block
      * It is much easier if the free blocks list is sorted by address 
      * we just have to look at the preceding and following block on the list once we find the insertion point for the newly freed blocks
      * otherwise all free blocks have to be examined to determine if coalescing is possible or not

* Hardware for Relocation Requirement 
  * Basic Idea
    * add relative addr to the process starting/base addr to form a real addr
  * 2 reg
    * base
    * limit
    * when process assigned to CPU $\Rightarrow$ load base register with the starting addr
    * load the limit register with the last legal address of process
    * on load or store $\Rightarrow$ add base to addr and compare with the limit
    * if (addr < base || addr >= base +limit ) : trap

#### Address Binding

* Cause

  * Programs must be in memory to execute

    * Program binary is loaded into a process address space

    * Address in program must be translated to real address

* When to address bound

  * option 1: Complie Time
    * must know what memory the process will use during compilation
    * call absolute code since binary contains real address
    * no relocation is possible
    * still used in simple embedded system
    
  * option 2: Load Time
    * aka Static Relocation
    1. Compiler bind symbolic addr $\to$ logical-relocatable addr in compilation unit / source file
    2. Linker take source file and bind logical/relocatable addr $\to$ logical-absolute addr in executable
       * resolve reference to symbol dedinded in source file
    3. Loader bind logical-absolute addr $\to$ phisical addr when program is loaded into memory
    
    * program can be loaded to different address when they start but cannot relocated later
    
  * option 3 : execution time
  
    * aka dynamic relocation
    * executable object file a.out contain logical address for entire program
    * translated to a real physical address during execution
    * Fliexible but require special hardware
  
#### Paging

* Definition
  
  * Each process gets the illusion of its personal address space (0 $\to 2^{64}$-1)
  * Each virtual page can be mapped to any physical page
  * process co-exit in memory
  * Data is either in memory or on disk
    * we must bring data in memory as needed (aka demand paging)
    * page table keep the correspondence between a virtual page and a physical page
  
* Paging Process
  
  1. Partition (Computer) memory into equal and fixed-size chunks
     * aka page frame
     * like a small book shelf
  2. Divide the process memory into the same-size chunks
     * aka page
     * like huge amount of books
  3. Any page can be assigned to any free frame
     * ==no external fragmentation==
     * internal fragmentation is at most a part of a page
       * usually pagesize = 4K
     * like putting books into shelf
  
  * possible page frame size are restricted to the power of 2
  * use the **Page Table Data Structure** to store the mapping of virtual pages to physical page
    * like book look up system
  
* Page Address Translation
  
  * **Hardware MMU convert VA into PA using Page Table** 
  * Notice that OS process has their own Page Table
* Time complexity
  
  * processes reference pages in localized patterns
    * temporal locality
      * recently referenced will referenced again
    * spatical locality
      * near recently referenced locaitons will referenced soon 
    * paging is infrequent
      * processes usually exhibit both kinds of locality during their execution $\Rightarrow$ make paging possible
      * all caching strategies depends on locality to be effective
  
* Protection And Sharing
  
  * we need access rights for page like file permissions
    * it is kept in the page tables
  * OS data shouldn't be accessible to users
  * avoid leaked information from deallocated pages
  
* Support
  * OS Support
    * OS maintain the page table for each process
      * SWDS store in OS memory
      
      * Page Table records which physical frame holds each page
      
      * Virtual address = 
        
        | [ $log_2$(vaddr / page_size) ]$_{2\to10}$ bit# | [ $log_2$(page_size) ]$_{2\to10}$ bit# |
        | ---------------------------------------------- | -------------------------------------- |
        | page number                                    | page offset                            |
        
        
        * page number = vaddr / page_size = [ vaddr >> $log_2$(page_size) ]$_{2\to10}$ 
        * page offset = vaddr % page_size = vaddr \& 0x3FF 
        
      * Physical addr = frame number + page offset
        * paddr = page_table[ vaddr / page_size  ] + vaddr % page_size
        * page_table[   [ vaddr >> $log_2$(page_size) ]$_{2\to10}$   ]  << [ $log_2$(page_size) ]$_{2\to10}$  | (vaddr \& 0x3FF )
  * **Hardware Support**
    
    * PTBR
      * indicate where the page table starts
  
* Memory Reference Process
  
  1. processor translate the page number to its frame number By looking up the Page Table
  2. add the offset to generate a physical address 
  3. PTBR quickly locate the page table for currently running process



### Page Table

>PTE == Page Table Entry
>
>PFN == Page Frame Number
>
>PTD == Page Table Directory
>
>TLB == Translation Lookaside Buffer
>
>MMU == Memory Management Unit
>
>VPN == Virtual Page Number
>
>PTBR == Page Table Base Register
>
>exploit [利用]
>
>Pentium [奔腾]
>
>

#### Page Table Entry

* #bit: 32 ( 4 byte )

  * |  1   |  1   | 1    |  3   |        26        |
    | :--: | :--: | ---- | :--: | :--------------: |
    |  M   |  R   | V    | Prot | Page Fram Number |

* Annotation

  * M = Modify Bit
    *  page has been written?
    * set 1 when a write to a page occurs
  * R = Reference Bit
    * page has been accessed?
    * set 1 when a read/write to a page occurs
  * V = Valid Bit
    * page can be used?
    * checked on each use of vaddr
  * Protection Bits
    * specify whar operation are allowed on page
    * read/write/execute
  * PFN
    * determine physical page

* PageTable store in protected memory

* **Limitation**

  * Space
    * **mem require for page table can be large**
      * need one PTE per page
      * direct page table (increase page size ) will cause internal fragmentation 
      * increase page table (i.e. one per code, stack, heap) will cause external fragmentation 
    * Solution : 
      1. hierachical PT (multi-level page table )
      2. hashed PT
      3. Inverted PT
  * Time
    * **mem reference overhead time**
      * 2 mem reads/reference per address lookup
        1. read PT
        2. read actual mem
      * hierachical page table make it worse
        * one read per level of page table
    * Solution:
      * use a hardware cache of lookup
        * TLB

#### Multi-Level Page Table

* Nutshell Idea

  * split PT into Pages of PTE
  * use a PTD / master page table to point to different page of PTEs

* Two-Level Page Table

  * vaddr =

    * | Map to Secondary Page Table | Map to Physical Frame | Offset of Physical Frame |
      | --------------------------- | --------------------- | ------------------------ |
      | Master Page Number ( 10 )   | Secondary ( 10 )      | Offset ( 12 )            |

  * #bit of offset = $log_2$(page_size)

  * #bit of (MPN + MPF) =  $log_2$(#Virtual Page)

  * #bit of MPF =  $log_2$(#Physical Page Frame / #PTE)

  * #bit of MPN =   $[ log_2$(#Virtual Page) $]^{\frac{1}{\#PT\_LEVEL}}$ 

  * Remember To Get Rid Of Leading Zero

  * Notice That Mem Dump 2 Level Are In Same Page

    * MD[MPN] $\to$ MD [ MD[MPN] ] $\to$ **PTE**

  * Common Number

    * 32-bit vaddr space
    * 4K pages
    * 4 byte / PTE
    * offset bit#: 12 (leave 20 bit)
    * each page table occupy 1 page frame
    * 4K / 4 byte = 1K PTE  per page table frame
      * Master 1K = 10 bits
      * Secondary 1K = 10 bits

* Trade-off

  * Save space
  * **Add more level of indirection when translating address**
  * Linux use 3-level page tables

* 64-bit Address Space

  * only using 48 bits
  * 9 bits | 9 bits | 9 bits | 9 bits | 12 bits
  * assuming 8 byts for PTE
  * may be 4 level too much overhead

* Solution 1: Hashed Page Tables

  * VPN $\to$ Hashed-bucked $\to$ PTE $\to$ PFN

* Solution 2: Incerted Page Tables

  * keep only one page table for the whole system
  * need to record prcess id

#### TLB

* aka Translation Lookaside Buffer

* Definition

  * small and fully-associative hardware cache of recently used translation
  * part of the MMU
  * **translate VPN into PTE**
  * can be don in a single machine cycle

* TLB Characteristic

  * all entries looked up in parallel = fully associative **cache**
  * cache tag = virtual page num
  * cache value = PTE 
  * with PTE + offset can know physical address / secondary PT

* TLB exploit locality

  * process only use a handful of pages at a time

    * only need those pages to be mapped
    * 65 entry TLB map 64 pages (256K)
  * HIT RATE important
    * typically >99% of translation are hits
  * still need to deal with TLB miss/TLB fault

#### Manage TLB

* ==who to place the translation / load TLB==
  * Hardware-loaded / MMU
    * eg: Intel Pentium
    * know PTBR (where page tables are in main mem)
    * HW access the OS maintained table in memory
    * Table have to be in HW-defined format
      * inflexible
  * Software-loaded / OS
    * eg: MIPS R2000
    * TLB faults to the OS
    * OS find appropriate PTE and loads it in TLB
    * Must be fast (but still 20-200 cycles)
    * CPU ISA has instructions for manipulating TLB
      * add entry
      * read entry
      * invalidate entry
      * invalidate all entries / flush
    * tables can be any format convenient for OS
      * flexible
* Ensure Consistency
  * **OS ensure TLB and PR are consistent**
    * invalidate the entry of the catched copy of the PT in TLB when it change the protection bits of a PTE
* Reload TLB on a process context switch
  * invalidate all entries / flush the TLB
  * how to fix it ( **Scheduling** )
* **TLB Miss**
  * a new PTE has to be loaded
  * a cached PTE must be evivted
    * choosing PTE to evict == TLB Replacement Policy
    * **implemented in hardware for hardware loaded TLB**

#### Demand Paging

* Process Definition

  * pages will be moved between memory and disk

* Semantics

  * OS use main memory as a **page cache** of all the data allocated by processes in the system

    1. initially page are allocated from mem

    2. when mem fills up $\Rightarrow$ allocating page in mem requires some other page to be **evicted** from memory

    3. evicted pages go to disk ( i.e. the swap file)
  
* Flavour

  * the movement of pages between mem and disk is done by the OS 
  * transparent to app

* Dirty VS Clean Page

  * dirty page == modified page
  * only dirty page need to be swapped
  * should know where clean pages on disk to read them from agian ( no actual swapping )

* ==OS View / Perspective Performance==
  
  * evict page to disk $\Leftarrow$ mem if full
  
  * load page from disk $\Leftarrow$ reference again
  
  *  when reference to an evicted page $\Rightarrow$ 
    
    * cause TLB miss
    
    * PTE was invalid so cause Page Fault
    * **OS allocate a page frame**
    * OS read page from disk
    * When I/O complete
      * OS fill in PTE
      * mark it valid
      * restart the faulting process
  
* Process Perspective Performance
  
  * when a process FIRST START / Created
    *  **a new-created process contain**
      * a brand new page table with all valide bits off (0000000...)
      * **no pages in memory**
  * when a process starts executing
    * instructions fault on code and data pages
    * faulting stops when all necessary code+ data page are in mem
    * only code + data needed by a process will be loaded
    * then change over time
  
* **Cost**
  
  * Timing
    * **disk read is initated when the process needs the page**
  * Request size
    * **process can only trigger page fault on one page at a time**
    * disk see single page-sized read
  
* Alternative Policy
  
  * Prepaging 
    * aka prefetching
    * predict future page use at time of current fault
    * eg
      *  load A $\to$ fault and fetch A $\to$ load B $\to$ fault and fetch B ( without prepaging)
      * load A $\to$ fault and fetch A+B (with prepaging to predict that B will be loaded at next)
    * limitation
      * how to predict?
  
  

#### Page Fault

* Definition
  * miss in TLB and page is not in memory
* Cause
  * Page table entry indicates that the page is not in mem
    * a process access a page that has been evicted
* Action
  * OS is responsible for loading page from disk
  * process stops until the data is brought into memory
  * use the OS page replacement policy
* **Detail Of Access Evicted Page**
  1. OS evict a page
     * set the PTE invalid
     * store the location of the page in the swap file in the PTE
  2. A process access the page
     * the invalid PTE will cause a trap ( aka Page Fault )
  3. Trap will run the OS Page Fault Handler
  4. **Handler** uses the invalid PTE to locate page in swap file
     * Read page into a PF
     * update PTE to Point to it
     * reload TLB
  5. Now prcess resume the process
* Limitation
  * each time we evict somthing else we put the invalid PTE
    * **slow**
* Solution
  * OS Usually keeps a pool of free pages around **so that allocation do not always cause eviction**



### Page Replacement Algorithm

> TLB == Translation Lookaside Buffer
>
> FIFO == First In First Out
>
> LRU == Least Recently Used
>
> MRU == Most Recently Used
>
> Thrashing [毒打]

#### Page Replacement Policy

* Cause
  * when page fault occurs
    * OS loads the faulted page form disk inot a page frame of memory
  * There may be no free frames available for use
  * OS must replace a page form each page faulted in
    * evict a page (aka the victime) to free up a frame
    * if the victim has been modified since the last page-out
      * it mus be written back to disk on eviction ( dirty page)
* Role
  * determine how a victime is chosen
  * avoid thrashing
    * thrashing == overcommitted
      * spending more time on paging data back and forth from disk than executing user programs
      * no time spent doing useful work
      * could just be that there isn't enough physical mem for all of the processes in the system
    * Possible solution
      * swapping
        * write out all pages of a process and suspend it
        * buy more mem [微笑.jpg]
* Goal
  * to reduce the fault rate by slelcting the best victime page to remove
* Evaluation
  * a reference string by counting the number of page faults
    * Evicting the page that won't be used for the longest period of time minimize the number of page faults

#### Belady's Algorithm

* Idea
  * Replace the page that will not be used for the longest period of time
* Contribution
  * have the lowest fault rate for any page reference stream
  * use it as a yardstick
    * compare implemetation of page replacement algorithms with the optimal to gauge room for improve
    * random replacement is often the lower bound
* Problem
  * have to know the future perfectly
* Belady's Example
  * Cold Miss
    * First access to a page ( unavoidable )
  * Capacity Miss
    * caused by replacement due to limited size of memory
    * choose a victim to evict

#### FIFO

* Idea

  * maintain list of pages in the order that were paged in
  * on replacement evict the oldest one

* Contribution

  * obvious algorithm 
  * simple to implement

* Limitation 

  * the one brought in the longest ago  may or may not being used 
  * **suffers form Belady's anomaly**
    * the fault rate might actually increase when the algorithm is given more memory
    * eg 3 page frames $\to$ 4 page frames

* Example

  * reference string

    * 0 1 2 3 0 1 4 0 1 2 3 4 [ 5 unique num ]

  * 3 frame

    * | Youngest   | \0/  | \1/  | \2/  | \3/  | /\0/\ | /\1/\ | \4/  | 4    | 4    | /\2/\ | /\3/\ | 3    |
    | ---------- | ---- | ---- | ---- | ---- | ----- | ----- | ---- | ---- | ---- | ----- | ----- | ---- |
    |            |      | 0    | 1    | 2    | 3     | 0     | 1    | 1    | 1    | 4     | 2     | 2    |
    | **Oldest** |      |      | 0    | 1    | 2     | 3     | 0    | 0    | 0    | 1     | 4     | 4    |

    * 9 fault = 5 cold miss + 4 capacity miss

  * 4 frame
    * | Youngest   | \0/  | \1/  | \2/  | \3/  | 3    | 3    | \4/  | /\0/\ | /\1/\ | /\2/\ | /\3/\ | /\4/\ |
      | ---------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----- | ----- | ----- | ----- | ----- |
      |            |      | 0    | 1    | 2    | 2    | 2    | 3    | 4     | 0     | 1     | 2     | 3     |
      |            |      |      | 0    | 1    | 1    | 1    | 2    | 3     | 4     | 0     | 1     | 2     |
  | **Oldest** |      |      |      | 0    | 0    | 0    | 1    | 2     | 3     | 4     | 0     | 1     |
    
    * 10 fault = 5 cold miss + 5 capacity miss

#### LRU

* Idea
  * make a guess based upon past experience 
  * on replacement evict the page that has not been used for the longest time in the past
    * LRU is opposite of Belady
  
* Contribution
  
  * it is a stack algorithm so no Belady's anomaly
  
* Implementing LRU
  * Option 1: ==Time Stamp==
    * implementation
      1. time stamp every reference
      2. evict page with the oldest time stamp
    * problem
      * ==need PTE big enough to holde meaningful time stamp (space complexity)==
      * ==need to examine every page on eviction to find the oldest one (time complexity)==
  * Option 2: ==Stack==
    * implementation 
      1. keep pages in a stack
      2. on reference $\Rightarrow$ move the page to the top of the stack
      3. on eviction $\Rightarrow$ replace the page at bottom
    * problem
      * ==need costly software operation to manipulate stack on EVERY memory reference process==

* Example

  * reference string

    * 0 1 2 3 0 1 4 0 1 2 3 4 [ 5 unique num ]

  * 3 frame

    * | MRU     | \0/  | \1/  | \2/  | \3/  | /\0/\ | /\1/\ | \4/  | 0    | 1    | /\2/\ | /\3/\ | /\4/\ |
      | ------- | ---- | ---- | ---- | ---- | ----- | ----- | ---- | ---- | ---- | ----- | ----- | ----- |
      |         |      | 0    | 1    | 2    | 3     | 0     | 1    | 4    | 0    | 1     | 2     | 3     |
      | **LRU** |      |      | 0    | 1    | 2     | 3     | 0    | 1    | 4    | 0     | 1     | 2     |

    * 10 fault = 5 cold miss + 5 capacity miss

  * 4 frame
  
    * | MRU     | \0/  | \1/  | \2/  | \3/  | 0    | 1    | \4/  | 0    | 1    | /\2/\ | /\3/\ | /\4/\ |
      | ------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----- | ----- | ----- |
      |         |      | 0    | 1    | 2    | 3    | 0    | 1    | 4    | 0    | 1     | 2     | 3     |
      |         |      |      | 0    | 1    | 2    | 3    | 0    | 1    | 4    | 0     | 1     | 2     |
    | **LRU** |      |      |      | 0    | 1    | 2    | 3    | 3    | 3    | 4     | 0     | 1     |
  
  * 8 fault = 5 cold miss + 3 capacity miss
  
* Limitation

  * Exact LRU  is too costly to implement

* Solution 

  * LRU Approximation
    * use the PTE reference bit/bits
    * basic idea
      * initially all R bits are 0
      * as process execute $\Rightarrow$ R  bits are set to 1 for pages that are used
      * periodically examine R bits
        * dk order of use but know pages that were/ were not used
    * Additional-Reference-Bits Algorithm
      * keep a counter for each page
      * at regular intervals for every page do
        * shift R bit into high bit of counter register
        * shift other bits to the right
        * pages with larger counters were used more recently

#### Second Chance Algorithm / CLOCK

* Algorithm Idea

  * FIFO
  * But inspect Reference Bit when page is slelected
    * if ref bit == 0 $\Rightarrow$ evict? replace the page : set the ref bit to 1
    * if ref bit == 1 $\Rightarrow$ evict?  clear ref bit + reset arrival time of page to current time: stay
  * pages that are used often enough   to keep the ref bits >= 0   will stay
  * can cobime with M bit to create 4 classes of pages
    * called Not Recently Used in text

* Implementation : CLOCK

  * replace the page that is old enough
    * arrange all of physical PF in a big circle (CLOCK)
    * a clock hand is used to select a good LRU candidate
    * sweep through the pages in circular order like a clock
      * only sweep the hand when a new page needs to be brought in
        * if not new page $\Rightarrow$ 
        
          1. hand hold at the current page
          2.  set hand.ref bit to 1
        
        * if new page $\Rightarrow$ 
        
          * if no need to replace/evict  $\Rightarrow$ 
        
            1. set new page/hand.ref bit to 1
            2.  sweep/time++
        
          * if need to replace/evict $\Rightarrow$ 
        
            * start inspecting the hand pointing page Loop (keep check clockwised)
        
            * if hand.ref bit == 0  $\Rightarrow$
              1.  replace this page with new page
              2.  set the new_page.ref bit to 1 
              3.  sweep/time++
              4. break the inspecting Loop (since has evicted)
            * if hand.ref bit == 1 $\Rightarrow$
              1.  turn this page ref bit to 0 
              2.  sweep/time++

* Contribution

  * arm moves quickly when pages are needed

* Limitation

  * **low overhead when plenty of memory**
  * if mem is large 
    *  **accuracy of information degrades**

#### Counting-Based Replacement

* Idea
  * count number of uses of a page
  * LFU == Least Frequently Used 
    * replace the page used least often
  * MFU == Most Frequently Used
    * favour new pages
* Limitation
  * poor approximation of OPT
  * pages that are heavily used at one time tend to stick around even when not needed anymore 
  * new pages haven't had a chance to be used much

#### Page Buffering

* Problem Addressing
  * the replacement algorithm will select a victim page when a new page needs to be brought in
  * most of these algorithms are too costly to run on EVERY page fault
  * so we reduce the frequency of running
* Idea
  * **maintain a pool of free pages**
  * run replacement algorithm when pool becomes to small ( i.e. low water mark )
  * free enough pages to at once replenish pool ( i.e. high water mark)
  * on page fault 
    * grab a frame from the free list
    * frame on free list still hhold previous contents
    * can be rescued if virtual page is referenced before reallocation



### Memory Allocation Usage Implementation 

>WS == Working Set

#### Fixed VS Variable Space Allocation

* Observation
  * need to allocate memory to competing processes in a multiprogramming system
* Memory Allocation Algorithm 
  * Fixed space algorithm
    * Each process is given a limit of pages it can use
    * Reaching the limit $\Rightarrow$ it replace from its own pages
    * Local replacement
      * some process may do well while other suffers
  * Variable/Dynamic space algorithm
    * Process 's set of pages grows and shrinks dynamically
    * Golbal replacement
      * one process can ruin all the rest
    * Local replacement
      * replacemnt / set size are separate from each process



#### Working Set Of A Process

* Role

  * model the dynamic locality of tis memory usage

* Definition

  * Annotation

    * Let $t_p$ be a time point
    * Let $\Delta$  be a number of the page reference that indicate the working set window
    * measured in page refs
    * like a page working observation range or limitation starting from some time point before until $t_p$
    * Let $P_i$ be a page referenced at time point $i$ 
  * Formula
    * WS$(t_p\text{,  }\Delta)$  = { $P_i |$ IsReferenced($P_i$) == True \&\& $i \in [t_p, t_p-\Delta]$}
    * A page is in the WS $\iff$ it was referenced in the last $\Delta$ reference
  * eg
    * ref string
      * 2 6 1 5 7 7 7 5 1 4 3
    * Let init time be 0
    * Assume one ref per second
    * if $t_p$ = 5, $\Delta$ = 4 $\Rightarrow$ WS$(t_p\text{,  }\Delta)$  = WS$(5\text{,  }4)$  = {6, 1, 5, 7} 
    * if $t_p$ = 5, $\Delta$ = 5 $\Rightarrow$ WS$(t_p\text{,  }\Delta)$  = WS$(5\text{,  }4)$  = {2, 6, 1, 5, 7} 
    * if $t_p$ = 11, $\Delta$ = 11 $\Rightarrow$ WS$(t_p\text{,  }\Delta)$  = WS$(5\text{,  }4)$  = {2, 6, 1, 5, 7, 4, 3} 
    * if $t_p$ = 11, $\Delta$ = 1 $\Rightarrow$ WS$(t_p\text{,  }\Delta)$  = WS$(5\text{,  }4)$  = {3} 
  
* Working Set Size

  * the number of pages in the WS
  * aka | WS$(t_p\text{,  }\Delta)$ |
  * changed with program locality
    * during periods of poor locality $\Rightarrow$ reference more pages $\Rightarrow$ size &uparrow;

* Goal

  * WS == { process needs in memory }
    * prevent heavy faulting
    * each process has parameter $\Delta$ = few-fault WS

* WS Problem

  1. how to determin $\Delta$
  2. how to knoe when the WS changes

  * both too hard to answer
    * not in practice as a page replacement

* Use

  * **used as an abstraction**
    * intuition still valid
    * eg: how much memory does Firfox needed == what is the size of Firfox's WS
  * useful approximation in practice



### Advanced Functionality Using VM Tricks

> VA == Virtual Address
>
> PT == Page Table
>
> PF == Page Frame
>
> PTE == Page Table Entry
>
> CoW == Copy on Write
>
> VM == Virtual Memory

#### Sharing Memory

* Observation
  * private VA space protect app from each other $\Rightarrow$ difficult to share data ( have to copy )
* Solution
  * use shared memory to allow processes to share data using direct memory references
* Implementation Using PT
  * Both PTE in each table  map to the same PF
  * Each PTE  can have different protection value
  * Must update Both PTE when page become invalid
* Mapping Shared Rigion
  * type 1: map shared mem at **same** VA in each process's addr space
    * pro
      * shared pointers are valid (why? same as whole system using one place no protection )
    * con
      * less flexible
  * type 2: map shared mem at **different** VA in each process's addr space
    * pro
      * flexible (no address space conflicts)
    * con
      * shared pointers are invalid (why? protection!)
  * if a shared segment pointer reference an addr outside the segment $\Rightarrow$ segmentation fault



#### Copy On Write

* Observation

  * OS spends a lot of time copying data
    * **sysCall arguments between user/kernel space**
    * entire addr space to implement ```fork()```

* Solution

  * use  CoW to **defer large copies as long as possible to avoid them altogether**

  * example with process inheritance

    * Implementation
      * **create shared** mappings of **parent pages in child virtual address space** [instead of cp pages]
      * shared pages are protected as Read-Only in child

    * sysCall Performance
      * reads happen as usual
      * **write generate a protection fault**
        * trap to OS
        * cp page
        * **change page mapping to client page table**
        * restart write instruction
    * help ```fork()``` since no need to write 2 time (write as initial-child + update child)
    * implemented as Unix ```vfork()```

#### Memory Mapped Files

* Contribution
  * mapped files enable processes to do file I/O 
    * using load and store instruction
    * [ instead of ```open(),  read into buffer, operate on buffer ...``` ]
* Functionality
  1. bind a file to a VM region ( i.e. ```mmap()``` in Unix )
     * PTE map the VA to PF holding file data
     * VA base  + N refers to the offset N in the file
       * can read or write at various offsets in file using memory operation
  2. Initially all pages mapped to file are invalid
     * OS read a page from file when invalid page is accessed
     * OS write a page to the file when evicted / region unmapped
  3. File is essentially backing store for that region of VA space [ instead of using the swap file ]
     * VA space not backed by real files also called Anonymous VM
* Advantage
  * **Uniform access for files and memory** 
    * just use pointers
  * Less copying
* Drawbacks
  * Process has less control over data movament
    * OS handles faults transparently
  * Does not generalized to streamed I/O like pipes / sockets

### Summary

#### Definition

##### Page Frame

* a fixed-size block of physical memory used in paging system
* hold parts of a process's address space
* frames are identical
* any frame can be used to hold any page for any process