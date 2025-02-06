#include <iostream>
#include <FindSurface.hpp>

#include "pointcloud.hpp"
#include "helper.hpp"

namespace Test {
    
    using namespace FindSurface;

    struct Cpp {

        static void run();

    private:
        static void runTest(
            std::shared_ptr<Context> context,
            FeatureType type,
            unsigned int seedIndex,
            float seedRadius
        );
        static const char* getTypeName(FeatureType type, bool capitalized = false);
        static void printResult(const Result& result);
    };
};

int main()
{
    Test::Cpp::run();
    return 0;
}

void Test::Cpp::run() {
    
    const float measurementAccuracy = 0.01f;
    const float meanDistance = 0.01f;
    const float seedRadius = 0.025f;

    auto context = Context::getInstance();

    context->setMeasurementAccuracy(measurementAccuracy);
    context->setMeanDistance(meanDistance);
    try {
        context->setPointCloudDataFloat(POINTS.data(), POINTS.size(), sizeof(Point));
    } catch (Exception::InvalidOperation e) {
		printf("Error! POINT_COUNT exceeds the limitation on input point clouds. Refer to the GitHub documentation (github.com/CurvSurf/FindSurface-x86_64) for details.");
        return;
    } catch (Exception::InvalidArgument e) {
        printf("Error! It might be one of the following error cases: the pointer is nullptr, the count is zero, or the stride is neither zero nor at least three times the size of a float or double.");
        return;
    } catch(...) {
        printf("Unknown Error!");
        return;
    }
    
    std::cout << "Normal cases: " << std::endl;

    int trial = 1;
    for (auto preset : Preset::normalList) {
        std::cout << trial++ << ". ";

        runTest(context, preset.featureType, preset.seedIndex, seedRadius);
    }

    std::cout << "Smart cases: " << std::endl;
    int smartOptions = FS_SCO_CONE_TO_CYLINDER
                     | FS_SCO_TORUS_TO_SPHERE
                     | FS_SCO_TORUS_TO_CYLINDER;
    context->setSmartConversionOptions(smartOptions);

    trial = 1;
    for (auto preset : Preset::smartList) {
        std::cout << trial++ << ". ";

        runTest(context, preset.featureType, preset.seedIndex, seedRadius);
    }
}

void Test::Cpp::runTest(std::shared_ptr<Context> context, FeatureType type, unsigned int seedIndex, float seedRadius) {

    std::cout << "FindSurface searched for a " << getTypeName(type) << std::endl;
    std::cout << "around the point of which index is " << seedIndex << std::endl;

    try {
        auto result = context->findSurface(type, seedIndex, seedRadius);

        if (result.type != FeatureType::none) {
            printResult(result);
        }
        else {
            std::cout << "Not found." << std::endl;
        }
    }
    catch (std::logic_error& error) {
        std::cout << "Couldn't run FindSurface due to the following error: " << std::endl;
        std::cout << error.what() << std::endl;
    }

    std::cout << std::endl;
}

const char* Test::Cpp::getTypeName(const FeatureType type, bool capitalized) {
    
    switch (type) {
    case FeatureType::plane: return capitalized ? "Plane" : "plane";
    case FeatureType::sphere: return capitalized ? "Sphere" : "sphere";
    case FeatureType::cylinder: return capitalized ? "Cylinder" : "cylinder";
    case FeatureType::cone: return capitalized ? "Cone" : "cone";
    case FeatureType::torus: return capitalized ? " Torus" : "torus";
    case FeatureType::any: return capitalized ? "Any shape" : "any shape";
    case FeatureType::none: return capitalized ? "Nothing" : "nothing";
    default: return capitalized ? "Unknown" : "unknown";
    }
}

void Test::Cpp::printResult(const Result& result) {
    
    const float rmsError = result.rmsError;
    std::cout << getTypeName(result.type, true) << " (rms error: " << rmsError << ")" << std::endl;

    switch (result.type) {
    case FeatureType::plane: std::cout << result.param.plane << std::endl; break;
    case FeatureType::sphere: std::cout << result.param.sphere << std::endl; break;
    case FeatureType::cylinder: std::cout << result.param.cylinder << std::endl; break;
    case FeatureType::cone: std::cout << result.param.cone << std::endl; break;
    case FeatureType::torus: std::cout << result.param.torus << std::endl; break;
    }
}