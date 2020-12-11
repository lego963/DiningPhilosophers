/*
  A VERY simplified semaphore class for use in a std UNIX
  environment.  See the text for instructions on how to use
  this class.  Copyright (c) 2002 J. S. Gray

  Exit codes for class operations:

  1 - Semaphore allocation failure   2 - Unable remove semaphore
  3 - Unable to LOCK semaphore       4 - Unable to UNLOCK semaphore
  5 - Failure on wait for ZERO       6 - Unable to assign value
  7 - Unable to return value
*/

#ifndef CustomSemaphore_h
#define CustomSemaphore_h
#define _GNU_SOURCE

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class CustomSemaphore
{
public:
	CustomSemaphore();	 // Constructor
	~CustomSemaphore();	 // Destructor - remove semaphore
	int P();			 // LOCK (decrement semaphore)
	void V();			 // UNLOCK (increment semaphore)
	int Z();			 // WAIT while semaphore is NOT 0
	void Put(const int); // Assign a value to semaphore
	int Get();			 // Return value of the semaphore
private:
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
	// definition in <sys/sem.h>
#else
	union semun
	{							   // We define:
		int val;				   // value  for SETVAL
		struct semid_ds *buf;	   // buffer for IPC_STAT, IPC_SET
		unsigned short int *array; // array  for GETALL, SETALL
		struct seminfo *__buf;	   // buffer for IPC_INFO
	};
#endif
	/*
	User semaphore template for semop system calls.
	*/
	struct sembuf
	{
		unsigned short int sem_num; // semaphore #: 0 = first
		short int sem_op;			// semaphore operation
		short int sem_flg;			// operation flags
	};

	union semun arg;				  // For semctl call
	struct sembuf zero, lock, unlock; // hoo ha's for P,V & Z operations
	int semid;						  // ID of semaphore
	pid_t my_pid;					  // PID of creator
};
#endif