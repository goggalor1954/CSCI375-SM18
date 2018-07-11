//james roesemann
//CSCi 375
//Project #2
//Due 7/10/18

//!!!This program does not work!!!!



//Despite my efforts, i wwas not able to coreclty implement this program.
//under this implementation, readers and writers are frequently in the critical section together.
//I never got around to implementing any shared buffer
//I think i'm doing something wrong with the semaphores? but i'm not sure.
//all in all i'm very disapointed.


/*
program goals:
-up to two reader processes can be inside the critical section without any writer processes.
-inorder for a writer process to go into the critical section, it must first check wether or there is a process int the critical section.

-the critical section reads shared data buffer form the reader. updates shared data buffer for the wrriter process.
i must implemnt any shared data for readers/writers but you must clearly specify the following in output.

    -when reader or writer enters the critical section, it must report wheteher there are any readers/reiters other than itself.
    -(optional) may print out the data you read write when implementing the real buffer.
    -print out a "panic message" when the semaphore rules are not observed.


You need to keep track of program counter of each process to resume at the right place once it will be chosen to run by keeping global counter variable per process. (**i don;t entirly understand this***).


*/





#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
using namespace std;




class countingSemaphore{
	private:
		bool keyIsAvailable;
		int availableTime;
	public:
		//default construction is to set lock position to 0. i think over time this needs to increase somehow? probably goes back to 0 once it finishes running. i think.
		countingSemaphore(){
			keyIsAvailable=true;
			availableTime=100;
		}
		bool getKeyIsAvailable(){return keyIsAvailable;}
		void setKeyIsAvailable(bool x){keyIsAvailable=x;}
		int getAvailableTime(){return availableTime;}
		void decreaseAvailableTime(){availableTime--;}
		// reset the availableTime to 100
		void resetAvailableTime(){availableTime=100;}

};

//simpler to implement. start here.
class binarySemaphore{
	private:
		bool keyIsAvailable;

	public:
		binarySemaphore(){keyIsAvailable=true;}
		bool getKeyIsAvailable(){return keyIsAvailable;}
		void setKeyIsAvailable(bool x){keyIsAvailable=x;}
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


//reports if any writers in the critical section
void checkCriticalSection(bool writerKey);

//reports if any readers are in the critical section
void checkCriticalSection(bool readerKeys[]);

//reports if any writers or readers are in the critical section
void checkCriticalSection(bool readerKeys[], bool writerKey);

//subwait is used to implement the counting semaphore. while availbleTime>0, checks if keyIsAvailable from the passedSemaphore = true. if it is returns true. if time has expired it resets availbleTime and returns false.
bool subWait(countingSemaphore * passedSemaphore);

//countingSemaphore wait. checks if there any available counting semaphores(where available time =100). if not it returns false to allow another process to enter the critical section. if there are it calls subWait for that semaphore. returns the result.
bool wait( countingSemaphore passedSemaphores[]);

//binarySemaphore wait. while the semaphores keyIsAvailable is false, break to allow another process to enter critical section. if it is true. change it to false, and allow the process to continue. 
bool wait( binarySemaphore * passedSemaphore);

//signal for the binarySemaphore, changes the value of keyIsAvailable
void signal(binarySemaphore * passedSemaphore);

//signal for the countingSemaphore. compares the availableTime of both semaphores. changes the value of keyIsAvailable for the one that is lowest. (the one that has been waiting the longest) 
//i might run itno a problem here. its an array of pointers os maybe it will change it, but i'm not entirely sure.
void signal(countingSemaphore passedSemaphore[]);

//checks to see if there are any writers in the critical section(for reader processes) if there are any in the critical section, return true. true causes the case to break. does not wait and does not change any variables. 
//i feel like this should be modified. maybe allow it to wait endlessly untill it can enter? probably a bad idea
bool checkForWriters(binarySemaphore * writerSemaphores);

//checks to see if there are any reader in the critical section(for writer processes) if there are any in the critical section, return true. true causes the case to break. does not wait and does not change any variables
bool checkForReaders(countingSemaphore  readerSemaphores[]);

//passed the readerKeys array and processID(the current case number).checks which readerKey is available to be locked and locks it. 
//i don't like how i implemented this. i have to check which key is available first before entering swaping it. i know that can lead to some problems but i don't know how to get around it. may need to rewrite this.
void lock(processObject * processId, bool readerKeys[]);

//passed the writerKey and processId( the current case number). locks the binarySemaphore
void lock(bool &writerKey,processObject * processId);

//passed the readerKeys array and processID(the current case number). unlocks the first available spot in the array
void unlock(processObject * processId, bool readerKeys[]);

//passed the writerKey. unlocks it
void unlock(bool &writerKey,processObject * processId);





int main(){

	//if array of the available processes. if this array were any larger i would have probably written a function to assign assign them.
	processObject *avaiableProcesses[5]={new processObject(0), new processObject(0), new processObject(0), new processObject(1), new processObject(1)};

	bool *readerKeys[2]{new bool(true), new bool(true)};
	bool writerKey=true;
	binarySemaphore writerLock;
	countingSemaphore *readerLock[2]={new countingSemaphore(), new countingSemaphore()};

	//random intilization
	srand(time(NULL));

	//for testing purposes this programs end after 20 "processes" have been called.
	int maxProcesses=20;
	int processCount=0;

	while(processCount<maxProcesses){
		//case 0-2 represent reader processes. csse 3-4 represent writer processes.
		switch(rand()%5){
			case 0:
				//wait untill you can enter the critical section. if no available semaphores or time expires, break
				if(wait(*readerLock)==false){break;}
				//check to see if thhere is a writer process in the critical section. if there is, signal the countingSemaphres, break and return to ready state to allow another process to enter the critical section.
				if(checkForWriters(& writerLock)==true){
					signal(*readerLock);
					break;
				}
				//lock the critical section. position on availableProcesses array is the came ase case number
				lock(avaiableProcesses[0], *readerKeys);
				//report current processes report processes curently in critical section
				std::cout << "reader case 0: entering critical section\n";
				checkCriticalSection(*readerKeys, writerKey);
				//***critical section***
				while(avaiableProcesses[0]->getHasKey()==true){
					//optional, do somthing with a shared buffer here
					//***end critical section***
					//unlock critical section
					unlock(avaiableProcesses[0], *readerKeys);
				}
				//signal semaphore
				signal(*readerLock);
				break;
			case 1:
				//wait untill you can enter the critical section. if no available semaphores or time expires, break
				if(wait(*readerLock)==false){break;}
				//check to see if thhere is a writer process in the critical section. if there is, signal the countingSemaphres, break and return to ready state to allow another process to enter the critical section.
				if(checkForWriters(& writerLock)==true){
					signal(*readerLock);
					break;
				}
				//lock the critical section. position on availableProcesses array is the came ase case number
				lock(avaiableProcesses[1], *readerKeys);
				//report current processes report processes curently in critical section
				std::cout << "reader case 1: entering critical section\n";
				checkCriticalSection(*readerKeys, writerKey);
				//***critical section***
				while(avaiableProcesses[1]->getHasKey()==true){
					//optional, do somthing with a shared buffer here
					//***end critical section***
					//unlock critical section
					unlock(avaiableProcesses[1], *readerKeys);
				}
				//signal semaphore
				signal(*readerLock);
				break;
			case 2:
				//wait untill you can enter the critical section. if no available semaphores or time expires, break
				if(wait(*readerLock)==false){break;}
				//check to see if thhere is a writer process in the critical section. if there is, signal the countingSemaphres, break and return to ready state to allow another process to enter the critical section.
				if(checkForWriters(& writerLock)==true){
					signal(*readerLock);
					break;
				}
				//lock the critical section. position on availableProcesses array is the came ase case number
				lock(avaiableProcesses[2], *readerKeys);
				//report current processes report processes curently in critical section
				std::cout << "reader case 2: entering critical section\n";
				checkCriticalSection(*readerKeys, writerKey);
				//***critical section***
				while(avaiableProcesses[2]->getHasKey()==true){
					//optional, do somthing with a shared buffer here
					//***end critical section***
					//unlock critical section
					unlock(avaiableProcesses[2], *readerKeys);
				}
				//signal semaphore
				signal(*readerLock);
				break;
			case 3:
				//check to see if there are any readers in the critical section. if so, break. if not continue.
				if(checkForWriters( &writerLock)==true){break;}
				//if another writer is in the critical section, break. if not, set keyIsAvailable to false and continue
				if(wait(&writerLock)==false){break;}

				//lock the critical section
				lock(writerKey, avaiableProcesses[3]); 
				//report on what is in the critical section before entering.
				std::cout << "writer case 3: entering critical section\n";
				checkCriticalSection(*readerKeys, writerKey);
				///***critical section ***
				while(avaiableProcesses[3]->getHasKey()==true){
				//do something with the buffer(optional)
				

				//***end critical section***
				//unlock
					unlock(writerKey, avaiableProcesses[3]); 
				//signal
				signal(&writerLock);
				}
				break;
			case 4:
				//check to see if there are any readers in the critical section. if so, break. if not continue.
				if(checkForWriters( &writerLock)==true){break;}
				//if another writer is in the critical section, break. if not, set keyIsAvailable to false and continue
				if(wait(&writerLock)==false){break;}

				//lock the critical section
				lock(writerKey, avaiableProcesses[4]); 
				//report on what is in the critical section before entering.
				std::cout << "writer case 4: entering critical section\n";
				checkCriticalSection(*readerKeys, writerKey);
				///***critical section ***
				while(avaiableProcesses[4]->getHasKey()==true){
				//do something with the buffer(optional)
				//***end critical section***
				//unlock
					unlock(writerKey, avaiableProcesses[4]); 
				}
				//signal
				signal(&writerLock);

				break;
		}
		processCount++;
	}


	return 0;
}





//checks if there are any writers in the critical section before atempting to enter.
void checkCriticalSection(bool writerKey){
	if(writerKey==false){std::cout << "0 writer processes in the critical section\n";}
	else{std::cout << "1 writer process in the critical section.\n";}
};

//checks if there are any readers in the critical section before attempting to enter.
void checkCriticalSection(bool readerKeys[]){
	int total=0;
	for(int i=0; i<2; i++){
		if(readerKeys[i]==false){total++;}
	}
	std::cout << total << " reader processes in the critical section\n";
};

//checks bolth if there are any writes or readers in the critical section before attemptin to enter.
void checkCriticalSection(bool readerKeys[], bool writerKey){
	checkCriticalSection(readerKeys);
	checkCriticalSection(writerKey);
};



//subwait is used to implement the counting semaphore. while availbleTime>0, checks if keyIsAvailable from the passedSemaphore = true. if it is returns true. if time has expired it resets availbleTime and returns false.
bool subWait(countingSemaphore * passedSemaphore){
	while(passedSemaphore->getAvailableTime()>0){
		if(passedSemaphore->getKeyIsAvailable()==true){return true;}
		passedSemaphore->decreaseAvailableTime();
	}
	passedSemaphore->resetAvailableTime();
	return false;
};



//countingSemaphore wait. checks if there any available counting semaphores(where available time =100). if not it returns false to allow another process to enter the critical section. if there are it calls subWait for that semaphore. returns the result.
bool wait( countingSemaphore passedSemaphores[]){
	//asume an array size of 2.
	for(int i=0; i<2; i++){
		if(passedSemaphores[i].getAvailableTime()==100){
			return subWait(& passedSemaphores[i]);
		}
	}
	return false;
};





//binarySemaphore wait. while the semaphores keyIsAvailable is false, break to allow another process to enter critical section. if it is true. change it to false, and allow the process to continue. 
bool wait( binarySemaphore * passedSemaphore){
	if(passedSemaphore->getKeyIsAvailable()==false){return false;}
	else{
		passedSemaphore->setKeyIsAvailable(false);
		return true;
	}
};

//signal for the binarySemaphore, changes the value of keyIsAvailable
void signal(binarySemaphore * passedSemaphore){
	passedSemaphore->setKeyIsAvailable(true);
};

//signal for the countingSemaphore. compares the availableTime of both semaphores. changes the value of keyIsAvailable for the one that is lowest. (the one that has been waiting the longest) 
//i might run itno a problem here. its an array of pointers os maybe it will change it, but i'm not entirely sure.
void signal(countingSemaphore passedSemaphore[]){
	//assumed that the array is of size 2.
	//im pretty sure that any process that is able to call signal will beable to signa; atleast once.  
	
	int oldest;
	if(passedSemaphore[0].getAvailableTime()<=passedSemaphore[1].getAvailableTime()){oldest=0;}
	else{oldest=1;}
	passedSemaphore[oldest].setKeyIsAvailable(true);
	passedSemaphore[oldest].resetAvailableTime();
	return;
};

//checks to see if there are any readers in the critical section(for writer processes) if there are any in the critical section, return true. true causes the case to break. does not wait and does not change any variables
bool checkForReaders(countingSemaphore  readerSemaphores[]){
	for(int i=0; i<2; i++){
		if(readerSemaphores[i].getKeyIsAvailable()==false){return true;}
	}
	return false;
};


//checks to see if there are any writers in the critical section(for reader processes) if there are any in the critical section, return true. true causes the case to break. does not wait and does not change any variables
bool checkForWriters(binarySemaphore * writerSemaphores){
	if(writerSemaphores->getKeyIsAvailable()==false){return true;}
	else{return false;}
};

//passed the readerKeys array and the current processObject.checks which readerKey is available to be locked and locks it. 
//i don't like how i implemented this. i have to check which key is available first before entering swaping it. i know that can lead to some problems but i don't know how to get around it. may need to rewrite this.
void lock(processObject * processId, bool readerKeys[]){
	for(int i=0; i<2; i++){
		if(readerKeys[i]==true){
			bool temp;
			temp=readerKeys[i];
			readerKeys[i]=processId->getHasKey();
			processId->setHasKey(temp);
			return;
		}
	}
};

//passed the writerKey and processObject. locks the binarySemaphore
void lock(bool & writerKey,processObject * processId){
	bool temp;
	temp=writerKey;
	writerKey=processId->getHasKey();
	processId->setHasKey(temp);
};

//passed the readerKeys array and processObject. unlocks the first available spot in the array
void unlock(processObject * processId, bool readerKeys[]){
	for(int i=0; i<2; i++){
		if(readerKeys[i]==false){
			bool temp;
			temp=readerKeys[i];
			readerKeys[i]=processId->getHasKey();
			processId->setHasKey(temp);
			return;
		}
	}

};

//passed the writerKey and processObject. unlocks it
void unlock(bool &writerKey,processObject *processId){
	bool temp;
	temp=writerKey;
	writerKey=processId->getHasKey();
	processId->setHasKey(temp);
};
