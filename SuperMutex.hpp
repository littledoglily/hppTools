/*Date:		2017/12/08 00:01:11
 *Author:	Jianlin LI
 */
#ifndef SUPERMUTEX_
#define SUPERMUTEX_

#include <pthread.h>
#include "NoCopy.hpp"

namespace multiTools {
class SuperMutex: NoCopy {
 public:
	SuperMutex() {
		pthread_mutex_init(&mutex_, NULL);
	}
	~SuperMutex() {
		pthread_mutex_destroy(&mutex_);
	}
	pthread_mutex_t* GetSuperMutex() {
		return &mutex_;
	}
 private:
	void Lock() {
		pthread_mutex_lock(&mutex_);
	}
	void UnLock() {
		pthread_mutex_unlock(&mutex_);
	}
	friend class SuperMutexFriend; 
	pthread_mutex_t mutex_;	
}; // SuperMutex

class SuperMutexFriend: NoCopy {
 public:
	explicit SuperMutexFriend(SuperMutex* mutex):Smutex_(mutex) {
		Smutex_->Lock();
	}
	~SuperMutexFriend() {
		Smutex_->UnLock();
	}
 private:
	SuperMutex* Smutex_;
}; //SuperMutexFriend
} //multiTools
#endif //SuperMutex
