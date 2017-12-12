#include "jaegertracing/DynamicLoad.h"

#include <cstring>
#include <system_error>

#include <opentracing/dynamic_load.h>

#include "jaegertracing/Tracer.h"
#include "TracerFactory.h"

int make_libjaegertracing_tracer(const char* config,
                                 void* tracerSharedPtrPtr,
                                 void* errorMsgStringPtr)
{
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

int 
opentracing_make_tracer_factory(const char* opentracingVersion,
                                const void** errorCategory,
                                void** tracerFactory) {
    if (std::strcmp(opentracingVersion, OPENTRACING_VERSION) != 0) {
        *errorCategory = static_cast<const void*>(
            &opentracing::dynamic_load_error_category());
        return opentracing::incompatible_library_versions_error.value();
    }

    *tracerFactory = new (std::nothrow) jaegertracing::TracerFactory{};
    if (*tracerFactory == nullptr) {
        *errorCategory = static_cast<const void*>(&std::generic_category());
        return static_cast<int>(std::errc::not_enough_memory);
    }

    return 0;
}
