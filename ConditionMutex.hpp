/*Date:		2017/12/10 18:01:11
 *Author:	Jianlin LI
 */
#ifndef CONDITIONMUTEX_
#define CONDITIONMUTEX_

#include "SuperMutex.hpp"

namespace multiTools {
class ConditionMutex: NoCopy {
 public:
	explicit ConditionMutex(SuperMutex& mutex):smutex_(mutex) {
		pthread_cond_init(&pcond_, NULL);
	}
	~ConditionMutex() {
		pthread_cond_destroy(&pcond_);
	}
	void Wait() {
		pthread_cond_wait(&pcond_, smutex_.GetSuperMutex());
	}
	void Notify() {
		pthread_cond_signal(&pcond_);
	}
	void NotifyAll() {
		pthread_cond_broadcast(&pcond_);
	}
 private:
	SuperMutex& smutex_;
	pthread_cond_t pcond_;
}; //ConditionMutex
} //multiTools

#endif //ConditionMutex
