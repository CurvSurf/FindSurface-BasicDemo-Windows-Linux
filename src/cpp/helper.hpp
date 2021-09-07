#pragma once

#include <iostream>
#include <type_traits>
#include <FindSurface.h>

#include "pointcloud.hpp"

using PlaneParam_t = decltype(std::declval<FS_FEATURE_RESULT>().plane_param);
using SphereParam_t = decltype(std::declval<FS_FEATURE_RESULT>().sphere_param);
using CylinderParam_t = decltype(std::declval<FS_FEATURE_RESULT>().cylinder_param);
using ConeParam_t = decltype(std::declval<FS_FEATURE_RESULT>().cone_param);
using TorusParam_t = decltype(std::declval<FS_FEATURE_RESULT>().torus_param);

inline std::ostream& operator<<(std::ostream& os, const PlaneParam_t& param) {
	os << "\tLower Left: " << reinterpret_cast<const Point&>(param.ll) << std::endl;
	os << "\tLower Right: " << reinterpret_cast<const Point&>(param.lr) << std::endl;
	os << "\tUpper Right: " << reinterpret_cast<const Point&>(param.ur) << std::endl;
	os << "\tUpper Left: " << reinterpret_cast<const Point&>(param.ul) << std::endl;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const SphereParam_t& param) {
	os << "\tCenter: " << reinterpret_cast<const Point&>(param.c) << std::endl;
	os << "\tRadius: " << param.r << std::endl;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const CylinderParam_t& param) {
	os << "\tBottom Center: " << reinterpret_cast<const Point&>(param.b) << std::endl;
	os << "\tTop Center: " << reinterpret_cast<const Point&>(param.t) << std::endl;
	os << "\tRadius: " << param.r << std::endl;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const ConeParam_t& param) {
	os << "\tBottom Center: " << reinterpret_cast<const Point&>(param.b) << std::endl;
	os << "\tTop Center: " << reinterpret_cast<const Point&>(param.t) << std::endl;
	os << "\tBottom Radius: " << param.br << std::endl;
	os << "\tTop Radius: " << param.tr << std::endl;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const TorusParam_t& param) {
	os << "\tCenter: " << reinterpret_cast<const Point&>(param.c) << std::endl;
	os << "\tAxis: " << reinterpret_cast<const Point&>(param.n) << std::endl;
	os << "\tMean Radius: " << param.mr << std::endl;
	os << "\tTube Radius: " << param.tr << std::endl;
	return os;
}