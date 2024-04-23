#pragma once

#include <Vector3.h>
#include <cmath>
#include <Matrix4x4.h>

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



