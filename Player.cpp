#include "Player.h"
#include "iostream"
#include "ImGuiManager.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>&models) { 
	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	//textureHandle_ = textureHandle;
	//  ワールド変換初期化
	worldTransformBase_.Initialize();
	// 体
	worldTransformBody_.Initialize();
	worldTransformBody_.translation_ = {0.0f, 10.0f, 0.0f};
	// 頭
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = {0.0f, 0.0f, 0.0f};
	// 右腕
	worldTransformRightArm_.Initialize();
	worldTransformRightArm_.translation_ = {2.0f, 3.0f, 0.0f};
	// 左腕
	worldTransformLeftArm_.Initialize();
	worldTransformLeftArm_.translation_ = {-2.0f, 3.0f, 0.0f};
	//武器
	worldTransformWeapon_.Initialize();


	// 親子関係を結ぶ
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformLeftArm_.parent_ = &worldTransformBody_;
	worldTransformRightArm_.parent_ = &worldTransformBody_;
	worldTransformWeapon_.parent_ = &worldTransformBody_;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//浮遊ギミック初期化
	InitializeFloatingGimmick();


}

void Player::Update() { 
	//--------------------------------
	// 基底クラスの更新
	//--------------------------------

	BaseCharacter::Update();

	//--------------------------------
	// 攻撃行動の更新
	//--------------------------------

	ChangeBehavior();

	//--------------------------------
	//ワールド行列の転送
	//--------------------------------
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformRightArm_, viewProjection);
	models_[3]->Draw(worldTransformLeftArm_, viewProjection);
	models_[4]->Draw(worldTransformWeapon_, viewProjection);
}

void Player::BehaviorRootInitialize() { 
	InitializeFloatingGimmick();
	worldTransformWeapon_.rotation_ = {};
	worldTransformLeftArm_.rotation_ = {};
	worldTransformRightArm_.rotation_ = {};
}

void Player::BehaviorAttackInitialize() { 
	//ギミックのアニメーション用数値のリセット
	currentRotationAngleX = 0.0f;
}

void Player::BehaviorRootUpdate() {
	
	//--------------------------------
	// 移動
	//--------------------------------

	Movement();

	//--------------------------------
	// 浮遊ギミック更新
	//--------------------------------

	UpdateFloatingGimmick();

}

void Player::BehaviorAttackUpdate() { 
	if (currentRotationAngleX < targetRotationAngleX) {
		currentRotationAngleX += rotationSpeed;
		if (currentRotationAngleX > targetRotationAngleX) {
			currentRotationAngleX = targetRotationAngleX;
		}
	} else {
		behaviorRequest_ = Behavior::kRoot;	
	}

	//worldTransformLeftArm_.rotation_.x = currentRotationAngleX;
	//worldTransformRightArm_.rotation_.x = currentRotationAngleX;
	worldTransformWeapon_.rotation_.x = currentRotationAngleX;
}

void Player::ChangeBehavior() {
	// std::nullopt以外の値がはいってるときtrueになる
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:

			BehaviorRootInitialize();

			break;

		case Behavior::kAttack:

			BehaviorAttackInitialize();

			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Behavior::kRoot:
	default:
		//--------------------------------
		// 通常行動の更新
		//--------------------------------

		BehaviorRootUpdate();

		break;
	case Behavior::kAttack:
		//--------------------------------
		// 攻撃行動の更新
		//--------------------------------

		BehaviorAttackUpdate();

		break;
	}
}


void Player::Movement() {
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
		
		//--------------------------------
		// 移動方向に見た目を合わせる
		//--------------------------------

		//Y軸周りの角度
		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);
		
	}
}

void Player::InitializeFloatingGimmick() { 
	//浮遊ギミックに使う変数を初期化
	floatingParameter_ = 0.0f;

	// 浮遊移動のサイクル＜Frame＞
	//何フレームでアニメーションするか
	floatingCycle_ = 60;
	tempFloat_ = static_cast<float>(floatingCycle_);

	//そのフレーム数の回数だけ加算したら2πになる数値を求めて、毎フレーム加算する値とする
	step_ = 2.0f * (float)M_PI / floatingCycle_;

	//振幅<m>
	amplitude_ = 0.5f;
}

void Player::UpdateFloatingGimmick() {

	//1フレームでのパラメータ加算値
	//そのフレーム数の回数だけ加算したら2πになる数値を求めて、毎フレーム加算する数値となる
	step_= 2.0f * (float)M_PI / floatingCycle_;

	//パラメータを1ステップ分加算
	floatingParameter_ += step_;

	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * (float)M_PI);

	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude_;

	//手をぶらぶらさせる
	worldTransformRightArm_.rotation_.x = std::sin(floatingParameter_) * amplitude_;
	worldTransformLeftArm_.rotation_.x = std::sin(floatingParameter_) * amplitude_;


	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head_Translation",&worldTransformHead_.translation_.x,-10.0f,10.0f);
	ImGui::SliderFloat3("LArm_Translation",&worldTransformLeftArm_.translation_.x,-10.0f,10.0f);
	ImGui::SliderFloat3("RArm_Translation",&worldTransformRightArm_.translation_.x,-10.0f,10.0f);
	if (ImGui::SliderFloat("step", &tempFloat_, 1.0f, 120.0f)) {
		floatingCycle_ = static_cast<uint16_t>(tempFloat_);
	}
	ImGui::SliderFloat("amplitude", &amplitude_,-10.0f, 10.0f);

	ImGui::SliderFloat3("Weapon.rotate", &worldTransformWeapon_.rotation_.x, -10.0f, 10.f);

	bool attack = false;
	ImGui::Checkbox("Attack", &attack);
	if (attack) {
		behaviorRequest_ = Behavior::kAttack;
	}
	ImGui::End();

}



