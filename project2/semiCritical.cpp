//james roesemann
//CSCi 375
//Project #2
//Due 7/10/18


/* what the fuck do i have to do to make this work.

sub project1-
    implement binary and counting semaphores
sub project2-
    implement swap operation.

create 3 readers and 2 writers for a total of 5 processes.

-up to two reader processes can be inside the critical section without any wroter processes.
-inorder for a writer process to go into the critical section, it must first check wether or there is a process int the critical section. (i assume it can't enter if there is one there)

-the critical section reads shared data buffer for the reader. updates shared data buffer for the wrriter process.
i must implemnt any shared data for readers/writers but you must clearly specify the following in output.

    -when reader or writer enters the critical section, it must report wheteher there are any readers/reiters other than itself.
    -(optional) may print out the data you read write when implementing the real buffer.
    -print out a "panic message" when the semaphore rules are not observed.

in main, run a random number generator to choose processes ro execute. 
chosen process starts/resumes and after one execution it returns, waits in ready queue.

can be implemented using a switch statment, do not use multi-threading or mutex feature from the programming languages.
each **process** is one switch statment. and is returned after each instruction.
You need to keep track of program counter of each process to resume at the right place once it will be chosen to run by keeping global counter variable per process. (**i don;t entirly understand this***).


*** i think that what i should do first is to implemtn the old rpoject in c++ first. then i'll be able to write this modified version for project 2.***


*/
