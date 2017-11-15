#pragma once

extern "C" int make_libjaegertracing_tracer(const char* config,
                                            void* tracerSharedPtrPtr,
                                            void* errorMsgStringPtr);
