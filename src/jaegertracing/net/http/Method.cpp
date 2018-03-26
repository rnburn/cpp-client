/*
 * Copyright (c) 2017-2018 Uber Technologies, Inc.
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

#include "jaegertracing/net/http/Method.h"
#include <algorithm>
#include <initializer_list>
#include <iterator>

namespace jaegertracing {
namespace net {
namespace http {

Method parseMethod(const std::string& methodName)
{
    static constexpr auto kMethodNames = { "OPTIONS", "GET",    "HEAD",
                                           "POST",    "PUT",    "DELETE",
                                           "TRACE",   "CONNECT" };

    auto itr =
        std::find(std::begin(kMethodNames), std::end(kMethodNames), methodName);
    if (itr == std::end(kMethodNames)) {
        return Method::EXTENSION;
    }
    return static_cast<Method>(std::distance(std::begin(kMethodNames), itr));
}

}  // namespace http
}  // namespace net
}  // namespace jaegertracing
