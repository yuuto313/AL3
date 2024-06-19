#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	const Vector3 operator+=(const Vector3& other) 
	{ 
		return Vector3(this->x += other.x, this->y += other.y, this->z += other.z);
	}

	const Vector3 operator-(const Vector3& other) 
	{ 
		return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	const Vector3 operator*(const float& other) 
	{ 
		return Vector3(this->x * other, this->y * other, this->z * other);
	}


	Vector3 operator*(const Matrix4x4& mat) {
		// 行列の4列目は無視しているものと仮定（これは一般的な方法です）
		return Vector3{
		    mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3], mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3],
		    mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3]};

};