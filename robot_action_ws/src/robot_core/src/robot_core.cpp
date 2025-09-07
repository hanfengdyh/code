#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <map>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_control_interfaces/action/move_robot.hpp"

#include <hiredis/hiredis.h>
#include <nlohmann/json.hpp>

using MoveRobot = robot_control_interfaces::action::MoveRobot;
using GoalHandleMoveRobot = rclcpp_action::ServerGoalHandle<MoveRobot>;
using json = nlohmann::json;

class MoveRobotActionServer : public rclcpp::Node {
public:
    MoveRobotActionServer()
    : Node("move_robot_action_server")
    {
        using namespace std::placeholders;

        action_server_ = rclcpp_action::create_server<MoveRobot>(
            this,
            "move_robot",
            std::bind(&MoveRobotActionServer::handle_goal, this, _1, _2),
            std::bind(&MoveRobotActionServer::handle_cancel, this, _1),
            std::bind(&MoveRobotActionServer::handle_accepted, this, _1));

        // 连接 Redis
        redis_ = redisConnect("127.0.0.1", 6379);
        if (redis_ == nullptr || redis_->err) {
            throw std::runtime_error("Redis connection failed");
        }
    }

    ~MoveRobotActionServer() {
        if (redis_) {
            redisFree(redis_);
        }
    }

private:
    rclcpp_action::Server<MoveRobot>::SharedPtr action_server_;
    redisContext* redis_;

    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &,
        std::shared_ptr<const MoveRobot::Goal> goal)
    {
        RCLCPP_INFO(this->get_logger(), "Received goal request: %s", goal->command.c_str());
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleMoveRobot>)
    {
        RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted(const std::shared_ptr<GoalHandleMoveRobot> goal_handle)
    {
        std::thread{std::bind(&MoveRobotActionServer::execute, this, goal_handle)}.detach();
    }

    void execute(const std::shared_ptr<GoalHandleMoveRobot> goal_handle)
    {
        const auto goal = goal_handle->get_goal();
        auto feedback = std::make_shared<MoveRobot::Feedback>();
        auto result = std::make_shared<MoveRobot::Result>();

        // 从 Redis 轮询状态
        std::string redis_key = goal->command;
        RCLCPP_INFO(this->get_logger(), "Executing goal, key=%s", redis_key.c_str());

        for (int i = 0; i < 50; i++) {
            // 模拟查询 Redis
            redisReply* reply = (redisReply*)redisCommand(redis_, "GET %s", redis_key.c_str());
            if (reply && reply->type == REDIS_REPLY_STRING) {
                try {
                    json j = json::parse(reply->str);
                    auto v = j["value"];

                    // 更新 Feedback
                    feedback->progress = v.value("progress", 0.0f);
                    feedback->execution_status = v.value("execution_status", "unknown");
                    feedback->error_code = v.value("error_code", 0);
                    feedback->count = v.value("count", 0);
                    goal_handle->publish_feedback(feedback);

                    // 如果状态不是 running 就认为任务结束
                    if (feedback->execution_status != "running") {
                        result->error_code = v.value("error_code", 0);
                        result->error_message = v.value("error_message", "");
                        result->execution_status = feedback->execution_status;
                        result->progress = feedback->progress;
                        result->count = feedback->count;
                        result->start_time = v.value("start_time", (int64_t)0);

                        goal_handle->succeed(result);
                        freeReplyObject(reply);
                        return;
                    }
                } catch (std::exception &e) {
                    RCLCPP_ERROR(this->get_logger(), "JSON parse error: %s", e.what());
                }
            }
            if (reply) freeReplyObject(reply);

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        // 超时未完成
        result->error_code = -1;
        result->error_message = "Timeout";
        result->execution_status = "aborted";
        result->progress = feedback->progress;
        result->count = feedback->count;
        result->start_time = 0;
        goal_handle->abort(result);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MoveRobotActionServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

