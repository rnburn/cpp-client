#pragma once

extern "C" int make_libjaegertracing_tracer(const char* config,
                                            void* tracerSharedPtrPtr,
                                            void* errorMsgStringPtr);

extern "C" int __attribute((weak))
opentracing_make_tracer_factory(const char* opentracingVersion,
                                const void** errorCategory,
                                void** tracerFactory);
