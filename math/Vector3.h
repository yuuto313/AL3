#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	const Vector3 operator+=(const Vector3& other) { 
		return Vector3(this->x += other.x, this->y += other.y, this->z += other.z);
	}

};