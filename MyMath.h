#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <corecrt_math_defines.h>
#pragma once

// 長さ
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

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