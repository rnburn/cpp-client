#include "TracerFactory.h"

#include "jaegertracing/Tracer.h"

namespace jaegertracing {

opentracing::expected<std::shared_ptr<opentracing::Tracer>>
TracerFactory::MakeTracer(const char* configuration,
                          std::string& error_message) const noexcept
try {
  YAML::Node yaml;
  try {
    yaml = YAML::Load(configuration);
  } catch(const YAML::ParserException& e) {
    error_message = e.what();
    return opentracing::make_unexpected(opentracing::configuration_parse_error);
  }

  const auto serviceNameNode = yaml["service_name"];
  if (!serviceNameNode) {
    error_message = "`service_name` not provided";
    return opentracing::make_unexpected(
        opentracing::invalid_configuration_error);
  }
  if (!serviceNameNode.IsScalar()) {
    error_message = "`service_name` must be a string";
    return opentracing::make_unexpected(
        opentracing::invalid_configuration_error);
  }
  std::string serviceName = serviceNameNode.Scalar();

  const auto tracerConfig = jaegertracing::Config::parse(yaml);
  return jaegertracing::Tracer::make(serviceName, tracerConfig);
} catch (const std::bad_alloc&) {
    return opentracing::make_unexpected(
        std::make_error_code(std::errc::not_enough_memory));
} catch (const std::exception& e) {
    error_message = e.what();
    return opentracing::make_unexpected(
        opentracing::invalid_configuration_error);
}
}  // namespace jaegertracing
