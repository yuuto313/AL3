#include "Player.h"
#include "iostream"
#include "ImGuiManager.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* modelFighterBody, Model* modelFighterHead, Model* modelFighterLightArm, Model* modelFighterRightArm, ViewProjection* viewProjection) { 
	//NULLポインタチェック
	assert(modelFighterBody);
	assert(modelFighterHead);
	assert(modelFighterLightArm);
	assert(modelFighterRightArm);
	//引数として受け取ったデータをメンバ変数に記録する
	modelFighterBody_ = modelFighterBody;
	modelFighterHead_ = modelFighterHead;
	modelFighterLeftArm_ = modelFighterLightArm;
	modelFighterRightArm_ = modelFighterRightArm;
	//model_ = model;
	
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	//ワールド変換初期化
	worldTransformBase_.Initialize();
	//体
	worldTransformBody_.Initialize();
	worldTransformBody_.translation_ = {0.0f, 2.0f, 0.0f};
	//頭
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = {0.0f, 5.0f, 0.0f};
	//右腕
	worldTransformRightArm_.Initialize();
	worldTransformRightArm_.translation_ = {2.0f, 0.0f, 0.0f};
	//左腕
	worldTransformLeftArm_.Initialize();
	worldTransformLeftArm_.translation_ = {-2.0f, 0.0f, 0.0f};


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//浮遊ギミック初期化
	InitializeFloatingGimmick();

}

void Player::Update() { 
	//--------------------------------
	//移動
	//--------------------------------

	Move();

	//--------------------------------
	// 浮遊ギミック更新
	//--------------------------------

	UpdateFloatingGimmick();

	//--------------------------------
	//ワールド行列の転送
	//--------------------------------
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
}

void Player::Draw() { 
	modelFighterBody_->Draw(worldTransformBody_, *viewProjection_); 
	modelFighterHead_->Draw(worldTransformHead_, *viewProjection_); 
	modelFighterRightArm_->Draw(worldTransformRightArm_, *viewProjection_); 
	modelFighterLeftArm_->Draw(worldTransformLeftArm_, *viewProjection_); 

}

void Player::Move() {
	//--------------------------------
	//コントローラーで移動処理
	//--------------------------------
	//速さ 
	const float speed = 0.3f;
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		
		//移動量
		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		//移動量に速さを反映
		move = move * speed;

		// カメラの回転角度を取得

		Vector3 rotationAngle = {GetViewProjection()->rotation_.x, GetViewProjection()->rotation_.y, GetViewProjection()->rotation_.z};

		////カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotationAngle.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotationAngle.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotationAngle.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		//移動ベクトルをカメラの座標だけ回転する
		move = TransformNormal(move, rotateXYZMatrix);
		
		//移動
		worldTransformBase_.translation_ += move;
		worldTransformBody_.translation_ += move;
		worldTransformHead_.translation_ += move;
		worldTransformRightArm_.translation_ += move;
		worldTransformLeftArm_.translation_ += move;

		//--------------------------------
		// 移動方向に見た目を合わせる
		//--------------------------------

		//Y軸周りの角度
		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformBody_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformHead_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformLeftArm_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformRightArm_.rotation_.y = std::atan2(move.x, move.z);
	}
}

void Player::InitializeFloatingGimmick() { 
	floatingParameter_ = 0.0f;

}

void Player::UpdateFloatingGimmick() {
	//浮遊移動のサイクル＜Frame＞
	const uint16_t floatingCycle = 60;
	//1フレームでのパラメータ加算値
	//そのフレーム数の回数だけ加算したら2πになる数値を求めて、毎フレーム加算する数値となる
	float step = 2.0f * (float)M_PI / floatingCycle;
	//パラメータを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * (float)M_PI);
	//浮遊の振幅<m>
	float amplitude = 0.5f;
	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;
	worldTransformHead_.translation_.y = std::sin(floatingParameter_) * amplitude;
	worldTransformRightArm_.translation_.y = std::sin(floatingParameter_) * amplitude;
	worldTransformRightArm_.rotation_.x = std::sin(floatingParameter_) * amplitude;
	worldTransformLeftArm_.translation_.y = std::sin(floatingParameter_) * amplitude;
	worldTransformLeftArm_.rotation_.x = std::sin(floatingParameter_) * amplitude;


	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head_Translation",&worldTransformHead_.translation_.x,-10.0f,10.0f);
	ImGui::SliderFloat3("LArm_Translation",&worldTransformLeftArm_.translation_.x,-10.0f,10.0f);
	ImGui::SliderFloat3("RArm_Translation",&worldTransformRightArm_.translation_.x,-10.0f,10.0f);
	ImGui::SliderFloat("step", reinterpret_cast<float>(floatingCycle), -120.0f, 120.0f);
	ImGui::SliderFloat("amplitude", &amplitude,-10.0f, 10.0f);
	ImGui::End();

}



