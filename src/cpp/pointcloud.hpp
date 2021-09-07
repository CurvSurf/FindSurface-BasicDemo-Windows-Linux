#pragma once

#include <array>
#include <iostream>
#include <FindSurface.hpp>

#define SPHERE_INDEX	7811
#define CYLINDER_INDEX	3437
#define CONE_INDEX		6637
#define TORUS_INDEX		7384

struct Preset {

	static const std::array<Preset, 4> normalList;
	static const std::array<Preset, 3> smartList;

	FindSurface::FeatureType featureType;
	unsigned int seedIndex;
};

struct Point {
	float x, y, z;
};

inline std::ostream& operator<<(std::ostream& os, const Point& pt) {
	os << "[" << pt.x << ", " << pt.y << ", " << pt.z << "]";
	return os;
}

extern const std::array<Point, 9664> POINTS;

