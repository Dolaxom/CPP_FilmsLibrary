#pragma once

#include <cstdint>
#include <string>

#include "json.hpp"

using json = nlohmann::json;

#include <hiredis/hiredis.h>

namespace fm {

using redisResponse = std::pair<int8_t, std::string>;  // status code, reply string

class Redis {
 public:
  Redis(const std::string& host, int32_t port);
  ~Redis();
  Redis(const Redis& other) = delete;
  Redis(Redis&& other) = delete;

 private:
  redisContext* context;

  friend redisResponse set(Redis& redis, const std::string& key, const std::string& value);
  friend redisResponse get(Redis& redis, const std::string& key);
};

}  // namespace fm
