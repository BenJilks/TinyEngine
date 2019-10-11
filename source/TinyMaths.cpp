#include "TinyMaths.h"
using namespace TinyEngine;

/* Translate a transformation matrix by a vector */
Mat4f TinyEngine::TranslateMatrix(Mat4f matrix, Vec3f vector)
{
	matrix.Set<3, 0>(vector.x());
	matrix.Set<3, 1>(vector.y());
	matrix.Set<3, 2>(vector.z());
	return matrix;
}

/* Create a rotation matrix in the X axis */
Mat4f CreateXRotation(float amount)
{
	Mat4f matrix(1.0f);
	matrix.Set<1, 1>(cos(amount));
	matrix.Set<2, 1>(-sin(amount));
	matrix.Set<1, 2>(sin(amount));
	matrix.Set<2, 2>(cos(amount));
	return matrix;
}

/* Create a rotation matrix in the Y axis */
Mat4f CreateYRotation(float amount)
{
	Mat4f matrix(1.0f);
	matrix.Set<0, 0>(cos(amount));
	matrix.Set<2, 0>(sin(amount));
	matrix.Set<0, 2>(-sin(amount));
	matrix.Set<2, 2>(cos(amount));
	return matrix;
}

/* Create a rotation matrix in the Z axis */
Mat4f CreateZRotation(float amount)
{
	Mat4f matrix(1.0f);
	matrix.Set<0, 0>(cos(amount));
	matrix.Set<1, 0>(-sin(amount));
	matrix.Set<0, 1>(sin(amount));
	matrix.Set<1, 1>(cos(amount));
	return matrix;
}

/* Rotate a transformation matrix in each axis */
Mat4f TinyEngine::RotateMatrix(Mat4f matrix, Vec3f amounts)
{
	matrix = matrix * CreateZRotation((float)TO_RADIANS(amounts.z()));
	matrix = matrix * CreateYRotation((float)TO_RADIANS(amounts.y()));
	matrix = matrix * CreateXRotation((float)TO_RADIANS(amounts.x()));
	return matrix;
}

/* Scales a transformation matrix */
Mat4f TinyEngine::ScaleMatrix(Mat4f matrix, Vec3f scale)
{
	matrix.Set<0, 0>(scale.x());
	matrix.Set<1, 1>(scale.y());
	matrix.Set<2, 2>(scale.z());
	return matrix;
}

/* Creates a new perspective matrix */
Mat4f TinyEngine::CreatePerspectiveMatrix(float fov, float near_plane, float far_plane, float aspect_ratio)
{
	Mat4f resualt;
	float y_scale = (1.0f / (float)tan(TO_RADIANS(fov / 2.0f))) * aspect_ratio;
	float x_scale = y_scale / aspect_ratio;
	float frustrum_length = far_plane - near_plane;

	resualt.Set<0, 0>(x_scale);
	resualt.Set<1, 1>(y_scale);
	resualt.Set<2, 2>(-((far_plane - near_plane) / frustrum_length));
	resualt.Set<2, 3>(-1.0f);
	resualt.Set<3, 2>(-((2 * far_plane * near_plane) / frustrum_length));
	resualt.Set<3, 3>(0.0f);
	return resualt;
}