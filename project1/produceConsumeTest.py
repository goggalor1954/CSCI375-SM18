#James Roesemann
#CSCI 375
#Operating Systems
#Project 1
#due 6/19/2018
#written in python

"""
This program is a implmentation of the producer consumer problem.
the user will be asked to input an integer to determine the buffer size and the counter limit.
the program will then call two threads, producer amd consumer
producer will add random integers to the buffer, so long as the buffer is not full. when added it will decrement the produceLimit
consumer will remove interger from the buffer, in order, so long as the buffer is not empty. when removed it will decrement consumeLimit
when produceLimit =0 the producer thread will end
when consumeLimt=0 the consumer limit will end

i'm still learning python and haven't figured out how to pass specfic variables to a thread. I relize it's a bit sloppy but i'm going to use a few global variables in the threads to demonstrate the producer/consumer problem.
"""

import time
import random
import threading


#producer is the producer thread. recives no input, initlized with .start()
class producer( threading.Thread):
	def run(self):
		global bufferList
		global produceLimit
		#while produceLimit is > 0, aquire the lock from the condition object bufferLock
		while produceLimit>0:
			bufferLock.acquire()
			#if bufferList=bufferSize, then the buffer is full. wait untill notified by the consumer
			if len(bufferList)==bufferSize:
				bufferLock.wait()
			#generate a random integer between 0 and 1000, append it to the end of the buffer.
			nextProduced=random.randint(0,1000)
			bufferList.append(nextProduced)
			print 'Produced:', nextProduced
			#increment produceLimit, notify the consumer that producer has produced to the buffer, release bufferLock
			produceLimit-=1
			bufferLock.notify()
			bufferLock.release()
		
#consumer is the consumer thread. recives no input. initlized with .start()
class consumer(threading.Thread):
	def run(self):
		global bufferList
		global consumeLimit
		#while consumerLimit is > than zero, acuire the lock from the condition object bufferLock
		while consumeLimit>0:
			bufferLock.acquire()
			#if bufferList is empty,  wait untill notified by producer
			if not bufferList:
				bufferLock.wait()
			#now that bufferList has something in it, consume the first element of the list. decrement consumeLimit by 1
			nextConsumed = bufferList.pop(0)
			print 'Consumed:', nextConsumed
			#deincrement consumeLimit, notify producer that consumer as consumed from bufferlist and release bufferLock 
			consumeLimit-=1
			bufferLock.notify()
			bufferLock.release()

#begining of program
random.seed()
bufferLock=threading.Condition()
bufferSize=input('What would you like the buffer size to be? enter a number: ')
produceLimit=input('What would you like the counter Limit to be? enter a number: ')
consumeLimit=produceLimit
bufferList=[]

producer().start()
consumer().start()
