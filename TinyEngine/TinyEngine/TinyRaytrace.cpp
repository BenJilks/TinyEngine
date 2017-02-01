#include "TinyRaytrace.h"

#if GRAPHICS_MODE == RAYTRACE
using namespace TinyEngine;

/* Check for a ray intersection */
TinyCollitionInfo TinyFace::CollideWithRay(TinyRay ray)
{
	Vec3f e1, e2, h, s, q;
	float a, f, u, v;
	e1 = m_points[1] - m_points[0];
	e2 = m_points[2] - m_points[0];

	h = ray.m_direction.DotProduct(e2);
	a = e1.InnerProduct(h);

	if (a > -0.00001 && a < 0.00001)
		return TinyCollitionInfo();

	f = 1 / a;
	s = ray.m_origin - m_points[0];
	u = f * s.InnerProduct(h);

	if (u < 0.0 || u > 1.0)
		return TinyCollitionInfo();

	q = s.DotProduct(e1);
	v = f * ray.m_direction.InnerProduct(q);

	if (v < 0.0 || u + v > 1.0)
		return TinyCollitionInfo();

	float t = f * e2.InnerProduct(q);

	if (t > 0.00001)
		return TinyCollitionInfo(Vec3f(), Vec3f());
	else
		return TinyCollitionInfo();
}

#endif