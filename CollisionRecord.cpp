#include "CollisionRecord.h"

void CollisionRecord::AddRecord(uint32_t number) {
	//履歴に追加
	history_.push_back(number);
}

bool CollisionRecord::CheckHistory(uint32_t number) { 
	//履歴に一致があればtrueを、なければfalseを返す
	return std::find(history_.begin(), history_.end(), number) != history_.end();
}

void CollisionRecord::Clear() {
	//履歴を抹消
	history_.clear();
}
