#include "LockOn.h"
#include "TextureManager.h"
#include "WinApp.h"

void LockOn::Initalize() {
	textureHandle_ = TextureManager::Load("lockOn.png");
	//スプライトの生成
	lockOnMark_.reset(Sprite::Create(textureHandle_, {640.0f,360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	// ロックオン状態なら
	if (target_) {
		// ロックオン解除処理
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			// ロックオンを外す
			target_ = nullptr;
		}
		// 選択外判定
		else if (OutsideSelectionRange(viewProjection)) {
			// ロックオンを外す
			target_ = nullptr;
		}
	} else {
		// ロックオン対象の検索
		// ロックオンボタンをトリガーしたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			// ロックオン対象の検索
			Search(enemies, viewProjection);
		}
	}

	// ロックオン継続
	if (target_) {
		// 敵のロックオン座標取得
		Vector3 positionWorld = target_->GetCenterPosition();
		// ワールド->スクリーン変換
		Vector3 positionScreen = WorldToScreen(positionWorld, viewProjection);
		// Vector2に格納
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);
		// スプライトの座標を設定
		lockOnMark_->SetPosition(positionScreenV2);
	}
}


void LockOn::Draw() { 
	//ロックオン中なら
	if (target_) {
		lockOnMark_->Draw();
	}
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {
	// ロックオン対象の絞り込み
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// すべての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		// 敵のロックオン座標を取得
		Vector3 positionWorld = enemy->GetCenterPosition();
		// ワールド->ビュー座標変換
		Vector3 positionView = Transform(positionWorld, viewProjection.matView);

		// 距離条件のチェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
			// カメラ前方との角度を計算
			float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

			// 角度条件チェック(コーンにおさまってるか)
			if (std::abs(arcTangent) <= angleRange_) {
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}

	}
	// 距離でソートしてロックオン
	// ロックオン対象をリセット
	target_ = nullptr;

	if (!targets.empty()) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象とする
		target_ = targets.front().second;
	}

}

Vector3 LockOn::WorldToScreen(Vector3& worldPosition,const ViewProjection& viewprojection) {
	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = viewprojection.matView * viewprojection.matProjection * matViewport;
	//ワールド->スクリーン変換
	Vector3 screenPosition = Transform(worldPosition, matViewProjectionViewport);
	return screenPosition;
}

bool LockOn::OutsideSelectionRange(const ViewProjection& viewProjection) {
	std::list<std::pair<float, const Enemy*>> targets;

	// すべての敵に対して順にロックオン判定
	// 敵のロックオン座標を取得
	Vector3 positionWorld = target_->GetCenterPosition();
	// ワールド->ビュー座標変換
	Vector3 positionView = Transform(positionWorld, viewProjection.matView);
	// 距離条件のチェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);
		// 角度条件チェック(コーンにおさまってるか)
		if (std::abs(arcTangent) <= angleRange_) {
			// 範囲外ではない
			return false;
		}
	}
	// 範囲外である
	return true;
}

Vector3 LockOn::GetTargetPosition() const {
	if (target_) {
		return target_->GetCenterPosition();
	}
	return Vector3{};
}