#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "DebugCamera.h"
//#include "MapChipField.h"
class MapChipField;

#pragma once
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:

	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Init(Model* model,ViewProjection* viewProjection,const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 移動入力
	/// </summary>
	void MovementInput();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};
	
	//加速度
	static inline const float kAcceleration = 0.05f;
	//速度減衰率
	static inline const float kAttenuation = 0.15f;
	//速度制限
	static inline const float kLimitRunSpeed = 0.3f;

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	//プレイヤーの向き	
	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

	//旋回時間＜秒＞
	static inline const float kTimeTurn = 0.3f;

	//設置状態フラグ
	bool onGround_ = true;

	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.2f;
	// 最大重力加速度（下方向）
	static inline const float kLimitFallSpeed = 50.0f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 0.625f;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;

	//マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceilingCollision = false;//天井衝突フラグ
		bool landing = false;//着地フラグ
		bool contactWall = false;//壁接触フラグ
		Vector3 amountOfMovement = {}; // 移動量
	};

	//角
	enum Corner {
		kRightBottom,//右下
		kLeftBottom,//左下
		kRightTop,//右上
		kLeftTop,//左上
		kNumCorner//要素数
	};

	//余白
	static inline const float kBlank = 0.1f;

public:
	/// <summary>
	/// 自キャラのWorldTransformを取得する
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	/// <summary>
	/// 自キャラの速度を取得するためのゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() const { return velocity_; }
	/// <summary>
	/// マップチップのセッター
	/// </summary>
	/// <param name="mapChipField"></param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	/// <summary>
	/// マップ衝突判定
	/// </summary>
	/// <param name="info"></param>
	void IsCollision(CollisionMapInfo& info);
	/// <summary>
	/// 指定した角の座標計算
	/// </summary>
	/// <param name="center"></param>
	/// <param name="corner"></param>
	/// <returns></returns>
	Vector3 CornerPosition(const Vector3& center, Corner corner);
	/// <summary>
	/// 上方向の当たり判定
	/// </summary>
	/// <param name="info"></param>
	/// <returns></returns>
	void IsCollisionUp(CollisionMapInfo& info);
	/// <summary>
	/// 判定結果を反映して移動させる
	/// </summary>
	/// <param name="info"></param>
	void Reflection(const CollisionMapInfo& info);
	/// <summary>
	/// 天井に当たってる場合の処理
	/// </summary>
	/// <param name="info"></param>
	void HitsTheCeiling(const CollisionMapInfo& info);

};
