//James Roesemann
//CSCI 375
//Operating Systems
//Project 1
//due 6/19/2018
//re written in c++ inorder to write project 2


#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
//#include <pthread.h>

using namespace std;
/*
class producerThread{
	public:
		//need to figureout how to pass a variable to a thread
		queue<int> bufferList;
		int produceLimit;
		//deafult configuration i think. write run function, then call run function in default constructor.
		producerThread(queue<int> const & bufferL, int const & produceL){
			bufferList=std::ref(bufferL);
			produceLimit=std::ref(produceL);
		}
		void run(){
			while(producerLimit>0){
				mtx.lock()
		
};
*/

//must be defigned outside main
std::mutex mtx;
std::condition_variable cv;

void producer(int bufferSize, int produceLimit, queue<int> & bufferQueue);
void consumer( int bufferSize, int consumeLimit,  queue<int> & bufferQueue);
//thread producer(int bufferSize, int produceLimit, queue<int> & bufferQueue, mutex & mtx, condition_variable & cv);

int main(){
	
	//mutex, don;t think were allowed to use this in project 2. not sure how to go about that.


	//test. change size later depending on input
	int bufferSize=5;
	int produceLimit=15;
	int consumeLimit=produceLimit;
	//initlize random
	srand(time(NULL));
	queue<int> bufferQueue;

	cout << "Test\n";
	std::thread uno (producer, bufferSize, produceLimit,  std::ref(bufferQueue));
	std::thread dos (consumer, bufferSize, consumeLimit,  std::ref(bufferQueue));
	uno.join();
	dos.join();
	return 0;
}


void producer(int bufferSize, int produceLimit, queue<int> & bufferQueue){
	int randInt;
	while(produceLimit>0){
		randInt=rand()%100;			
		std::unique_lock<std::mutex> lck(mtx);
		//only do this if the size of bufferQueue is less than bufferSize
		if(bufferQueue.size()==bufferSize){
			cv.wait(lck);
		}
		std::cout << "produced: " << randInt << std::endl;
		bufferQueue.push(randInt);
		produceLimit--;
		cv.notify_all();
	}
}
void consumer( int bufferSize, int consumeLimit,  queue<int> & bufferQueue){
	int nextConsumed;
	while(consumeLimit>0){
		std::unique_lock<std::mutex> lck(mtx);
		if(bufferQueue.size()==0){
			cv.wait(lck);
		}
		nextConsumed=bufferQueue.front();
		bufferQueue.pop();
		std::cout << "consumed: " << nextConsumed << std::endl;
		consumeLimit--;
		cv.notify_all();
	}
}



///I think it works!!! hazaaah!
