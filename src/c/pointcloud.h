#ifndef _POINTCLOUD_H_
#define _POINTCLOUD_H_

#include <FindSurface.h>

#define SPHERE_INDEX		7811
#define CYLINDER_INDEX		3437
#define CONE_INDEX			6637
#define TORUS_INDEX			7384
#define NORMAL_PRESET_COUNT	4
#define SMART_PRESET_COUNT	3
#define POINT_COUNT			9664

typedef struct {
	FS_FEATURE_TYPE feature_type;
	unsigned int seed_index;
} Preset;

extern const Preset NORMAL_PRESET_LIST[NORMAL_PRESET_COUNT];

extern const Preset SMART_PRESET_LIST[SMART_PRESET_COUNT];

typedef struct {
	float x, y, z;
} Point;

extern const Point POINTS[POINT_COUNT];

#endif