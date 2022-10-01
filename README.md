# OSPrinciples-Assignment4

**FCFS Reader/Writer PROGRAMMING**
 
150 points 
 
Due: Friday, Nov 12, 2021, 11:59pm 
 
This assignment consists of writing C/C++ programs for implementing semaphore-based and  monitor-based  solutions  for  the  FCFS  version  of  the  reader-writer problem.  The  FCFS version works as follows: readers and writers access the shared file in the FCFS order; if multiple readers arrive at the system back-to-back, these readers can read the file concurrently when it is their turn to read. It can be assumed that threads in every queue associated with a semaphore, or a monitor or a conditional variable are managed with the FCFS policy. 
 
The  goal  of  this  assignment  is  to  practice  developing  semaphores-based  and  monitor-based  applications.  Furthermore,  practice  some  system  calls  and  process  management  by  the operating system.  
 
**Details**
 
You  need  to  Develop  C  or  C++  programs  to  implement  your  semaphore-based  and Monitor solutions to the FCFS version of the reader-writer problem, using the POSIX semaphore functions  and  conditional  Variables  (if  needed).  Name  your  programs  as “Assign4_sem.c”  and “Assign4_mon.c”  to  refer  to  the  semaphore  solution  and  monitor  solution,  respectively.  The programs should be complied correctly on the CSE machine. Suppose the name of the compiled executable  files  are  “assign4sem_exe”  and  “assign4mon_exe”,  respectively.  Your  programs should have the following functionalities: 
 
1. It accepts 10 arguments from the command-line, denoted as b1, b2, ..., b10. Each bi is either  0  or  1.  If  bi  is  0,  it  means  the  ith  arriving  thread  is  a  reader;  otherwise,  it  is  a writer. For example, the programs may be launched as: 

 _cse ...> assign4sem_exe 0 1 0 0 1 0 0 0 0 1_
  
 _cse ..> assign4mon_exe 0 1 0 0 1 0 0 0 0 1_
 
meaning  your  programs  should  solve  the  FCFS  version  of  the  reader-writer  problems for the situation that 10 reader/writer threads (i.e., the 2nd, 5th and 10th ones are writers while others are readers for this example) arrive at the system (i.e., start running) in the specified order. 
 
2. After  accepting  the  10  arguments  from  the  command-line,  the  program  should create  10  reader/writer  threads  and  start  them  in  the  specified  order.  Once created, these threads run concurrently. Each reader thread performs one reading operation and each writer thread performs one writing operating.  
 
Performing a reading or a writing operation is simulated with making the thread sleeps  for  1  millisec.  When  a  reader  (or  writer)  starts  or  ends  reading  (or writing),  a  message  should  be  printed.  For  the  example  shown  in  (1),  the message  should  be  like  _“Reader  1  starts reading”_, ..., _“Reader 1 ends reading”_, ..., _“Writer 2 starts writing”_, ..., _“Writer 2 ends writing”_, .... 
 
The  program  will  need  to  use  pthreads  to  create  threads,  and  sleep  system  call  to simulate time elapse, in addition to the semaphore functions for the semaphore solution, and the use of conditional variables and semaphores to create the monitor. 
