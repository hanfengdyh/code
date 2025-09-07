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

    // 单条写入
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

private:
    redisContext* c_;
};

int main() {
    try {
        RedisClient redis;

        // 构造动作状态 JSON
        std::map<std::string, std::string> robot_states = {
            {"run", R"({"value":{"speed":50,"duration":10.5,"execution_status":"running","progress":45.2,"start_time":1234567890,"error_code":0}})"},
            {"status.routine.body.push_up", R"({"value":{"count":0,"execution_status":"idle","progress":0,"error_code":0}})"},
            {"status.routine.body.wave", R"({"value":{"speed":60,"amplitude":50,"times":3,"execution_status":"failed","progress":33.3,"error_code":1001,"error_message":"舵机故障"}})"}
            // 其他动作同理...
        };

        // 批量写入
        redis.setBatch(robot_states);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

