#pragma once

class Vector4D
{
public:
	// Members
	float x, y, z, w;

	// Construction/destruction:
	Vector4D(void);
	Vector4D(float X, float Y, float Z, float W);
	Vector4D(float* clr);

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector4D& v) const;
	bool operator!=(const Vector4D& v) const;

	// arithmetic operations
	Vector4D& operator+=(const Vector4D& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	Vector4D& operator-=(const Vector4D& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	Vector4D& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	Vector4D& operator*=(const Vector4D& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	Vector4D& operator/=(const Vector4D& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	// this ought to be an opcode.
	Vector4D&	operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector4D&	operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}
	Vector4D&	operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}

	// negate the vector components
	void	Negate();

	// Get the vector's magnitude.
	float	Length() const;

	// Get the vector's magnitude squared.
	float LengthSqr(void) const
	{
		return (x*x + y*y + z*z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance &&
			w > -tolerance && w < tolerance);
	}

	float	NormalizeInPlace();
	Vector4D	Normalized() const;
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector4D const &boxmin, Vector4D const &boxmax);

	// Get the distance from this vector to the other one.
	float	DistTo(const Vector4D &vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	float DistToSqr(const Vector4D &vOther) const
	{
		Vector4D delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;
		delta.w = w - vOther.w;

		return delta.LengthSqr();
	}

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void	MulAdd(const Vector4D& a, const Vector4D& b, float scalar);

	// Dot product.
	float	Dot(const Vector4D& vOther) const;

	// assignment
	Vector4D& operator=(const Vector4D &vOther);

	// 2d
	float	Length2D(void) const;
	float	Length2DSqr(void) const;

	/// get the component of this vector parallel to some other given vector
	Vector4D  ProjectOnto(const Vector4D& onto);

	// copy constructors
	//	Vector4D(const Vector4D &vOther);

	// arithmetic operations
	Vector4D	operator-(void) const;

	Vector4D	operator+(const Vector4D& v) const;
	Vector4D	operator-(const Vector4D& v) const;
	Vector4D	operator*(const Vector4D& v) const;
	Vector4D	operator/(const Vector4D& v) const;
	Vector4D	operator*(float fl) const;
	Vector4D	operator/(float fl) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector4D	Min(const Vector4D &vOther) const;
	Vector4D	Max(const Vector4D &vOther) const;
};