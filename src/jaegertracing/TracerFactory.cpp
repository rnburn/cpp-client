#include "TracerFactory.h"

namespace jaegertracing {

opentracing::expected<std::shared_ptr<opentracing::Tracer>>
TracerFactory::MakeTracer(const char* configuration,
                          std::string& error_message) const noexcept
{
  return std::shared_ptr<opentracing::Tracer>{};
}
}  // namespace jaegertracing
