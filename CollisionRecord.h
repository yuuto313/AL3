#pragma once
#include <vector>

/// <summary>
/// 接触記録
/// </summary>
class CollisionRecord {
public:
	/// <summary>
	/// 履歴追加
	/// </summary>
	/// <param name="number"></param>
	void AddRecord(uint32_t number);

	/// <summary>
	/// 履歴チェック
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	bool CheckHistory(uint32_t number);

	/// <summary>
	/// 履歴抹消
	/// </summary>
	void Clear();

private:
	//履歴
	std::vector<uint32_t> history_;
};
