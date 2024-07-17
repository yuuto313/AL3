#include "Player.h"
#include "iostream"
#include "ImGuiManager.h"
#include "LockOn.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>&models) { 
	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	//textureHandle_ = textureHandle;
	//  ワールド変換初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, -30.0f};
	// 体
	worldTransformBody_.Initialize();
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};
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
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformLeftArm_.parent_ = &worldTransformBody_;
	worldTransformRightArm_.parent_ = &worldTransformBody_;
	worldTransformWeapon_.parent_ = &worldTransformBody_;

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "HeadTranslation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmLTranslation", worldTransformLeftArm_.translation_);
	globalVariables->AddItem(groupName, "ArmRTranslation", worldTransformRightArm_.translation_);
	globalVariables->AddItem(groupName, "floatingCycle", floatingCycle_);
	globalVariables->AddItem(groupName, "floatingAmplitude",amplitude_);

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
	// 調整項目の適用
	//--------------------------------

	ApplyGlobalVariables();

	//--------------------------------
	//ワールド行列の転送
	//--------------------------------
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

	if (input_->TriggerKey(DIK_RETURN)) {
		GlobalVariables::GetInstance()->SaveFile("Player");
	}

}

void Player::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformRightArm_, viewProjection);
	models_[3]->Draw(worldTransformLeftArm_, viewProjection);
	models_[4]->Draw(worldTransformWeapon_, viewProjection);
}

void Player::ApplyGlobalVariables() { 
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupname = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupname, "HeadTranslation");
	worldTransformLeftArm_.translation_ = globalVariables->GetVector3Value(groupname, "ArmLTranslation");
	worldTransformRightArm_.translation_ = globalVariables->GetVector3Value(groupname, "ArmRTranslation");
	floatingCycle_ = globalVariables->GetIntValue(groupname, "floatingCycle");
	amplitude_ = globalVariables->GetFloatValue(groupname, "floatingAmplitude");
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

void Player::BehaviorJumpInitialize() {
	//階層アニメーション出のパーツの回転などをリセットする
	worldTransformBody_.translation_.y = 0;
	worldTransformLeftArm_.rotation_.x = 0;
	worldTransformRightArm_.rotation_.x = 0;

	//ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	//ジャンプ初速を与える
	velocity_.y = kJumpFirstSpeed;
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


	//--------------------------------
	// ImGui
	//--------------------------------

	ImGui::Begin("Player");
	ImGui::Text("C:Camera\nSPACE:Jump");

	ImGui::SliderFloat3("HeadTranslation", &worldTransformHead_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmLTranslation", &worldTransformLeftArm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmRTranslation", &worldTransformRightArm_.translation_.x, -10.0f, 10.0f);
	if (ImGui::SliderFloat("step", &tempFloat_, 1.0f, 120.0f)) {
		floatingCycle_ = static_cast<uint16_t>(tempFloat_);
	}
	ImGui::SliderInt("floatingCycle", &floatingCycle_, -10, 10);
	ImGui::SliderFloat("floatingAmplitude", &amplitude_, -10.0f, 10.0f);

	ImGui::SliderFloat3("Weapon.rotate", &worldTransformWeapon_.rotation_.x, -10.0f, 10.f);
	ImGui::End();
}

void Player::BehaviorAttackUpdate() { 
	////ロックオン中
	//if (lockOn_ && lockOn_->ExistTarget()) {
	//	// ロックオン座標
	//	Vector3 lockOnPosition = lockOn_->GetTargetPosition();
	//	// 追従対象からロックオン対象へのベクトル
	//	Vector3 sub = lockOnPosition - worldTransform_.translation_;

	//	//距離
	//	float distance = Length(sub);
	//	//距離しきい値
	//	const float threshold = 0.2f;

	//	//しきい値より離れているときのみ
	//	if (distance > threshold) {
	//		//Y軸周り角度
	//		worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
	//		//追い越し防止処理未実装
	//	}
	//}

	

	if (currentRotationAngleX < targetRotationAngleX) {
		currentRotationAngleX += rotationSpeed;
		if (currentRotationAngleX > targetRotationAngleX) {
			currentRotationAngleX = targetRotationAngleX;
		}
	} else {
		behaviorRequest_ = Behavior::kRoot;	
	}

	worldTransformWeapon_.rotation_.x = currentRotationAngleX;
}

void Player::BehaviorjumpUpdate() {
	//移動
	worldTransform_.translation_ += velocity_;
	//重力加速度
	const float kGravityAcceleration = 0.05f;
	//加速度ベクトル
	Vector3 accelaretionVector = {0.0f, -kGravityAcceleration, 0.0f};
	//加速する
	velocity_ += accelaretionVector;

	//着地
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
		//ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
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

		case Behavior::kJump:
			BehaviorJumpInitialize();

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

	case Behavior::kJump:
		//--------------------------------
		// ジャンプ更新
		//--------------------------------

		BehaviorjumpUpdate();

		break;
	}
}


void Player::Movement() {
	//--------------------------------
	// コントローラーで移動処理
	//--------------------------------
	// 速さ
	const float speed = 0.3f;
	XINPUT_STATE joyState;

	Input::GetInstance()->GetJoystickState(0, joyState);

	if ((float)joyState.Gamepad.sThumbLX != 0 || (float)joyState.Gamepad.sThumbLY != 0) {

		// 移動量
		velocity_ = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		// 移動量に速さを反映
		velocity_ = velocity_ * speed;

		// カメラの回転角度を取得
		Vector3 rotationAngle = {GetViewProjection()->rotation_.x, GetViewProjection()->rotation_.y, GetViewProjection()->rotation_.z};

		////カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotationAngle.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotationAngle.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotationAngle.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		// 移動ベクトルをカメラの座標だけ回転する
		velocity_ = TransformNormal(velocity_, rotateXYZMatrix);

		// 移動
		worldTransform_.translation_ += velocity_;

		//--------------------------------
		// 移動方向に見た目を合わせる
		//--------------------------------

		// Y軸周りの角度
		worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	} else if (lockOn_ && lockOn_->ExistTarget()) {
		// ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPosition - worldTransform_.translation_;

		// Y軸周り角度
		worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
	}

	//--------------------------------
	// キーボードで移動操作
	//--------------------------------

	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_W)) {

			acceleration.z += speed;

		} else if (Input::GetInstance()->PushKey(DIK_S)) {

			acceleration.z -= speed;
		}
		velocity_ = acceleration;
	}



	if (input_->PushKey(DIK_W)) {
		velocity_.z = speed;
	}

	if (input_->PushKey(DIK_S)) {
		velocity_.z = -speed;
	}

	if (input_->PushKey(DIK_A)) {
		velocity_.x = -speed;
	}

	if (input_->PushKey(DIK_D)) {
		velocity_.x = speed;
	}

	//--------------------------------
	// 移動方向に見た目を合わせる
	//--------------------------------

	// Y軸周りの角度
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);


	// 移動
	//--------------------------------
	// ジャンプ発動
	//--------------------------------

	if (input_->TriggerKey(DIK_SPACE) || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		// ジャンプリクエスト
		behaviorRequest_ = Behavior::kJump;
	}

	//--------------------------------
	// 攻撃発動
	//--------------------------------
	
	if (input_->TriggerKey(DIK_J) || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		behaviorRequest_ = Behavior::kAttack;
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

}

Vector3 Player::GetCeterPosition() const { 
	//ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Player::OnCollison() {
	//ジャンプリクエスト
	behaviorRequest_ = Behavior::kJump;
}



