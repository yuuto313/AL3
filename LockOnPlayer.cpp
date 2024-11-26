#include "LockOnPlayer.h"
#include "Input.h"
#include "TextureManager.h"
#include "WinApp.h"

void LockOnPlayer::Initialize() { 
	textureHandel_ = TextureManager::Load("lockOn.png");

	lockOnSprite_.reset(Sprite::Create(textureHandel_, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
}

void LockOnPlayer::Update(const std::unique_ptr<Player>& player, const ViewProjection& viewProjection) {
	
	

	// ロックオン状態なら
	if (target_) {
		// 範囲外判定
		if (OutsideSelectionRange(viewProjection)) {
		// ロックオンを外す
		target_ = nullptr;
		}
	} else {
		// ロックオン対象の検索
		Search(player, viewProjection);
	}

	// ロックオン継続
	if (target_) {
		// プレイヤーのロックオン座標
		Vector3 positionWorld = target_->GetCenterPosition();
		// ワールド座標からスクリーン座標に変換
		Vector3 positionScreen = WorldToScreen(positionWorld, viewProjection);
		// Vector2に格納
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);
		// スプライトの座標を設定
		lockOnSprite_->SetPosition(positionScreenV2);
	}

}

void LockOnPlayer::Draw() {
	
	if (target_) {
		lockOnSprite_->Draw();
	}

}

void LockOnPlayer::Search(const std::unique_ptr<Player>& player, const ViewProjection& viewProjection) {
	// 目標
	std::list<std::pair<float, const Player*>> targets;

	// プレイヤーに対してロックオン判定
	// 敵のロックオン座標を取得
	Vector3 positionWorld = player->GetCenterPosition();

	// ワールド->ビュー座標変換
	Vector3 positionView = Transform(positionWorld, viewProjection.matView);
	
	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		// 角度条件チェック（コーンに収まっているか）
		if (std::abs(arcTangent) <= angleRange_) {
			targets.emplace_back(std::make_pair(positionView.z, player.get()));
		}
	}

	// ロックオン対象をリセット
	target_ = nullptr;
	if (!targets.empty()) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		// ソートの結果一番近いプレイヤーをロックオン対象とする
		target_ = targets.front().second;
	}

}

Vector3 LockOnPlayer::WorldToScreen(Vector3& worldPosition, const ViewProjection& viewprojection) { 
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = viewprojection.matView * viewprojection.matProjection * matViewport;
	// ワールド->スクリーン変換
	Vector3 screenPosition = Transform(worldPosition, matViewProjectionViewport);
	return screenPosition;
}

bool LockOnPlayer::OutsideSelectionRange(const ViewProjection& viewProjection) {
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

Vector3 LockOnPlayer::GetTargetPosition() const {
	if (target_) {
		return target_->GetCenterPosition();
	}
	return Vector3{};
}
