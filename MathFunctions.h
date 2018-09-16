

#pragma once

#include "Vector.h"

#include <stdint.h>

#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f

double inline __declspec (naked) __fastcall FASTSQRT(double n)
{
	_asm fld qword ptr[esp + 4]
		_asm fsqrt
	_asm ret 8
}

void AngleVectors(const Vector &angles, Vector *forward);
void VectorTransform(const Vector in1, float in2[3][4], Vector &out);
void inline SinCos(float radians, float *sine, float *cosine);
Vector ExtrapolateTick(Vector p0, Vector v0);
void VectorAngles(const Vector &vecForward, Vector &vecAngles);
void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
void Normalize(Vector &vIn, Vector &vOut);
void SEAngleFix(float &val);
void CalcAngle(const Vector & vecSource, const Vector & vecDestination, Vector & qAngles);
float AngleNormalize(float angle);

float GetFov(const Vector & viewAngle, const Vector & aimAngle);

float VectorDistance(Vector v1, Vector v2);
