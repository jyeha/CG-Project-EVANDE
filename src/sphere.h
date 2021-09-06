#pragma once
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "cgmath.h"

struct sphere_t {
	int		mother = -1;
	vec4	center = vec4(0);
	float	rotation_speed;
	float	revolution_speed;
	float	distance;
	float	radius;
	vec4	color;
	mat4	model_matrix;

	void	update(const std::vector<struct sphere_t>& spheres, float t);
};

inline void sphere_t::update(const std::vector<struct sphere_t>& spheres, float t) {
	float rott = rotation_speed * t * 0.5f;
	float revt = revolution_speed * t * 5.0f;
	float rotc = cos(rott), rots = sin(rott);
	float revc = cos(revt), revs = sin(revt);

	mat4 scale_matrix = {
		radius, 0, 0, 0,
		0, radius, 0, 0,
		0, 0, radius, 0,
		0, 0, 0, 1
	};

	mat4 rotation_matrix = {
		rotc, -rots, 0, 0,
		rots, rotc, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	mat4 revolution_matrix = {
		revc, -revs, 0, 0,
		revs, revc, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	mat4 distance_matrix = {
		1, 0, 0, distance,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	center = revolution_matrix * distance_matrix * vec4(0, 0, 0, 1);

	mat4 mother_matrix = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	if (mother > 0) {
		struct sphere_t mother_planet = spheres[mother];
		mother_matrix = {
			1, 0, 0, mother_planet.center.x,
			0, 1, 0, mother_planet.center.y,
			0, 0, 1, mother_planet.center.z,
			0, 0, 0, 1
		};
	}

	center = mother_matrix * center;
	model_matrix = mother_matrix * revolution_matrix * distance_matrix * rotation_matrix * scale_matrix;
}

#endif