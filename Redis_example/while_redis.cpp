#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <sstream>

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

    bool set(const std::string& key, const std::string& json) {
        redisReply* reply = (redisReply*)redisCommand(c_, "SET %s %s", key.c_str(), json.c_str());
        if (!reply) return false;
        bool success = reply->type == REDIS_REPLY_STATUS && std::string(reply->str) == "OK";
        freeReplyObject(reply);
        return success;
    }

private:
    redisContext* c_;
};

int main() {
    try {
        RedisClient redis;

        int msg_count = 500;          // 每秒写入 500 条消息
        int interval_ms = 1000;       // 每秒循环一次
        int loop_index = 0;

        while (true) {
            auto start = std::chrono::steady_clock::now();

            for (int i = 0; i < msg_count; ++i) {
                std::stringstream key;
                key << "robot_state_" << loop_index << "_" << i;

                // 构造 JSON 示例
                std::stringstream json;
                json << R"({"value":{"speed":)"
                     << (i % 100)  // 模拟不同速度
                     << R"(,"duration":)"
                     << (10.0 + i * 0.1)
                     << R"(,"execution_status":"running","progress":)"
                     << (i % 100)
                     << R"(,"start_time":1234567890,"error_code":0}})";

                redis.set(key.str(), json.str());
            }

            loop_index++;

            // 控制每秒写入 500 条
            auto end = std::chrono::steady_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (elapsed_ms < interval_ms) {
                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms - elapsed_ms));
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

