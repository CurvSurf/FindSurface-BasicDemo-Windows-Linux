#include <stdio.h>

#include <FindSurface.h>

#include "pointcloud.h"

static void runCTest();

int main(int argc, const char* argv[])
{
	runCTest();
	return 0;
}

static void runTest(FIND_SURFACE_CONTEXT fs_ctx, FS_FEATURE_TYPE type, unsigned int seed_index, float seed_radius);

void runCTest() {

	const float measurement_accuracy = 0.01f;
	const float mean_distance = 0.01f;
	const float seed_radius = 0.025f;

	FIND_SURFACE_CONTEXT fs_ctx = NULL;
	createFindSurface(&fs_ctx);

	setMeasurementAccuracy(fs_ctx, measurement_accuracy);
	setMeanDistance(fs_ctx, mean_distance);
	setPointCloudFloat(fs_ctx, POINTS, POINT_COUNT, sizeof(Point));

	printf("Normal cases: \n");

	for (int trial = 0; trial < NORMAL_PRESET_COUNT; trial++) {
		const Preset* preset = &(NORMAL_PRESET_LIST[trial]);
		const FS_FEATURE_TYPE type = preset->feature_type;
		const unsigned int index = preset->seed_index;
		printf("%d. ", trial);
		runTest(fs_ctx, type, index, seed_radius);
	}

	printf("Smart cases: \n");
	int smart_options = FS_SCO_CONE_TO_CYLINDER 
					  | FS_SCO_TORUS_TO_SPHERE 
					  | FS_SCO_TORUS_TO_CYLINDER;
	setSmartConversionOptions(fs_ctx, smart_options);

	for (int trial = 0; trial < SMART_PRESET_COUNT; trial++) {
		const Preset* preset = &(SMART_PRESET_LIST[trial]);
		const FS_FEATURE_TYPE type = preset->feature_type;
		const unsigned int index = preset->seed_index;
		printf("%d. ", trial);
		runTest(fs_ctx, type, index, seed_radius);
	}
}

static const char* getTypeName(FS_FEATURE_TYPE type);
static void printResult(const FS_FEATURE_RESULT* result);

void runTest(FIND_SURFACE_CONTEXT fs_ctx, FS_FEATURE_TYPE type, unsigned int seed_index, float seed_radius) {
	
	printf("FindSurface searched for a %s\n", getTypeName(type));
	printf("around the point of which index is %d.\n", seed_index);

	FS_FEATURE_RESULT result;
	const FS_ERROR error = findSurface(fs_ctx, type, seed_index, seed_radius, &result);

	switch (error) {
	case FS_NO_ERROR: printResult(&result); break;
	case FS_NOT_FOUND: printf("Not found.\n"); break;
	default: 
		printf("Couldn't run FindSurface due to the following error: \n");
		printf("%s", getFindSurfaceErrorMessage(fs_ctx));
	}

	printf("\n");
}

const char* getTypeName(const FS_FEATURE_TYPE type) {
	switch (type) {
	case FS_TYPE_PLANE: return "plane";
	case FS_TYPE_SPHERE: return "sphere";
	case FS_TYPE_CYLINDER: return "cylinder";
	case FS_TYPE_CONE: return "cone";
	case FS_TYPE_TORUS: return "torus";
	case FS_TYPE_ANY: return "any shape";
	case FS_TYPE_NONE: return "nothing";
	default: return "unknown";
	}
}

static const char* getCapTypeName(const FS_FEATURE_TYPE type);

static void printFloat3(const char* fmt, const float float3[3]) {
	printf(fmt, float3[0], float3[1], float3[2]);
}

void printResult(const FS_FEATURE_RESULT* result) {
	
	const float rms_error = result->rms;
	printf("%s (rms error: %g)\n", getCapTypeName(result->type), rms_error);
	
	switch (result->type) {

	case FS_TYPE_PLANE: 
		printFloat3("\tLower Left: [%g, %g, %g]\n", result->plane_param.ll);
		printFloat3("\tLower Right: [%g, %g, %g]\n", result->plane_param.lr);
		printFloat3("\tUpper Right: [%g, %g, %g]\n", result->plane_param.ur);
		printFloat3("\tUpper Left: [%g, %g, %g]\n", result->plane_param.ul);
		break;

	case FS_TYPE_SPHERE:
		printFloat3("\tCenter: [%g, %g, %g]\n", result->sphere_param.c);
		printf("\tRadius: %g\n", result->sphere_param.r);
		break;

	case FS_TYPE_CYLINDER:
		printFloat3("\tBottom Center: [%g, %g, %g]\n", result->cylinder_param.b);
		printFloat3("\tTop Center: [%g, %g, %g]\n", result->cylinder_param.t);
		printf("\tRadius: %g\n", result->cylinder_param.r);
		break;

	case FS_TYPE_CONE:
		printFloat3("\tBottom Center: [%g, %g, %g]\n", result->cone_param.b);
		printFloat3("\tTop Center: [%g, %g, %g]\n", result->cone_param.t);
		printf("\tBottom Radius: %g\n", result->cone_param.br);
		printf("\tTop Radius: %g\n", result->cone_param.tr);
		break;

	case FS_TYPE_TORUS:
		printFloat3("\tCenter: [%g, %g, %g]\n", result->torus_param.c);
		printFloat3("\tAxis: [%g, %g, %g]\n", result->torus_param.n);
		printf("\tMean Radius: %g\n", result->torus_param.mr);
		printf("\tTube Radius: %g\n", result->torus_param.tr);
		break;

	default: 
		/*This text should never be printed and it actually will never be.*/
		break;
	}
}

const char* getCapTypeName(const FS_FEATURE_TYPE type) {
	switch (type) {
	case FS_TYPE_PLANE: return "Plane";
	case FS_TYPE_SPHERE: return "Sphere";
	case FS_TYPE_CYLINDER: return "Cylinder";
	case FS_TYPE_CONE: return "Cone";
	case FS_TYPE_TORUS: return "Torus";
	case FS_TYPE_ANY: return "Any shape";
	case FS_TYPE_NONE: return "Nothing";
	default: return "Unknown";
	}
}
