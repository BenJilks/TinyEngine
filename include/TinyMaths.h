#pragma once
#include <string>
#include <iostream>
#include <math.h>
using namespace std;

#define M_PI 3.14159265359
#define TO_RADIANS(degrees) ((degrees) * M_PI / 180.0f)

/* Vector maths */
namespace TinyEngine
{
	/* Store data about a vector */
	template <unsigned int size, typename T>
	struct TinyVector
	{
		/* Create a new empty vector */
		TinyVector() { Reset(); };

		/* Create a vector filled with a value */
		TinyVector(T fill) { for (unsigned int i = 0; i < size; i++) v[i] = fill; };

		/* Create a vector with values */
		template<typename ... T_values>
		TinyVector(T_values... values)
		{
			Reset();
			T value_arrray[] = { (T)values ... };
			for (unsigned int i = 0; i < size; i++)
				v[i] = value_arrray[i];
		}

		/* Copy a vector of a smaller size and parse the rest of the values */
		template<unsigned int smaller_size, typename ... T_values>
		TinyVector(TinyVector<smaller_size, T> vector, T_values... values)
		{
			Reset();
			/* Copy from smaller vector */
			for (unsigned int i = 0; i < smaller_size; i++)
				v[i] = vector.Get(i);
			
			/* Set the rest of the values */
			T value_arrray[] = { (T)values ... };
			for (unsigned int i = smaller_size; i < size; i++)
				v[i] = value_arrray[i - smaller_size];
		}

		/* Sets all the values in the matrix to zero */
		inline void Reset() { for (unsigned int i = 0; i < size; i++) v[i] = (T)0; };
		
		/* Operators */
		TinyVector<size, T> operator+(TinyVector<size, T> right) { TinyVector<size, T> out = *this; for (unsigned int i = 0; i < size; i++) out.v[i] += right.v[i]; return out; }
		TinyVector<size, T> operator-(TinyVector<size, T> right) { TinyVector<size, T> out = *this; for (unsigned int i = 0; i < size; i++) out.v[i] -= right.v[i]; return out; }
		TinyVector<size, T> operator/(T right) { TinyVector<size, T> out = *this; for (unsigned int i = 0; i < size; i++) out.v[i] /= right; return out; }
		void operator+=(TinyVector<size, T> right) { for (unsigned int i = 0; i < size; i++) v[i] += right.v[i]; }
		void operator-=(TinyVector<size, T> right) { for (unsigned int i = 0; i < size; i++) v[i] -= right.v[i]; }

		/* Calculate the dot product */
		TinyVector<3, T> DotProduct(TinyVector<3, T> right)
		{
			TinyVector<3, T> result;
			result.x((y() * right.z()) - (z() * right.y()));
			result.y((z() * right.x()) - (x() * right.z()));
			result.z((x() * right.y()) - (y() * right.x()));
			return result;
		}

		/* Calculate the length of the vector */
		float Length() { float total = 0; for (unsigned int i = 0; i < size; i++) total += v[i] * v[i]; return sqrt(total); }

		/* Return the normalized vector */
		TinyVector<size, T> Normalize() { return *this / Length(); }

		/* Calculates the inner product */
		float InnerProduct(TinyVector<size, T> right) { float result = 0; for (unsigned int i = 0; i < 3; i++) result += v[i] * right[i]; return result; }

		/* Named getters */
		inline T x() { return v[0]; };
		inline T y() { return v[1]; };
		inline T z() { return v[2]; };
		inline T w() { return v[3]; };
		inline T r() { return v[0]; };
		inline T g() { return v[1]; };
		inline T b() { return v[2]; };
		inline T a() { return v[3]; };

		/* Named setter */
		inline void x(T t) { v[0] = t; }
		inline void y(T t) { v[1] = t; }
		inline void z(T t) { v[2] = t; }
		inline void w(T t) { v[3] = t; }
		inline void r(T t) { v[0] = t; }
		inline void g(T t) { v[1] = t; }
		inline void b(T t) { v[2] = t; }
		inline void a(T t) { v[3] = t; }

		/* Print the vector for debugging */
		inline void Print()
		{
			for (unsigned int i = 0; i < size; i++)
				cout << v[i] << (i != size - 1 ? ", " : "\n");
		}

		inline T Get(unsigned int index) { return v[index]; }
		inline void Set(unsigned int index, T value) { v[index] = value; }

		/* Vector data */
		T v[size];
	};

	/* Define vector 3D data types */
	typedef TinyVector<3, float> Vec3f;
	typedef TinyVector<3, int> Vec3i;

	/* Define vector 2D data types */
	typedef TinyVector<2, float> Vec2f;
	typedef TinyVector<2, int> Vec2i;

	/* Define other types */
	typedef TinyVector<4, float> Vec4f;
	typedef Vec4f Colour;
}

/* Matrix maths */
namespace TinyEngine
{
	/* Store information about a matrix */
	template <unsigned int size, typename T>
	struct TinyMatrix
	{
		/* Create an empty matrix */
		TinyMatrix() { Reset(); }

		/* Create an identity matrix */
		TinyMatrix(T scale) { Reset(); for (unsigned int i = 0; i < size; i++) m[i*size + i] = scale; }

		/* Sets all the values in the matrix to zero */
		inline void Reset() { for (unsigned int i = 0; i < size*size; i++) m[i] = 0.0f; }

		/* Prints the matrix for debugging */
		inline void Print()
		{
			for (unsigned int i = 0; i < size*size; i++)
				cout << m[i] << ((i % size == size-1) ? "\n" : ", ");
		}

		/* Multiply two matrices together */
		TinyMatrix<size, T> operator*(TinyMatrix<size, T> right)
		{
			TinyMatrix<size, T> result;
			for (unsigned int row = 0; row < size; row++)
				for (unsigned int column = 0; column < size; column++)
					for (unsigned int rolumn = 0; rolumn < 4; rolumn++)
						result.m[column*size+row] += m[rolumn*size+row] * right.m[column*size+rolumn];
			return result;
		}
		TinyMatrix<size, T> operator*=(TinyMatrix<size, T> right)
		{
			return this->operator*(right);
		}

		/* Returns a value at 2D point */
		template<unsigned int x, unsigned int y>
		inline T Get() { return m[y * size + x]; }

		/* Sets a value at a 2D point */
		template<unsigned int x, unsigned int y>
		inline void Set(T value) { m[y * size + x] = value; }

		T m[size*size];
	};

	/* Define matrix types */
	typedef TinyMatrix<3, float> Mat3f;
	typedef TinyMatrix<4, float> Mat4f;
	
	/* Translate a transformation matrix by a vector */
	Mat4f TranslateMatrix(Mat4f matrix, Vec3f vector);

	/* Rotate a transformation matrix in each axis */
	Mat4f RotateMatrix(Mat4f matrix, Vec3f amounts);

	/* Scales a transformation matrix */
	Mat4f ScaleMatrix(Mat4f matrix, Vec3f scale);

	/* Creates a new perspective matrix */
	Mat4f CreatePerspectiveMatrix(float fov, float near_plane, float far_plane, float aspect_ratio);
}