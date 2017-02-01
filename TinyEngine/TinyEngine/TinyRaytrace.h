#pragma once
#include "TinyCompilerConfig.h"

#if GRAPHICS_MODE == RAYTRACE
#include "TinyMaths.h"

namespace TinyEngine
{

	/* Represents a ray of light */
	struct TinyRay
	{
		TinyRay(Vec3f origin, Vec3f direction) : 
			m_origin(origin), m_direction(direction) {}

		/* Where the ray beginis */
		Vec3f m_origin;
		
		/* The direction the ray is facing */
		Vec3f m_direction;
	};

	/* Represents a collition */
	struct TinyCollitionInfo
	{
		TinyCollitionInfo() :
			m_hit(false) {}
		TinyCollitionInfo(Vec3f point, Vec3f normal) :
			m_hit(true), m_point(point), m_normal(normal) {}

		/* Has it hit the object or not */
		bool m_hit;

		/* The point it was hit at */
		Vec3f m_point;

		/* The direction it was hit at */
		Vec3f m_normal;
	};

	/* Store information about a single face (triangle) and handle ray collisions */
	class TinyFace
	{
	public:
		TinyFace(Vec3f points[3]) { memcpy(m_points, points, 3); }

		/* Check for a ray intersection */
		TinyCollitionInfo CollideWithRay(TinyRay ray);
	private:
		/* Store all the points of the face */
		Vec3f m_points[3];
	};
}

#endif