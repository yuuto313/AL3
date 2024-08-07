#pragma once
#include <functional>

/// <summary>
/// 時限発動
/// </summary>
class TimedCall {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="f"></param>
	/// <param name="time"></param>
	TimedCall(std::function<void>f,uint32_t time);

	/// <summary>
	/// /更新
	/// </summary>
	void Update();

	/// <summary>
	/// 完了ならtrueを返す
	/// </summary>
	/// <returns></returns>
	bool IsFinished() { return isComplite; }

private:
	// コールバック
	std::function<void> f_;

	// 残り時間
	uint32_t time_;

	// 完了フラグ
	bool isComplite = false;
};
