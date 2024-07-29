#include "DeathParticles.h"
#include <cassert>
void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position) {
	//メンバ変数初期化
	assert(model);
	model_ = model;
	viewProjevtion_ = viewProjection;

	//ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	//終了なら何もしない
	if (isFinished_) {
		return;
	}


	//移動処理
	for (uint32_t i = 0; i < kDivision; ++i) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0.0f, 0.0f};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//Z軸周りの回転行列
		Matrix4x4 rotationMatrix = MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, rotationMatrix);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
	}

	//カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	//存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了扱いにする
		isFinished_ = true;
	}

	//ワールド変換の更新
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}
	//モデルの描画
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjevtion_);
	}
}
