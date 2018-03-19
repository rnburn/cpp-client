/*
 * Copyright (c) 2017 Uber Technologies, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "jaegertracing/TracerFactory.h"
#include <gtest/gtest.h>

namespace jaegertracing {
TEST(TracerFactory, testInvalidConfig)
{
    const char* invalidConfigTestCases[] = { "",
                                             "abc: {",
                                             R"({
      "service_name": {}
    })",
                                             R"({
      "service_name": "t",
      "badField": 97
    })" };
    TracerFactory tracerFactory;
    for (auto&& invalidConfig : invalidConfigTestCases) {
        std::string errorMessage;
        auto tracerMaybe =
            tracerFactory.MakeTracer(invalidConfig, errorMessage);
        ASSERT_FALSE(tracerMaybe);
        ASSERT_NE(errorMessage, "");
    }
}

TEST(TracerFactory, testValidConfig)
{
    const char* config = R"(
  {
    "service_name": "test",
    "disabled": true,
    "sampler": {
      "type": "probabilistic",
      "param": 0.001
    },
    "reporter": {
      "queueSize": 100,
      "bufferFlushInterval": 10,
      "logSpans": false,
      "localAgentHostPort": "127.0.0.1:6831"
    },
    "headers": {
      "jaegerDebugHeader": "debug-id",
      "jaegerBaggageHeader": "baggage",
      "TraceContextHeaderName": "trace-id",
      "traceBaggageHeaderPrefix": "testctx-"
    },
    "baggage_restrictions": {
        "denyBaggageOnInitializationFailure": false,
        "hostPort": "127.0.0.1:5778",
        "refreshInterval": 60
    }
  })";
    TracerFactory tracerFactory;
    std::string errorMessage;
    auto tracerMaybe = tracerFactory.MakeTracer(config, errorMessage);
    ASSERT_EQ(errorMessage, "");
    ASSERT_TRUE(tracerMaybe);
}
}  // namespace jaegertracing
