#include <variant>
#include <map>
#include <string>
#include <fstream>
#include <Vector3.h>
#include <json.hpp>

using json =nlohmann::json;
#pragma once

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables {
public:
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	static GlobalVariables* GetInstance();

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);

	//値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3 value);


private:
	GlobalVariables() = default;
	~GlobalVariables() = default;

	//コピーコンストラクタとコピー代入演算子を削除
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

	//項目
	struct Item {
		//項目の値
		std::variant<int32_t, float, Vector3> value;
	};

	//グループ
	struct Group {
		std::map<std::string, Item> items;
	};

	//全データ
	std::map<std::string, Group> datas_;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoqryPath = "Resources/GlobalVariables/";
};
