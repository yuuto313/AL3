#include "LockOn.h"
#include "TextureManager.h"

void LockOn::Initalize() {
	textureHandle_ = TextureManager::Load("lockOn.png");
	//スプライトの生成
	lockOnMark_.reset(Sprite::Create(textureHandle_, {640.0f,360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {
	//ロックオン状態なら
	if (target_) {
		//c.ロックオン解除処理
	} else {
		// a.ロックオン対象の検索
		// ロックオンボタンをトリガーしたら
		if (input_->TriggerKey(DIK_R)) {
			//ロックオン対象の検索
			Search(enemies, viewProjection);

		}
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

	// ロックオン継続
	if (target_) {
		//敵のロックオン座標取得
		//Vector3 positionWorld = target_->GetCenterPosition();
		//ワールド座標からスクリーン座標に変更
		Vector3 positionScreen=

	}
}
