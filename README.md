# FindSurface-BasicDemo-Windows/Linux (C/C++)
**Curv*Surf* FindSurface™** BasicDemo for Windows/Linux (C/C++)

## Overview
This sample code demonstrates the basic usage of FindSurface for a simple task, which attempts to search for specific geometry shapes in point cloud data.

FindSurface Library ([Windows](https://github.com/CurvSurf/FindSurface-Windows), [Linux](https://github.com/CurvSurf/FindSurface-Linux)) is required to build the source code into a program. Download the framework [here (Windows)](https://github.com/CurvSurf/FindSurface-Windows/releases) or [here (Linux)](https://github.com/CurvSurf/FindSurface-Linux/releases).

To link the library with your project, add `-lFindSurface` flag to your linker options in case of using Linux and refer to [here](https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-160) for Windows.

## About the source code

In this demo, the following functions introduces how to call FindSurface API:

-  `runCTest` function in [main.c](src/c/main.c) file.
-  `Test::Cpp::Run` function in [main.cpp](src/cpp/main.cpp) file. 

There are two header files (`FindSurface.h`, `FindSurface.hpp`) in the distributed library representing C and C++ respectively. The latter contains a C++14 wrapper as an example to help you to use FindSurface API in an OOP style. You may write your own wrapper.

To call FindSurface API in this function is specified in 4 steps as follows:

### Obtaining FindSurface Context

C:

````C
#include <FindSurface.h>
...
  
FIND_SURFACE_CONTEXT fs_ctx = NULL;
createFindSurface(&fs_ctx);
````

C++:

````C++
#include <FindSurface.hpp>
...
  
auto context = FindSurface::Context::getInstance();
````

First of all, obtain the FindSurface context to call FindSurface APIs using this instance.

> Note: In C, the context must be created once in the program though the context isn't provided in a singleton instance, since FindSurface is not thread-safe and it does not allow multiple context instances currently due to the nature of its implementation.

### Setting Input point cloud and parameters

C:

````C
setMeasurementAccuracy(fs_ctx, 0.01f);
setMeanDistance(fs_ctx, 0.01f);
setPointCloudFloat(fs_ctx, POINTS, POINT_COUNT, sizeof(Point));
````

C++:

````c++
context->setMeasurementAccuracy(0.01f);
context->setMeanDistance(0.01f);
context->setPointCloudFloat(POINTS.data(), POINTS.size(), sizeof(Point));
````
When an application is ready for an input point cloud, pass it to FindSurface along with parameters related to the points. Refer to [here](https://github.com/CurvSurf/FindSurface#how-does-it-work) for the meanings of the parameters.

### Invoking FindSurface algorithm

C:

````C
void runTest(FIND_SURFACE_CONTEXT fs_ctx, FS_FEATURE_TYPE type, unsigned int seed_index, float seed_radius) {
  	...
    FS_FEATURE_RESULT result;
  	const FS_ERROR error = findSurface(fs_ctx, type, seed_index, seed_radius, &result);
  
  	switch (error) {
    case FS_NO_ERROR: ... break; // found
    case FS_NOT_FOUND: ... break;
    default:
        ... // error occurred.
    }
    ...
}
````

C++:

````c++
void Test::Cpp::runTest(std::shared_ptr<Context> context, FeatureType type, unsigned int seedIndex, float seedRadius) {
  	...
    try {
      	auto result = context->findSurface(type, seedIndex, seedRadius);
      
      	if (result.type != FeatureType::none) {
          	printResult(result);
        }
      	else {
          	... // not found.
        }
    } catch (std::logic_error& error) { // base class of all exceptions related to FindSurface.
      	... // error occurred.
    }
}
````

The parameters of  `findSurface` method are composed of property `featureType`, property `seedIndex`, and float `seedRadius`. The `featureType` is an enum value of `FS_FEATURE_TYPE` (`FeatureType` in C++), which can be one of the five geometric shapes (i.e., `plane`, `sphere`, `cylinder`, `cone`, `torus`) and `any`, which means "try finding one of the five". Refer to [here](https://github.com/CurvSurf/FindSurface#how-does-it-work) for the detailed descriptions of the parameters.

The method in C returns an error code that represents the status of FindSurface context after its algorithm finishes the search, and if the method fails to detect any geometric shape, the method returns `FS_NOT_FOUND` enum value (`FeatureType::none` in C++). If succeeded, it fills the information about the geometry in the `result`, with the dummy error code `FS_NO_ERROR`.

FindSurface's error codes other than  `FS_NOT_FOUND` and `FS_NO_ERROR` mean it failed to execute its algorithm for some reasons (e.g., an invalid parameter value, out of memory). It is recommended to design your application defensively so that your application does not have to get any error except the "out of memory" case in run-time. Refer to API reference ([Windows](https://github.com/CurvSurf/FindSurface-Windows/blob/master/FindSurface-API-reference-C.md), [Linux](https://github.com/CurvSurf/FindSurface-Linux/blob/master/FindSurface-API-reference-C.md))for the cases of when FindSurface throws an error message.

In C++, the wrapper indirectly handles the error code and provide the result. When the algorithm failed to find any geometric surface, the result object of which `type` property is set to `FeatureType::none` is returned. In erroneous cases, the wrapper throws an exception corresponding to the error case.

### Fetching the Result

C:

````C
const float rms_error = result->rms;
````

C++:

````c++
const float rmsError = result.rmsError;
````

The `rms` property (`rmsError` in C++) describes the root-mean-squared value of errors between the inlier points and the detected surface. The value describes how much the points fits the geometric surface well and it is not related to the algorithm's accuracy. This value will get greater as the points have greater errors in measurement, which means the result also be affected by the errors.

C:

````C
printf("%s (rms error: %g)\n", getCapTypeName(result->type), rms_error);
switch (result->type) {
            
case FS_TYPE_PLANE:
		auto& plane = result->plane_param;
		printf("\tLowerLeft: [%g, %g, %g]\n", plane.ll[0], plane.ll[1], plane.ll[2]);
		printf("\tLowerRight: [%g, %g, %g]\n", plane.lr[0], plane.lr[1], plane.lr[2]);
		printf("\tUpperRight: [%g, %g, %g]\n", plane.ur[0], plane.ur[1], plane.ur[2]);
 		printf("\tUpperLeft: [%g, %g, %g]\n", plane.ul[0], plane.ul[1], plane.ul[2]);
		break;
   
    ...
      
		default:
		printf("This text should never be printed.\n"); // and actually will never be printed
		break;
}
````

C++:

````c++
std::cout << getTypeName(result.type, true) << " (rms error: " << rmsError << ")" << std::endl;

switch (result.type) {
  case FeatureType::plane: std::cout << result.param.plane << std::endl; break;
  ...
}
````

The `type` property has a value of `FS_FEATURE_TYPE` (`FeatureType` in C++) and can be one of the five types. The type will be the same as the input parameter, except for several special cases (refer to [Auto Detection](TBD) and [Smart Conversion](TBD)). Since the result type cannot be set to `any`, the `default` section will never be executed. 



## About point cloud

The point cloud in this demo is the same as the sample used in FindSurface WebDemo. Please refer to the [WebDemo](https://developers.curvsurf.com/WebDemo/) for a visual representation of FindSurface's results. 
