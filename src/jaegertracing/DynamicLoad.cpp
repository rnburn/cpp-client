#include "jaegertracing/DynamicLoad.h"

#include "jaegertracing/Tracer.h"
#include <iostream>
#include <sstream>

int make_libjaegertracing_tracer(const char* config,
                                 void* tracerSharedPtrPtr,
                                 void* errorMsgStringPtr)
{
    // When running on Docker-ubuntu the integers are ignored by iostreams.
    std::istringstream iss{"123"};
    int x = 0;
    iss >> x;
    std::cout << "x = " << x << std::endl;

    auto& tracer = *static_cast<std::shared_ptr<opentracing::Tracer>*>(
        tracerSharedPtrPtr);
    auto& errorMsg = *static_cast<std::string*>(errorMsgStringPtr);
    try {
        const auto yaml = YAML::Load(config);
        const auto serviceName = yaml["service_name"].as<std::string>();
        const auto tracerConfig = jaegertracing::Config::parse(yaml);
        tracer = jaegertracing::Tracer::make(serviceName, tracerConfig);
        return 0;
    } catch (const std::exception& e) {
        errorMsg.assign(e.what());
        return -1;
    }
}
