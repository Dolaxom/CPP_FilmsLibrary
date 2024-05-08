#include "redis-wrapper.h"

#include <stdexcept>

#include "utils/logger.h"

namespace fm {

Redis::Redis(const std::string& host, int32_t port) {
  context = redisConnect(host.c_str(), port);
  if (context != nullptr && context->err) {
    throw std::runtime_error(context->errstr);
  } else {
    Logger::Get().Write("Success Redis connection.");
  }
}

Redis::~Redis() { redisFree(context); }

redisResponse set(Redis& redis, const std::string& key, const std::string& value) {
  redisReply* reply;
  reply = static_cast<redisReply*>(redisCommand(redis.context, "SETEX %s 10 %s", key.c_str(), value.c_str()));

  std::string replyStr(reply->str, reply->len);
  int8_t status = reply->type;

  freeReplyObject(reply);
  return {status, replyStr};
}

redisResponse get(Redis& redis, const std::string& key) {
  redisReply* reply;
  reply = static_cast<redisReply*>(redisCommand(redis.context, "GET %s", key.c_str()));

  std::string replyStr(reply->str, reply->len);
  int8_t status = reply->type;

  freeReplyObject(reply);
  return {status, replyStr};
}

}  // namespace fm