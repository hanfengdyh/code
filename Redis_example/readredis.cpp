#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include <map>

class RedisClient {
public:
    RedisClient(const std::string& host = "127.0.0.1", int port = 6379) {
        c_ = redisConnect(host.c_str(), port);
        if (!c_ || c_->err) {
            throw std::runtime_error(std::string("Redis connection error: ") + (c_ ? c_->errstr : "cannot allocate context"));
        }
    }

    ~RedisClient() {
        if (c_) redisFree(c_);
    }

    // 写入单条
    bool set(const std::string& key, const std::string& json) {
        redisReply* reply = (redisReply*)redisCommand(c_, "SET %s %s", key.c_str(), json.c_str());
        if (!reply) return false;
        bool success = reply->type == REDIS_REPLY_STATUS && std::string(reply->str) == "OK";
        freeReplyObject(reply);
        return success;
    }

    // 批量写入
    void setBatch(const std::map<std::string, std::string>& kvs) {
        for (const auto& kv : kvs) {
            if (set(kv.first, kv.second)) {
                std::cout << "[OK] " << kv.first << std::endl;
            } else {
                std::cerr << "[FAIL] " << kv.first << std::endl;
            }
        }
    }

    // 读取单条
    std::string get(const std::string& key) {
        redisReply* reply = (redisReply*)redisCommand(c_, "GET %s", key.c_str());
        if (!reply) return "";
        std::string value;
        if (reply->type == REDIS_REPLY_STRING) {
            value = reply->str;
        } else {
            std::cerr << "GET " << key << " failed or key does not exist." << std::endl;
        }
        freeReplyObject(reply);
        return value;
    }

private:
    redisContext* c_;
};

int main() {
    try {
        RedisClient redis;

        // 读取单条
        std::string run_state = redis.get("run");
        std::cout << "run: " << run_state << std::endl;

        std::string wave_state = redis.get("status.routine.body.wave");
        std::cout << "status.routine.body.wave: " << wave_state << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

