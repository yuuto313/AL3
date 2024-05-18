#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;


  
	// - 演算子のオーバーロード
	Vector3 operator-(float other) const { return Vector3(other - x, other - y, other - z); }

	// * 演算子のオーバーロード
	Vector3 operator*(float other) const { return Vector3(x + other, y + other, z + other);}

    // +演算子のオーバーロード
	Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }

	// -演算子のオーバーロード
	Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    
	// += 演算子のオーバーロード
	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	};
};