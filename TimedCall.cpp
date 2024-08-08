#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f, uint32_t time){
	callBack_ = f;	
	time_ = time;
}

void TimedCall::Update() { 
	if (IsFinished()) {
		return;
	}

	time_--;
	if (time_ <= 0) {
		isComplite = true;
		//コールバック関数呼び出し
		callBack_();
	}
}
