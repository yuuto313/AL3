#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <corecrt_math_defines.h>
#pragma once

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 線形保管
Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);

//球面線形補間
Vector3 Sleap(const Vector3& v1, const Vector3& v2, float t);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡縮
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);


//ベクトル変換
//ベクトルの向きだけを回転させるため、4x4行列の平行移動成分を打ち消して、回転（とスケーリング）のみ反映させたもの
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 二項演算子
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator+(float s, const Vector3& v);
Vector3 operator*(float s, const Vector3& v);
Vector3 operator*(const Vector3& v, float s);
Vector3 operator/(const Vector3& v, float s);