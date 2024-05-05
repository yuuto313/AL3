#pragma once
#include <cstdint>
#include "vector"
#include "Vector3.h"


enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};


class MapChipField {
	public:

	//マップチップデータ読み込みに備えて読み込み済みデータをリセットする関数
	void ResetMapChipData();

	//読み込み
	void LoadMapChipCsv(const std::string& filePath);

	//マップチップ種別の取得
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	//マップチップ座標の取得
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }; 
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; };


	private:

	//１ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

};
