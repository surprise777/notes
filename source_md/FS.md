## File System

***



[TOC]





***

### What is an OS ( I/O Overview )

> OS == Operating System 
>
> CPU == Processor
>
> I/O == Input/Output
>
> HD == Hard Drive 
>
> 1B == 1 Byte == 8 Bit
>
> KiB == KB == 1024B
>
> MiB == MB == 1024KB 
>
> GiB == GB == 1024MB
>
> Hierarchy [等级制度] 
>
> Concurrency [并行]
>
> Persistence [永久性存留]

#### Definition

* A body of software to make sure the system operates corectly and efficiently on an easy-to use manner, between user APP with Hardware 

#### Goal 

==( often contradictory )==

* Convenience for user
* efficiency for machine use/operation

#### Role ( OS  == ? ) 

* **Virtural Machine**
  * extend + simplify interface for physical machine
  * provide a library of functions accessible though an API
* **Resource Allocator**
  * allow the resources (HW, SW, data) properly use the system operation
  * provide an environment that programs can work uesfully in in this
* **Control Program**
  * ctrl the execution of user programs without error/improper use of PC, especially the programs related to I/O devices

#### OS Themes

* **Virtualization**
* physical resource (CPU, Mem, HDs) &rightarrow; general + powerful + ez-2-use form
  * only one/a few really existed resource &rightarrow; multiple/unlimited illusion
  
* **Concurrency**
* coordinate multiple activities together to ensure correctness
  
* **Persistence**
* Some data needs to survive from crashes or power failures

#### How to relate to other parts of PC system

* **Covinient abstraction of Hardware**
  * Address? Page table?
* **Protection, Security, Authentication**
  * PID? 
* **Communication**
  * System call?



### Storage/Memory

> HD == Hard Drive
>
> FS == File System
>
> Rudimentary [基本的] == basic
>
> Auxiliary [辅助的] == assistant/ back-up
>
> Volatility [波动性]
>
> Cache [缓存]

#### Storage/Memory Hierarchy
* **Rudimentary memory hierarchy**
  * Processor Register
  * Main Memory
  * Auxiliary Memory

* **Classified by**
  * Memory Speed
  * Cost
  * Volatility
* **Cache**
  * install to hide the performance diff between two LVs' large access-time gap
* **Larger + Slower + Cheaper**
  * CPU ( within LV1 Cache) &rightarrow; LV2/LV3 Cache &rightarrow; Main Memory &rightarrow; Disk

#### How to virtualize persistent storage

* Convenience  $\gets$ Virtualization + Abstraction
  * Let files and directories abstract away from the HD
* Efficiency  $\gets$ Control Program  
  * Let FS controls when and how data transferred to persistent storage



### What is a FS

> FS == File System
>
> Dominant [支配]
>
> Truncation [截取]

#### Role  ( FS  == ? )

* **Data Control Program**
  * Provide long-term information storage, i.e., persistent storage
* **File Management System**
  * Implement an abstraction (i.e. files) for secondary storage
  * Organize files logically by directories
  * Permit sharing of data between processes, people, and machines
  * Protect data from unwanted access (security)

#### Requirement

* Store very large amountes of info
* Make info survive from the termination of process
* Make info concurrently accessed by multiple processes

#### File Operations

* **Creation**
  * find space in file system
  * add entry to directory
  * map file name to location and attributes
* **Writing**
* **Reading**
  * Dominant abstraction: File &rightarrow; Stream
* **Reposition within a file**
* **Deletion**
* **Truncation/Appending**
  * May erase all/part of the contents of a file while keeping attributes

#### Handling File Operations

* Search d_entry associated with the named file
* When the file is first used, store its attribute info in a System-wide Open-file Table
  * Notice: The index into table is used for sub-op, now no searching required



### File Sharing

> FS == File System
>
> DB == Database
>
> Synchronization [同步]
>
> Sequential Access [顺序存取]
>
> Semantics [语义]
>
> Identical [一模一样]
>
> Consistency [一致性]
>
> Dangling [悬空]

#### Role

* Basis for communication and synchronization

#### Key Issue

* **Semantics of concurrent access**
  * reading process + writign process?
  * multi processes open one file for writing?
* **Protection**

#### Addressing

* Using Two Level Internal Tables
  1. Per-Process Open File Table (i.e. one for each process)
     * Contains the current position in each file that certain process has opened
  2. System-wide Open File Table (i.e. one for whole system)
     * Contains the initial attribute info for each file that whole system has opened
* Each entry of 1. has a pointer to a certain entry of 2. for process independent info

#### File Access Method

* **General-Purpose FS (Simple)**
  * Sequetial access
    * read fixed  bytes one at a time in order
  * Direct access
    * random access given block/byte number
* **DBFS (Sophisticated)**
  * Record access
    * fixed/variable length
  * Indexed access
* **Example in real world**
  * Unix/Linux
  * Windows
* **Older == More complicated**

#### File Sharing Implementation

* **File Link** 
  * a new dir entry with a pointer to another file or subdirectories
* **Link Type**
  * **Symbolic/soft link**
    * d_entry  with pointer to a d_entry that holds true path to the linked file
    * indirect and form a new d_entry point to the d_entry of the linked  file
  * **Hard link**
    * d_entry  with pointer to the linked file
    * direct and form a new d_entry identical to the d_entry of the linked file
* **Issue With Links**
  * **Multiple Absolute Path Per Files**
    * should avoid traversing shared structs in FS more than once
  * **Maintaining Consistency Problem**
    * how to update permissions of hard link d_entry
  * **Shared File Space Deallocation + Reuse**
    * deletion of the file entry itself deallocates space and leave the link pointers danling
    * keep a reference count for hard links
  * Sharing Signal
    * how to tell when porcesses are sharing the same file



### Directories

> Acyclic [非循环]

#### What are directory and file?

* File
  * an abstraction to let us refer to persistent data on disk
* Directory
  * a logical organization of files
  * a special type of file that contains a list of d_entries

#### Role

* **Logical structure for file systems**
  * For user
    * a means to organize files
  * For FS
    * convenient nameing interface
      1. allow the implementation to separate logical file organization from physical file placement
      2. store info about files (owner, permission, etc)

#### Structure

* Self

  * Effectively Random Unorder Entries:  ( name, metadata )
  * Stored same as files
    * Keep the single kind of secondary storage unit
* Level
  * Tree - Structured
    * Single/Two/Multi Level
  * Acyclic - Graph
    * Allow for shared directories (same file/subdir may be in 2 different dir)

#### Implementation

* Option 1: List
  * List < file_name, List< pointer_to_data_block >  >
  * Linear search to find entries
  * Pros
    * Easy to implement
  * Cons
    * Slow to execute (notice that directory ops are frequent)
* Option 2: Hash Table
  * HashTable <file_name: pointer_to_file_info_struct_in_list >
  * Create a list of file info struct
  * Pros
    * Fast Time complexity
  * Cons
    * Hash Table takes space

### File System Implementation

> FS == File System
>
> PC == Personal Computer
>
> Granularity [间隔尺寸]
>
> Replicated [被复制]
>
> Sequential Access [顺序存取]

#### Structure

* **FS Block Size**
  * the standard size defined by FS  of each block on  the disk
    * disk space is allocated in granularity of blks by this to store files in FS
  
* **Master Block/Partition Control Block/Superblock**
  * the important structure to connect the disk to PC
  
  * determine the location of root directory
    * always at a well-known disk location
    * often replicated across disk for reliability
    * include other metadata about FS
  
* **Free map**
  * determine which blocks are free
    * usually a bitmap, one bit per block on the disk
    * also stored on disk, cached in memeory for performance
  
* **Remaining Disk Block/Data Block**
  * store files and dirs

#### Disk Layout Strategies

* **Cause**

  * Files often span multiple disk blocks
* **Major Strategy**

  * **Contiguous Allocation**
* all blocks of file are located together on disk
  
* Pros
      * sequential access fast
      * allocation fast
      * deallocation fast
      * small amount of metadata ( i.e. simplfied the directory access )
      * allow/ez to indexing
    * Cons
      * inflexible
      * external fragmentation
      * require compaction
      * need to move whole files around
  
* **Linked/Chained Structure**
  * each block point to the next, dir pointes to the first
  
  * Pros
    * ez sequential/streaming access
      * disk blocks can be anywhere
      * no external fragmentation
    * Cons
      * direct access is expensive
      * if one data block is corrupted &Rightarrow; lost all rest of file
    
  * **Indexed Structure** 
* An index block contains pointers to many other blocks
    * May require multiple, linked index blocks
  * Pros
      * handle reandom accsess well
      * small files, still quick for sequential and random access
      * no external fragmentation
    * Cons
      * limited file size
      * &uparrow;cost of access bytes near the end of large files 
* **Unix Inodes**

  * ITSELF == Indexed Structure ( for files )
  * D_entry < file/dir_name, inode_of_file/dir >
  * Each indie contains file metadata, + 15 block pointers
    * Block [ 0:12 ] == Direct Block Pointer
      * disk address of first 12 data blocks of this file/dir
    * Block [ 12 ] == Single Indirect Block Pointer
      * disk addr of a block containing the disk addr of data blocks
        * contain the disk addr of >12 data blocks of this file/dir
    * Block [ 13 ] ==  Double Indirect Block Pointer
      * disk addr of a block containing the disk addr of such single indirect blocks 
        * contain the disk addr of > 12 + 1 $\times$ BLOCK_SIZE data blocks of this file/dir
    * Block [ 14 ] == Triple Indirect Block Pointer
      * disk addr of a block containing the disk addr of such double indirect blocks 
        * contain the disk addr of > 12 + 1 $\times$ BLOCK_SIZE $\times$ BLOCK_SIZE data blocks of this file/dir
  * **Imbalenced tree**
    * Most files are small < 2KB
    * Files are usually accessed sequentially
    * Dir are typically small < 20 d_entry
  * Example Unix Inode
    * Ext2 FS Size == 128 bytes

  
### FS Design Case: VSFS

  > VSFS == Very Simple File System
  >
  > OS == Operating System
  >
  > DS == Data Structure
  >
  > pointer_size = 4 byte
  >
  > Disk Volume [磁盘卷]

#### Main Idea

* Create a FS for an Unformatted Disk
  * Create some structs in it for data to be ez to find + organize
* Where to store file data + metadata structs (i.e. nodes)
  * data blocks + inode blocks
* How to keep track of data allocations
  * inode bitmap + data block bitmap
* How to locate file data + metadata
  * traverse path
    * superblock $\to$ root d_entry $\to$ LV0 root dir inode $\to$ root dir data blocks $\to$ d_entries in them $\to$ LV1 dir/file inodes $\to$  LV1 dir/file data blocks $\to \cdots$
* What are the limitations
  * max file size depends on # inode and # hierarchy of inode and block size

#### Overal Orgnization

* Input Assumption
  * Unformatted Raw Disk Size = $nx$ KB
  * Inode Size = $\frac{x}{1024}$ KB
  * Block Size = $x$ KB
  * #of Inode = $1024m$
  * max #of block pointer in inode = $p$ 
    * notice that $4p \leq x$ since fixed inode size and fixed pointer size
  * max #of indirect pointer hierachy in inode = $t$
  
* Conputed Size Value
  * #of all blocks = $\frac{\text{Disk Size}}{\text{Block Size}} = n$
  * #of inode block $\frac{\text{#Inode} \times \text{Inode Size}}{\text{Block Size}} = m$
  * #of block bitmap = $\frac{\text{#Blocks}}{\text{byte to bit }\times \text{ Block Size}} = \frac{n}{8x}$ 
  * #of inode bitmap = $\frac{\text{#inode}}{\text{byte to bit }\times \text{ Block Size}} = \frac{1024m}{8x}$
  * #of data blocks =  $  n-m-\frac{n}{8x}-\frac{m}{8x}-1$
    * #all_blks - #inode_blk - #blk_bitmap - #inode_bitmap - #superblk
  * max inode direct pointer = $p - t$ 
  
* Limitation
  
  * Max file numer
  
    * 1024$m$  (i.e. #of inode Since one file match one inode)
  
  * Max data block use per file
  
    * $(p-t)+(\frac{1024x}{4})^1+(\frac{1024x}{4})^2+(\frac{1024x}{4})^3+\cdots+(\frac{1024x}{4})^t $
  
    * =#direct data block +  
      $ ($ #ptr stored in $LV(t)$ indirect blk pointing to $LV(t-1)$ indirect blk $)^t$
  
#### Disk Region

* **Superblock**
  * Key Region for mounting FS + Formatting Disk Into FS
    * OS read the superblock FIRST
    * identify FS type and other parameters from this
    * attach the disk volume to FS tree with proper settings
  * Structure
    * type of FS (indicated by some magic number)
    * #inodes
    * #data block (not all blks)
    * index of inode table begin
* **Inode Table**
  * Metadata structs storing info of each ( one ) file for FS to track
    * inode_block_id = $\frac{\text{inode id}}{\text{block size}}$
  * Structure
    * mode
    * file size
    * #block of  this file
    * block pointer[ max #of block pointer in inode ]
    * $\cdots$
* **Allocation Strucure/ Free Map**
  * keep track of which blks used/free
  * Structure 
    *  Bitmap (each bit == one block, 0 for free 1 for use)
  * Type
    1. data block bitmap
       *  #block =$\frac{n}{8x}$
    2. inode bitmap
       *  #block =$\frac{1024m}{8x}$



#### Inodes-Based Approach DS

* Inode
  * DS representing a FS object like file, dir, link
  * attributes indicate metadata file info and disk block location
    * describe where on the disk the blocks for a file/dir are placed
  * no file name (file name mapped by another DS d_entry)
  * $\ne$ dir
* Directory Entry
  * List <file_name, inode_id> 
    * map file names to inodes
  * represent for a file/dir/itself ( . )/parent dir( . )
  * To open "/f"
    1. use Superblock $\to$ identify the inode for root dir "/"
    2. read root dir inode $\to$ data block for root dir
    3. read data block for root dir $\to$ look for d_entry of "f"
    4. read d_entry $\to$ identify the inode for "f"
    5. read inode of "f" into memory $\to$ first data block of "f" on disk
    6. read first data block of "f" into memory to access the data in the file



#### Other Approaches From Inode to Data

* **Extent - Based**
  * A new struct extent < pointer_to_start_block, #continuos_blk >
  * Pros
    * use smaller amount of metadata per file
    * file allocation is more compact
  * Cons
    * less flexible than the pointer-based approach
    * external fragmentation
  * Example
    * EXT4
    * HFS+
    * NTFS
    * XFS
* **Link - Based**
  * LinkList from the first data block to next
  * Use an in-memory File Allocation Table indexed by address of data block
  * Example
    * FAT



### Secondary Storage

> Secondary [次级]
>
> Floppy [软盘]
>
> Volatile [不稳定]

#### Quality

* Large
* Persistent
* Slow

#### Secondary Storage Devices

* Drum (ancient story)
* Magnetic Disk
  * Fixed
  * Removable/Floppy
* Optical Disk
  * Write-Once + Read-Many
    * CD-R
    * DVD-R
  * Write-Many + Read-Many
    * DVD-RW
* Flash Memory
  * Solid State + Non-Volatile Memory



### Magnetic Disk

> OS == Operating System
>
> FCFS == First Come First Seek
>
> FIFO == First In First Out
>
> SSTF == Shotest Seek Time First
>
> Actuator [传动装置]
>
> Latency [延迟]
>
> Optimization [最优化]
>
> Skew [时钟相位差]

#### Disk Physical Structure

* **metal arm component**
  * Arm
  * Actuator
  * Read/Write Head
* **circle platter component**
  * Platter
  * Upper Surface
  * Lower Surface
*  **cylinder component**
  * Track
  * Sector ( bar by bar on track)

#### Disk Performance

* **Performance Dependency**
  * seeks reduced
  * large transfers are used

* **Seek**
  * role
    * move the disk arm to the correct cylinder
  * time dependency
    * how fast disk arm can move
  * time cost
    * avg 5-6ms depending on distance
  * development improving speed
    *  very slowly ( 8% per year )
* **Rotation**
  * role
    * waiting for the sector to rotate under the head
  * time dependency
    * the rotation rate of disk (7200 RPM SATA)
  * time cost
    * avg latency of a half rotation
  * development improving speed
    *  no change in recent year
* **Transfer**
  * role
    * transferring data  from surface into disk controller electronics 
    * sending it back to the host
  * time dependency
    * the density (increasing quickly)
  * time cost
    * 100MB/S ( avg sector transfer time of 5us )
  * development improving speed
    *  imporving rapidly ( 40% per year )
* Traditional Service Time Component
  * Read ( Transfer) sector1 $\to$ Seek for sector2  $\to$  Rotation Latency  $\to$  Read
  * OS will minimize particularly seeks and rotation when using the disk

#### Hardware Optimization

* **Track Skew**
* Problem
  
  * if the arm moves to outer track too slowly $\to$  miss the close sector
  
* Address
    * skew the outer track location far at the same time head to meet sector 
  
* **Zone**
* Problem
  
  * each sector has fixed 512 size $\to$  outer tracks are larger by geometry 
  
* Address
    *  Outer track shoude hold more sectors instead of same #sector /round
  
* **Cache (aka Track Buffer)**
* Small Memory Chip
  
  * part of the hard drive
  
  * usually 8-16 MB
  
* Diff VS OS cache
  
  *  it is aware of the disk geometry (OS cache is just an installation)
    * when reading a sector, may cache the whole track to speed up future reads on the same track

#### Disk and the OS ( Disk Interface )

* Disks are messy physical devices
  * errors
  * bad blocks
  * missed seeks
* OS hide the miss in disks from such higher level software
  * low-level device control
    * initiate a disk read
  * higher-level abstractions
    * file
    * database
* OS may provide diff-level disk access $\to$ diff-level clients
  * physical disk ( access )
    * surface
    * cylinder
    * sector
  * logical disk ( access )
    * #block of disk
  * logical file  ( access )
    * file block
    * record
    * #byte

#### Disk Scheduling

* Cause
  
  * seeks are so expensice (avg propotional to ms)
  
* Approach
  
  | RequestQueue    |      |      |      |      |      |
  | :-------------- | :--- | ---- | ---- | ---- | ---- |
  | Head @       53 | 98   | 183  | 37   | 122  | 14   |
  
  * **FCFS** 
    
    * reasonable when load is low
    * long waiting times for log request queues
    * example
      * scheduling as a simple queue FIFO
        * identical with request queue [ 53, 98, 183, 37, 122, 14 ]
      * execution
        * 53 $\stackrel{+\text{45}}{\longrightarrow}$ 98 $\stackrel{+85}{\longrightarrow}$ 183 $\stackrel{-146}{\longrightarrow}$ 37 $\stackrel{+85}{\longrightarrow}$ 122 $\stackrel{-108}{\longrightarrow}$ 14
      * time cost
        * |+45| + |+85| + |-146| + |+85| + |-108| = 469
    
  * **SSTF**
    
    * minimize arm movement ( i.e. seek time )
    * maximize request rate
    * favor middle blocks
    * example
      * scheduling as shortest-distance-priority queue
        *  option 1: for each current head
          1. sort the current request queue by distance with head
          2. get the nearest sector
          3. remove this sector from request queue
          4. update head by this nearest sector
        * option 2: request queue built as AVL and know current head
          1. get the next biggest sector of head from the tree
          2. get the next smallest setor of head from the tree
          3. select the shortest distance sector from one and two
          4. remove the current head from request queue AVL
          5. update current head by selected sector
        * result queue [ 53, 37, 14, 98, 122, 183 ] 
      * execution
        * 53 $\stackrel{-\text{16}}{\longrightarrow}$ 37 $\stackrel{-23}{\longrightarrow}$ 14 $\stackrel{+84}{\longrightarrow}$ 98 $\stackrel{+24}{\longrightarrow}$ 122 $\stackrel{+61}{\longrightarrow}$ 183
      * time cost
        * |-16| + |-23| + |+84| + |+24| + |+61| = 208
    
  * **SCAN/Elevator**
    
    * service requests in one direction until sector 0, then reverse
    
    * example
    
      * scheduling as sorted order queue (maybe AVL tree)
    
        1. randomly set initial head as pivot sector
        2. sort the request queue
        3. divid the sorted request queue as 2 sub queues by pivot
        4. execute the smaller-than-pivot sub from pivot **to sector 0**
        5. execute the bigger-than-pivot sub queue by insc order
    
        * result queue [<u>53</u>, 37, 14, 0, 98, 122, 183]
    
      * execution
    
        * 53 $\stackrel{-\text{16}}{\longrightarrow}$ 37 $\stackrel{-23}{\longrightarrow}$ 14  $\stackrel{-14}{\longrightarrow}$ 0 $\stackrel{+98}{\longrightarrow}$ 98 $\stackrel{+24}{\longrightarrow}$ 122 $\stackrel{+61}{\longrightarrow}$ 183
    
      * time cost
    
        * |-16| + |-23| + |-14| + |+98| + |+24| + |+61| = 236
    
  * **C-SCAN**
    
    * like SCAN but only go one direction ( like typewriter )
    
  * **LOOK/C-LOOK**
  
    * like SCAN/C-SCAN but only go as far as last request in each direction (not full width of the disk)
  
    * example
  
      * scheduling as sorted order queue (maybe AVL tree)
  
        1. randomly set initial head as pivot sector
        2. sort the request queue
        3. divid the sorted request queue as 2 sub queues by pivot
        4. execute the initial head
        5. execute the smaller-than-pivot sub queue by desc order
        6. execute the bigger-than-pivot sub queue by insc order
  
        * result queue [<u>53</u>, 37, 14, 98, 122, 183]
  
      * execution
  
        * 53 $\stackrel{-\text{16}}{\longrightarrow}$ 37 $\stackrel{-23}{\longrightarrow}$ 14 $\stackrel{+84}{\longrightarrow}$ 98 $\stackrel{+24}{\longrightarrow}$ 122 $\stackrel{+61}{\longrightarrow}$ 183
  
      * time cost
  
        * |-16| + |-23| + |+84| + |+24| + |+61| = 208
  
* Impact

  * In general, unless there are request queues, disk scheduling low impact
  * important for servers but less so for PC

* Implementation on disks themselves

  * disk know their layout better than OS
  * disk may ignore any scheduling done by OS

#### Software Interface Layer

* Major Layer

  |                   *Program*                   |
  | :-------------------------------------------: |
  |         **<   File Name, Offest   >**         |
  |                    *FS/DB*                    |
  |         **<   Partition, #Block   >**         |
  |     *Device Driver (Persistent Storage)*      |
  |      *Device Driver (Secondary Storage)*      |
  | **<              #Disk, #Sector           >** |
  |               *I/O Controller*                |
  |  **<        Cylinder, Track, Sector     >**   |
  |                 *Disk Media*                  |
  
* Role

  * abstract details from the divice below it
    * naming 
    * address mapping
  * provide abstractions to the divice above it
    * caching
    * reuest transformation

#### Disk Interaction



### Disk And FS

> FS == File System
>
> FFS == Fast File System
>
> LBN == Logical Block Number
>
> Amortization [分期摊销]
>
> oblivious [忽略]
>
> parameterize [参数化]

#### High-Level Disk Characteristic

* **Closeness**
  * reduce seek times by putting related things close to one another
  * benifits can be $n^2$ range
* **Amortization**
  * amortize each positiong delay by grabbing lots of useful data
  * benifits can be $n^{10}$ range

#### FS Key Idea (Role)

* be aware of disk characteristics for performance
  * allocation algorithms to enhance performance
    * allocating related data close together
  * Request scheduling to reduce seek time

#### Allocation Strategy

* **Goal**
  * improvment for Inodes on Indirection + Independence
    * +File size grows dynamically
    * +Allocations are independent
    * -Hard to achiece closeness and amortization

* **Traditional UNIX FS**
  * Storage as linear array of blocks
    * each block has a LBN
  * Small blocks ( 1k )
    * low bandwidth utilization
    * small max file size ( function of block size)
  
  * Pros
    * simple straightforward implementation 
    * ez to implement and understand
  * Cons
    * poor utilization of disk bandwidth (lots of seeking)
      * fragmentaion of an aging file system causes more seeking
      * inodes allocated far from blocks, going back and forth from inodes to data blocks to traverse files/dirs causes more seeking
  
* **BSD FFS :  Disk-Aware File System**
* **Cylinder Group** ( aka allocation group )
  * disk partitioned into groups of cylinders
  * data blocks in same file allocated in same grp
    * files in same dir allocated in same grp
    * inodes for files allocated in same grp with file data blocks
    * superblock + bitmap + inode + datablock
  * **Free space requirenent**
    * the disk must have free space scattered across cylinder for allocation
    * 10%of the disk is reserved just for this purpose
    * when allocating a large file , break it into large chunks and allocated to different cylinder group instead of full the one group
    * if preferred cylinder group is full allocated from a nearby group
  * Fix **using larger block** (4K)
    * very large files only need two level of indirection
  * introduce **fragment** ( 1k pieces of a block) to fix internal fragmentation
  * Pros
    * allocation group provides closeness
    * reduce num of long seeks
  * Cons
    * media failues
      * replicate master block
    * device oblivious
      * parameterize according to device characteristics



### Reliability and Write Optimization

> FFS == Fast File System
>
> Transaction [交易处理]

#### Goal

* how to guarantee consistency of on-disk storage
* how to handle OS crashes and disk errors
* how to optimize writes

#### Consistency Issue/Crash Recovery

* Metadata update must be synchronous operation
  * FS op affect multiple metadata blocks
    * write newly allocated inode to disk $\to$ d_entry name with dir
    * remove a directory name $\to$ deallocated inode
    * deallocate inode $\to$ place files' data blocks into free list
  * If OS crashes in between any of this op, FS == inconsistent state
* Solution ( overview )
  * fsck
    * post-crash recovery process to scan FS structs + restore consistency
      * all data blocks pointed by inodes/indirect blocks must 1 in bitmap
      * all allocated inodes must mapped in some d_entry
      * inode link count must match
  * log update
    * enable to roll-back or roll-forward
* Scenairo Example

#### fsck

* **Definition**
* a Unix tool for finding inconsistencies and repairing them
  
* **Limitation**
* only care the FS integrity/metadata is consistent
  * cannot know when Db is garbage
  * too slow
    * disks are very large nowadays scanning all this could take hours
    * even for snall inconsistency must scan whole disk
  
* **Check List**
1. Superblock
  
   * sanity checks
     * use another superblock copy if suspected corruption
  
2. Free block
  
   * scan inodes/indirect blocks, build bitmap
     * inodes/data bitmaps inconsistency $\Rightarrow$ resolve by trusting inodes
  
3. Inode state
  
   * check inode fields for possible corruption ( ex: must have valid mode )
     * if cannot fix $\Rightarrow$ remove inode and update inode bitmap
  
4. Inode link
  
   * verify #links for each inode
  
     * traverse directory tree
  
     * compute expected #link
       * fix if needed
  
   * if inode discovered but no dir refers to it $\Rightarrow$ move to "lost+found"
  
5. Duplicate
  
   * check if two different inodes refer to same block
     * clear one if obviously bad / give each inodes its own copy of block
  
6. Bad blocks
  
   * bad pointers outside of valid range
     * remove the pointer from the inode or indirecrt block
  
7. Directory checks
  
   * integrity of directory structure
       * make sure "." and ".." are the first entries
       * each inode in a directory entry is allocated
       * no directory is linked more than once

#### Journaling

* aka **Write-Ahead-Logging**
* **Basic Idea**
* log the operations you are about to do beforr overwriting structures
  * if crash during write$\Rightarrow$ go back to journal and return the actual write
    * don't need to scan the entire disk
    * can recover data as well
  * if crash before journal write finish $\Rightarrow$ act write not happened
    * nothing is inconsistent
* **Jounal Structure**
* EXT3 FS Extend EXT2 with journaling capability
    * journal identical with on-disk format
    * can be just another large file
    * can be allocated from anywhere on disk like data blocks
  * contain a sequence of transaction
* **Transaction Structure**
* TxBegin block
    * contain a transaction TID
  * blocks with the content to be written
    * **physical logging**
      * log exact physical content
    * **logical logging**
      * loh more compact logical representation
  * TxEnd block
    * contain a transaction TID
* **Journal Trade-off**
  * Pros
    * recovery is much faster with journaling
      * replay only a few transactions instead of checking the whole disk
    * ensures FS consistency
    * complexity is in the size of the journal instead of the disk
    * metadata journaling is the most commonly used
      * reduce the amount of traffic to the journal
      * provide reasonable consistency guarantee at the same time
  * Cons
    * normal ops are slower
      * every update must first write to the journal then do the update
      * writing time is at least doubled
    * journal writing may break sequential writing
      * need to jump back-and-forth betwen write to journal and to real
* **Data Jounraling Process**
1. write the transaction to the log
     * option 1 : write each block at a time into Journal 
       * slow
       * unsafe if power off before several op before TxEnd same as valid
     * option 2: split into 2 steps
       1. **Journal Write Step**
          * write all except TxEnd each block at a time into Journal 
          * if crash $\Rightarrow$ skip the pending update
       2. **Journal Commit Step**
          * write TxEnd only when step 1 completed into Journal 
  2. **Checkpoint Step**
     * write the blocks/actual data and metadata to right locations on FS
     * if crash $\Rightarrow$ 
       * on reboot
       * scan the journal and look for committed transactions
       * replay these transactions (after this FS is ensured consistent)
       * called redo loggiing
  3. **Free Step**
     * mark the transaction free in the journal
* **Metadata Journaling Process**
  * now we only write FS metadata to journal
  * option 1: same as data journaling process
    * unsafe 
      * if crash before all data is writen, inode may point to garbage data
  * option2: write data before writing metadata to journal
    1. Write Data (Wait until it completes)
       * Data Journal Write Step (optional)
       * Data Journal Commit Step  (optional)
       * Data Checkpoint Step
    2. Metadata Journal Write Step
    3. Metadata Journal Commit Step
    4. Metadata Checkpoint Step
    5. Free Step



### Log-Structured File System

> LFS == Log-Structured File System
>
> HDD == Hard Drive
>
> FFS == Fast File System
>
> CR == Checkpoint Region
>
> Locality [地点]
>
> Mitigate [缓和]
>
> Batch [批量]
>
> Obsolete [陈旧的]

#### Traditional FS

* Files lay out with spatial locality in mind
* Changes in place to mitigate seeks
* Avoid fragmenting files 
  * keep locality to reads perform well

#### Another Approach

* A new approach that prioritizes update performance
* **Gist**
  * instead of overwriting in place, append to log and reclaim obsolete data later (garbage collect)

* Observation + Solution
  * Memory increasing
    * don't care about reads
    * most will hit in memory
  * assume writes will pose the bigger I/O penalty
    * treat storage as a circular log
* Pros
  * write throughout improved/batched into large sequential chunks
  * very efficient write
  * crash recovery simpler
* Cons
  * initial assumption may not hold
    * reads much slower on HDDs?
  * less efficient reads
    * more indirection does not solve anthing 
    * but assume most reads hit in memory anyway
  * Garbage collection is a tricky problem

#### LFS

* **Indexed Structure**

  * write all FS data in  a continuous log
  * use inode and d_entry from FFS in Segment
  * Segment
    * superblock
    * summary block
      * contain pointer to next one
    * inodes
    * data blocks
  * if need a fresh segment
    * first clean an existing partially-used segment
    * garbage collection

* **Locating Inodes**

  * Unix FS v.s. LFS
    * ez for Unix FS
      * array on disk at fixed location
      * superblock $\to$ Inode Block Addr $\to$ #Inode block $\to$ #Inode
    * hard for LFS
      * updates are sequential
        * indoes scattered all over the disk
      * indoes not static keep moving
  * **Imap**
    * an inode map to find the inodes
      * inode number no longer a simple index
    * chunks of imap are placed next to new information
      * get updated frequently so not fixed part
      * keep seeks and performance
  * **Checkpoint Region**
    * pointers to the latest pieces of the inode map
      * find imap pieces by reading the CR

* **Crash Recovery**
  
  * Wrting Performance
    * buffer writes in segment
    * write segment to disk when full/at periodic time intervals
    * updates the CR periodically
  * Crash Solution
    * Uncommitted Segments
      * reconstruct from log after reboot
    * CR
      * Keep two CR at either end of the disk
      * alternate writes
      * update protocol (header/body/last block)
  
* **Garbage Collection**

  * Cause

    * LFS repetedly write latest version of a same file to new location on disk

  * Impact

    * Older versions of file ( i.e. Garbage ) are scatterd thoughout the disk

  * Goal

    * periodically clean up obslete version to free blks for subsequent write

    * cleaning done on a segment-by-segment basis

      * the segments are large chunks$\Rightarrow$ no small holes of free space

      

### Disk Failure Fix

> RAID == Redunant Array of Independent Disk
>
> Thoughput [生产量]
>
> Parity [奇偶校验]

#### Redundancy

* Definition
  * have more than one copies of the data
* Goal
  * prevent data loss when disk failures ( now journal doen't work )

#### RAID

* **Reliability Strategy**
  * data duplicated
    * mirror images
    * redunant full copu
  * data spread out across multiple disks with redundancy
    * can recover form a disk failure by reconstructing the data
* **Concept**
  * Redundancy/Mirroring
    * keep multiple copy of the same blk on diff drives in case drive failure
  * Parity Info
    * XOR each bit from 2 drives and store checksum on the $3^{rd}$drive
* **Multiple RAID level**
  * RAID 0 (2 diff disks)
    * Striping 
      * Files are divied across disks
      * Improve thoughput
      * One drive fails $\Rightarrow$ the whole volume lost
  * RAID 1 (2 same disks)
    * Mirroring 
      * Capacity is half
      * Any drive can serve a read
      * Improved read throughput
      * Write throughput is slower
      * One drive fails $\Rightarrow$ no data lost
  * RAID 5 (3 diff disks + 1 parity disk)
    * Block level striping
    * Distrubuted parity
    * A failed disk can be reconstructed from the rest



### Solid State Disk

> SSD == Solid State Disk
>
> HDD == Hard Drive
>
> amplification [扩充]

#### SSD Info

* **Definition**
  * Replace rotating mechanical disks with non-volatile memory
* **Type**
  * Battery-backed RAM
  * NAND flash
* Pro
  * faster
* Cons
  * expensive
  * wear-out/ flash-based
* **NAND flash storage technology op**
  * Read
  * Write
  * Erase!

#### SSD Characteristics

* Data cannot be modified "in place"
  * no overwrite without erase
* **Terminology**
  * **Page**
    * unit of read/write op
  * **Block**
    * unit of erase op
  * typically one block many pages
* Uniform random access performance
  * Disk typically has multi channels so data can be split/stripped across blks
  * access time $\uparrow$

#### Writing

* **Problem**
  * update a bitmap allocation block
    * finde the SSD block containing the target page
    * read all active pages in the SSD block into controller memory
    * update target page with new data in contorller memory
    * erase the SSD block  (high voltage to set all bits to 1)
    * write entire SSD block to drive
* **Impact**
  * SSD blocks wear out /limited erase cycle
    * FS blocks are frequently update
* **Solution Algorithm**
  * **Wear Leveling**
    * always write to new location
    * map < lofical FS #block :  < current SSD #block, page location > >
    * old version of logically overwritten pages == stale (garbage collection)
  * **Garbage Collection**
    * reclaiming stale pages and creating empty erased blocks
  * **RAID 5** (with parity check) striping across I/O channels to multi NAND chips

#### FS And SSDs

* Support FS
  * EXT4
  * btrfs
  * XFS
  * JFS
  * F2FS
* FS no need to take care of wear-leveling
  * done internally by SSD
  * BUT
    * TRIM op tell SSD which blks are no longer in use
      * otherwise a delete operation doesn't go to disk
* Flsh FS (F2FS/GFFS2) help reduce write amplification 
  * especially for small updates on FS metadata
* Other Typical HDD Feature
  * Defragmentaion
  * Disk scheduling algorithm

#### SSD Reliability



### File System Summary

> FAT == File Allocation Table
>
> FFS == Fast File System
>
> NTFS == New Technology File System
>
> MFT == Master File Table
>
> VFS == Virtual File System
>
> DS == Data Structure
> 
> overhead [磁盘开销]
> 
> Sequential Access [顺序存取]
>
> Heuristics [启示]
>
> Locality [局部化]
>
> Consecutive [持续的]
>
> Clustering [集群]

#### Goal

* Efficient directory use to translate file name $\to$  file number
* Sequential file access performance
* Efficient random access to any file block
* Efficient support for small files
  * overhead in terms of space and access time
* Support large files
* Efficient metadata storage and look up
* Crash recovery

#### FS Component

* Index structure
  * to locate each block of a file
* Free Space Management
* Locality Heuristics
* Crash Recovery

#### FS In Real World

* **FAT FS**

  * Link Allocation
    * notice that links are in the File Allocation Table, not in the block itself
    * blocks allocated in a linked structure table
  * Directory map file name to the first block of file
  * FAT store <the linkedlist of blocks of file, free blocks>
  * Limitation
    * Poor random access
    * Poor locality
    * Limited file metadata and access control

* **NTFS**

  * Each volume is a linear sequence of blocks and a MFT ( 4KB SIZE )

  * MFT
    * Sequence of 1KB records
    * one or more record per file/dir
      * similar to inodes but more flexible
    * record is a sequence of var_length <attribute header, value >
    * long attributes can be stored externally using a ptr kept in MFT record
  * Extent/Run allocation for consecutive blocks
  * Metadata/Attributes
    * key-value pairs
    * significant flexibility
      * allow implement extra feature like compression and diff file type

* **VFS**

  * an abstract file system interface
    * Separate abstraction and collections of file from speci implementation
    * sys calls implemented in terms of operations on the abstract file sys
      * vfs_open/close
  * abstraction layer is for OS itself
    * user-level programmers interacts with the file sys through the sys calls

* **EXT3**

  * Pros
    * recoverability
    * recoverable when significant data corruption ( other tree-based not )
  * Cons
    * lack modern FS feature like extent
    * metadata is in fixed well-known location and DS have redundancy

* **FFS**

  * Inode
    * store disk block index
    * store metadata
  * D_entry
    * store file name and inode number
  * Free Space Management
    * bitmap
  * Read Performance
    * locate related blocks in same cylinder group
    * locate inodes close to data blocks
  * Write Performance
    * block reallocation
      * reduce fragmentation
      * control aging
    * soft update
      * alternatice to journaling
      * ensures consistency without limiting performance
    * background fsck
      * for the rest of failure issues
  * Performance Observation
    * optimized for disk block clustering
    * using properties of the disk to inform file system layout
    * when memory is large enough
      * most reads that go to the disk ar the first read of the file
      * subsequent reads are satisfied in memeory by file buffer cache
      * writes are not well-clustered ?

* Popular FS List

  * Windows
    * FAT32
    * NTFS
  * MAC OS X
    * HFS+
  * BSD/Solaris
    * UFS
    * ZFS
  * Linux
    * EXT2/3/4
      * EXT2 borrowed heavily from FFS
      * updated on reliability and write optimization
    * ReiserFS
    * XFS/JFS
    * btrfs
    * zfs
  
* FS Comparison
  
  |                             |       FAT       |            FFS             |           NTFS           |
  | :-------------------------- | :-------------: | :------------------------: | :----------------------: |
  | Index Structure             |   LinkedList    |       Tree( inodes )       |      Tree( Extent )      |
  | Index Structure Granularity |      Block      |           Block            |          Extent          |
  | Free Space Management       |    FAT array    |           Bitmap           |          Bitmap          |
  | Locality Heuristics         | Defragmentation | Block group reserved space | Best fit defragmentation |
  
   









