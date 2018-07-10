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

maybe implement some kind of queue for the processes to go once executed?

if i stick with a queue for the buffer, then ide have to be able to recursibly pop data and recreate a new queu? that dosent seam right. mabye i do need to write an array.


ok. inorder to do anyof this, i first need to understand the data structure that i'm working with. what is the signal signaling and what is it waiting for? what is the lock and jey and where are they kept.
so i think 3 and an aditional array of 1, for now. would be expanded if this lab called for more writers.


new processes are randomly assigned to the ready queue. ready queue contains process objects that are either deginated as readers or writers. when conditions are possible, head of the queu is assigned to running. 
 
if a running process must wait for something(like another process to signal) the process goes to the waiting queue. i might make 2 waiting queus, one for readers and writers. when one recives the proper signal the top of the waiting queue goes back to the running queue.

//i think he said i don't have to worry about interupts/time. because each process is only running one instruction.

need to create a process object, and assigne them to be either readers or writers.
ill make two array objects for easy implementation.

never mind the queue. instructions say to run a random number generator to pick from. so i guess its just an array of 5 objects.

//i think in order to implement this, i need to call the thread function in a loop and then join at the end of the loop. loop should terminate eventually.

maybe do something with test and set. is that much?

maybe i can do two arrays with a reader and writer lock? maybe i do the writer lock, then check if there are any available readers, and if there are i allow it to procede, and if not i unlock and quit?
im still not sure how the semaphores get implemented here.
i think the binary semaphores are for the writers, and the counting semaphores are for the writers. maybe i use another binary semaphore to check if the writer is prevented by readers.

implement the writer first. seams simpler.

use std::ref() inorder to change something across threads.

**remember, join all threads before termnating the program. maybe wigure out some other wait at the end of the program to do this.
*/

//this needs to be implemented with a lock and key. i guess the lock is a array pointing to the reader/writer processes. key would be tru only when available? this is going to be difficult.

// i think lock the lock is automaticly set to true in the begining. the process picks it up, thats the key. when its done. it returns the key back to the lock.
//no. i think semaphore is for notification of its waiting. 
//each pocess should be able to lock by picking up a key




#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
using namespace std;

//swap function. swaps the boolean value of a lock(a) and key(b).
void swap(bool *a, bool *b){
	bool temp;
	std::ref(*a)=*b;
	std::ref(*b)=temp;
}

//*****i don't actually understand this one. implement it after i get binary working.
class countingSemaphore{
	private:
		bool isAvailable;
		int availableTime;
	public:
		//default construction is to set lock position to 0. i think over time this needs to increase somehow? probably goes back to 0 once it finishes running. i think.
		countingSemaphore(){
			isAvailable=true;
			availableTime=100;
		}
		bool getIsAvailable(){return isAvailable;}
		void setIsAvailable(bool x){isAvailable=x;}
		int getAvailableTime(){return availableTime;}
		void decreaseAvailableTime(){availableTime--;}
		// reset the availableTime to 100
		void resetAvailableTime(){availableTime=100;}

};

//simpler to implement. start here.
class binarySemaphore{
	private:
		bool isAvailable;

	public:
		binarySemaphore(){isAvailable=true;}
		bool getIsAvailable(){return isAvailable;}
		void setIsAvailable(bool x){isAvailable=x;}
};


class processObject{
	private:
		//process type of 0=reader. 1=writer
		int processType;
		bool hasKey;
	public:
		//constructor
		processObject(int x){
			if(x !=0 && x != 1){
				//error message. look up how to do this properly again.
				std::cout << "You have entered a process number that does not match the available range of 0 to 1.\n";
			}
			else{processType=x;}
			hasKey=false;
		}
		//not puting a set for processType, should not change once created.
		int getProcessType(){return processType;}
		bool getHasKey(){return hasKey;}
		void setHasKey(bool x){hasKey=x;}

};

//if there are any writers in the critical section before atempting to enter.
void checkCriticalSection(binarySemaphore writerLock){
	if(writerLock.getIsAvailable()==false){std::cout << "There are curently 0 writer processes in the critical section\n";}
	else{std::cout << "There is currently 1 writer process in the critical section.\n";}
};

//checks if there are any readers in the critical section before attempting to enter.
void checkCriticalSection(countingSemaphore readerLocks[]){
	int total=0;
	for(int i=0; i<3; i++){
		if(readerLocks[i].getIsAvailable()==false){total++;}
	}
	std::cout << "There are curently " << total << " reader processed in the critical section\n";
};

//checks bolth critical sections at once
void checkCriticalSection(countingSemaphore readerLocks[], binarySemaphore writerLock){
	checkCriticalSection(readerLocks);
	checkCriticalSection(writerLock);
};



//countingSemaphore subWait. i designed this so that it has to wait for a specific semaphore to get the signal......because signal shouls signal the one that has been waiting the longest.
//while availableTime >0, checks if isAvailable = true. if it is, it returns true. if not, calls decreaseAvailableTime. if available time =0, resets available time and returns falce.
bool subWait(countingSemaphore * passedSemaphore){
	while(passedSemaphore->getAvailableTime()>0){
		if(passedSemaphore->getIsAvailable()==true){return true;}
		passedSemaphore->decreaseAvailableTime();
	}
	passedSemaphore->resetAvailableTime();
	return false;
};

//wait for the counting semaphore. while availableTime > 0 and isAvailable is false. loop untill either isAvailable is true or availableTime =0. if availableTime =0. break. if isAvailable = true, allow set isAvailable to false and enter critical section.
//i guess i need a way to pick a semaphore to deincrement? like maybe the first one that = 100, and then have a sub function that counts doen untill zero, if neither is 100 then just break.

//countingSemaphore wait
bool wait( countingSemaphore passedSemaphores[]){
	//asume an array size of 2.
	for(int i=0; i<2; i++){
		if(passedSemaphores[i].getAvailableTime()==100){
			return subWait(& passedSemaphores[i]);
		}
	}
	return false;
};



//wait for the binarySemaphore. while the semaphores isAvailable is false, break to allow another process to enter. if it is true. change it to false, and allow the process to continue. 
bool wait( binarySemaphore * passedSemaphore){
	if(passedSemaphore->getIsAvailable()==false){return false;}
	else{
		passedSemaphore->setIsAvailable(false);
		return true;
	}
};

//signal for the binary semaphore, just changes the value of the one passed semaphore. 
void signal(binarySemaphore * passedSemaphore){
	passedSemaphore->setIsAvailable(true);
};

//signal for the countingSemaphore. compares the availableTime of both semaphores. changes isAvalable for the one that is lowest. (the one waiting the longest) 
//i might run itno a problem here. its an array of pointers os maybe it will change it, but i'm not entirely sure.
void signal(countingSemaphore passedSemaphore[]){
	//assumed that the array is of size 2.
	//im pretty sure that any process that is able to call signal will beable to signa; atleast once.  
	
	int oldest;
	if(passedSemaphore[0].getAvailableTime()<=passedSemaphore[1].getAvailableTime()){oldest=0;}
	else{oldest=1;}
	passedSemaphore[oldest].setIsAvailable(true);
	passedSemaphore[oldest].resetAvailableTime();
	return;
};
		
		

int main(){

	//if array of the available processes. if this array were any larger i would have probably written a function to assign assign them.
	processObject *avaiableProcesses[5]={new processObject(0), new processObject(0), new processObject(0), new processObject(1), new processObject(1)};

	//bool readerLocks[2]{true,true};
	//bool writerLock=true;

	//this if sor testing. must end eventually.
	int maxProcesses=20;
	int processCount=0;
	//random intilization
	srand(time(NULL));

	binarySemaphore writerLock;
	countingSemaphore *readerLock[2]={new countingSemaphore(), new countingSemaphore()};

	while(processCount<maxProcesses){
		switch(rand()%5){
			case 0:
				/* reader process goals:
					-break off into a thread. I think. doulbe check this. i don't know how you would do this project without that.
					-check if there are any other processes in the critical section.
					-report what those processes are. 
					-don't enter the critical section untill if there are already atlead 2 reader processes or 1 writer processes. wait untill apropriate singal recived.( either that the writer has left or that one of the readers hasent.)
					-lock a critical section reader with the processes key 9 i think, not sure of the implementation)  (maybe use swap)
					-do something with the shared buffer (optional)
					-unlock (maybe use swap)
					-signal (unlock and signal might need to be swapped)
					-join thread
				*/
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				if(wait(&writerLock)==false){break;}
				/*writer process goals:
					-break off into a thread. I think. doulbe check this. i don't know how you would do this project without that. no. says not to use any multithreading feature from a program matrix.
					-check if there are any other processes in the critical section.
					-report what those processes are. 
					-don't enter the critical section if there are any other processes in the critical section.
					-lock a critical section reader with the processes key ( i think, not sure of the implementation) ( maybe use swap)
					-do something with the shared buffer (optional)
					-unlock (maybe use swap)
					-signal (unlock and signal might need to be swapped)
					-join thread
				*/
				break;
			case 4:
				break;
		}
		processCount++;
	}


	return 0;
}
