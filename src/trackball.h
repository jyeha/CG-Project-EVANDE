#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__
#include "cgmath.h"

struct trackball
{
	bool	b_tracking = false;
	bool	b_zooming = false;
	bool	b_panning = false;
	float	scale;			// controls how much rotation is applied
	mat4	view_matrix0;	// initial view matrix
	vec2	m0;				// the last mouse position
	vec3	eye0;
	vec3	at0;

	trackball(float rot_scale = 1.0f) :scale(rot_scale) {}
	bool is_tracking() const { return b_tracking; }
	void begin(const mat4& view_matrix, float x, float y)
	{
		b_tracking = true;			// enable trackball tracking
		m0 = vec2(x, y) * 2.0f - 1.0f;	// convert (x,y) in [0,1] to [-1,1]
		view_matrix0 = view_matrix;	// save current view matrix
	}
	vec3 end(mat4 new_view) {
		b_tracking = false;
		return mat3(new_view).inverse() * (-vec3(new_view._14, new_view._24, new_view._34));
	}

	bool is_zooming() const { return b_zooming; }
	void begin_zoom(const mat4& view_matrix, float x, float y, vec3 eye)
	{
		b_zooming = true;			// enable trackball tracking
		m0 = vec2(x, y) * 2.0f - 1.0f;	// convert (x,y) in [0,1] to [-1,1]
		view_matrix0 = view_matrix;	// save current view matrix
		eye0 = eye;
	}
	vec3 end_zoom(mat4 new_view) {
		b_zooming = false;
		return mat3(new_view).inverse() * (-vec3(new_view._14, new_view._24, new_view._34));
	}

	bool is_panning() const { return b_panning; }
	void begin_pan(const mat4& view_matrix, float x, float y, vec3 eye, vec3 at)
	{
		b_panning = true;			// enable trackball tracking
		m0 = vec2(x, y) * 2.0f - 1.0f;	// convert (x,y) in [0,1] to [-1,1]
		view_matrix0 = view_matrix;	// save current view matrix
		eye0 = eye;
		at0 = at;
	}
	std::vector<vec3> end_pan(mat4 new_view) {
		b_panning = false;
		vec3 new_eye = mat3(view_matrix0).inverse() * (-vec3(new_view._14, new_view._24, new_view._34));
		std::vector<vec3> tmp = { new_eye, at0 + new_eye - eye0 };
		return tmp;
	}

	mat4 update(float x, float y, vec3 center)
	{
		// retrive the current mouse position
		vec2 m = vec2(x, y) * 2.0f - 1.0f; // normalize xy

		// project a 2D mouse position to a unit sphere
		static const vec3 p0 = vec3(0, 0, 1.0f);	// reference position on sphere
		vec3 p1 = vec3(m.x - m0.x, m0.y - m.y, 0);	// displacement with vertical swap
		if (!(b_tracking || b_zooming || b_panning) || length(p1) < 0.0001f) return view_matrix0;			// ignore subtle movement
		p1 *= scale;														// apply rotation scale
		p1 = vec3(p1.x, p1.y, sqrtf(max(0, 1.0f - length2(p1)))).normalize();	// back-project z=0 onto the unit sphere

		// find rotation axis and angle
		// - right-mult of mat3 = inverse view rotation to world
		// - i.e., rotation is done in the world coordinates

		if (b_tracking) {
			vec3 n = p0.cross(p1) * mat3(view_matrix0);
			float angle = asin(min(n.length(), 1.0f));
			// return resulting rotation matrix
			//printf("eye: (%f, %f, %f)\tcenter: (%f, %f, %f)\n", eye0.x, eye0.y, eye0.z, center.x, center.y, center.z);
			return view_matrix0 * mat4::translate(center) * mat4::rotate(n.normalize(), angle) * mat4::translate(-center);
		}

		if (b_zooming) {
			vec3 n = view_matrix0.rvec3(2);
			vec3 deye = (m0.y - m.y) * n * 100.0f;
			//printf("eye: (%f, %f, %f)\tdeye: (%f, %f, %f)\n", eye0.x, eye0.y, eye0.z, deye.x, deye.y, deye.z);
			return view_matrix0 * mat4::translate(-deye);
		}

		if (b_panning) {
			mat4 new_view = mat4::translate(100.0f * vec3(m.x - m0.x, m0.y - m.y, 0.0f)) * view_matrix0;
			vec3 new_eye = mat3(new_view).inverse() * (-vec3(new_view._14, new_view._24, new_view._34));
			//printf("eye: (%f, %f, %f)\tat: (%f, %f, %f)\n", eye0.x, eye0.y, eye0.z, at0.x, at0.y, at0.z);
			return new_view;
		}

		return view_matrix0;
	}
};

#endif // __TRACKBALL_H__
