#include "TimedCall.h"

TimedCall::TimedCall(std::function<void> f, uint32_t time)
    : f(f_)

void TimedCall::Update() { 
	if (IsFinished) {
		return;
	}

	time_--;
	if (time_ <= 0) {
		isComplite = true;
		//コールバック関数呼び出し

	}
}
