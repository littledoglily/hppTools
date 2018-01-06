/*Date:		2017/12/08 00:01:11
 *Author:	Jianlin LI
 */
#ifndef BLOCKQUEUE_
#define BLOCKQUEUE_
#include "NoCopy.hpp"
#include "ConditionMutex.hpp"
namespace multiTools {
template <class T>
class BlockQueue: NoCopy {
 public:
	explicit BlockQueue(size_t length):Qlength_(length), Head_(0), Tail_(0), Qsize_(0), Smutex_(), Fullmutex_(Smutex_), Nullmutex_(Smutex_)  {
		Qarray_ = new (std::nothrow)T[Qlength_];
	}
	~BlockQueue() {
		if (NULL != Qarray_) {
			delete[] Qarray_;
			Qarray_ = NULL;
		}
	}
	void Push(const T& item) {
		SuperMutexFriend localLock(&Smutex_);
		while (((Head_ + 1) % Qlength_) == Tail_) {
			Fullmutex_.Wait();
		}
		Qsize_ += 1;
		Qarray_[Head_] = item;
		Head_ = (Head_ + 1) % Qlength_;
		Nullmutex_.Notify();	
	}
	T Pop() {
		SuperMutexFriend localLock(&Smutex_);
		while (Tail_ == Head_) {
			Nullmutex_.Wait();
		}
		Qsize_ -= 1;
		T var = Qarray_[Tail_];
		Tail_ = (Tail_ + 1) % Qlength_;
		Fullmutex_.Notify();
		return var; 
	}
	size_t Length() const {
		SuperMutexFriend localLock(&Smutex_);
		return Qsize_;
	}
	bool IsEmpty() const {
		return Length() == 0;
	}
 private:
	size_t Qlength_;
	size_t Head_;
	size_t Tail_;
	size_t Qsize_;
	T* Qarray_;
	mutable SuperMutex Smutex_;
	ConditionMutex Fullmutex_;
	ConditionMutex Nullmutex_;
}; //BlockQueue
} //multiTools
#endif //BlockQueue
