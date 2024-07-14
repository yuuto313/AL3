#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	// オペレーター関数 *=
	Matrix4x4& operator*=(const Matrix4x4& other) {
		Matrix4x4 result;
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				float sum = 0.0f;
				for (int k = 0; k < 4; ++k) {
					sum += m[row][k] * other.m[k][col];
				}
				result.m[row][col] = sum;
			}
		}
		// 結果を元の行列にコピーする
		*this = result;
		return *this;
	}

	//*演算
	Matrix4x4 operator*(const Matrix4x4& other) const {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m[i][k] * other.m[k][j];
				}
			}
		}
		return result;
	}

};
