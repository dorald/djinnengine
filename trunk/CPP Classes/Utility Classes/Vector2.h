/*
 *  Vector2.h
 *  Djinn Engine
 *
 *  Created by Craig Giles on 4/22/09.
 *
 */

#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2
{
public:
	Vector2();
	Vector2( const float value );
	Vector2( const float x, const float y );
	~Vector2();
	
	//	The X and Y dimensions. These are publicaly available because
	//	its so often gotten or set, and have no "incorrect" values, that
	//	there is no good reason to encapsulate it.
	float x;
	float y;
	
	//	Returns the unit vectors with the members initialized to 1 or 0 depending
	//	which function is called. The ZERO vector has both the X and Y coord set
	//	to 0, while the ONE vector has both the X and Y coord set to 1.
	static Vector2 ONE() { return Vector2(1, 1); }
	static Vector2 ZERO() { return Vector2(0, 0); }
	
	//	assignment operator overload.
	Vector2 &operator = ( const Vector2 &value );
	
	//	operator overloads for math manipulations of vector objects (IE: VectorA + VectorB)
	//	Returns an anon object of Vector2 type.
	Vector2 operator + ( const Vector2 &value ) const;
	Vector2 operator - ( const Vector2 &value ) const;
	Vector2 operator * ( const Vector2 &value ) const;
	Vector2 operator / ( const Vector2 &value ) const;
	
	//	operator overloads for map manipulations of vector objects (IE: VectorA += VectorB)
	//	returns by value since the invoking object is being changed
	Vector2 &operator += ( const Vector2 &value );
	Vector2 &operator -= ( const Vector2 &value );
	Vector2 &operator *= ( const Vector2 &value );
	Vector2 &operator /= ( const Vector2 &value );
	
private:
	static Vector2 unitOne;
	static Vector2 unitZero;
};

#endif