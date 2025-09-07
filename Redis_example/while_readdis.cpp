#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

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

    // 读取单条
    std::string get(const std::string& key) {
        redisReply* reply = (redisReply*)redisCommand(c_, "GET %s", key.c_str());
        if (!reply) return "";
        std::string value;
        if (reply->type == REDIS_REPLY_STRING) {
            value = reply->str;
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

        while (true) {
            std::string run_state = redis.get("run");
            std::string push_up_state = redis.get("status.routine.body.push_up");
            std::string wave_state = redis.get("status.routine.body.wave");

            std::cout << "run: " << run_state << std::endl;
            std::cout << "push_up: " << push_up_state << std::endl;
            std::cout << "wave: " << wave_state << std::endl;
            std::cout << "-----------------------------" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 每500ms读取一次
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

