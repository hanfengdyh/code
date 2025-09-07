#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_control_interfaces/action/move_robot.hpp"

using namespace std::chrono_literals;
using MoveRobot = robot_control_interfaces::action::MoveRobot;
using GoalHandleMoveRobot = rclcpp_action::ClientGoalHandle<MoveRobot>;

class MoveRobotActionClient : public rclcpp::Node {
public:
    MoveRobotActionClient()
    : Node("move_robot_action_client")
    {
        client_ptr_ = rclcpp_action::create_client<MoveRobot>(
            this,
            "move_robot");
    }

    void send_goal(const std::string &command, int32_t mode, int32_t speed,
                   float duration, int32_t amplitude, int32_t times)
    {
        if (!client_ptr_->wait_for_action_server(5s)) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available!");
            return;
        }

        auto goal_msg = MoveRobot::Goal();
        goal_msg.command = command;
        goal_msg.mode = mode;
        goal_msg.speed = speed;
        goal_msg.duration = duration;
        goal_msg.amplitude = amplitude;
        goal_msg.times = times;

        RCLCPP_INFO(this->get_logger(), "Sending goal: %s", command.c_str());

        auto send_goal_options = rclcpp_action::Client<MoveRobot>::SendGoalOptions();

        // Iron版：goal_response_callback直接用GoalHandleMoveRobot::SharedPtr
        send_goal_options.goal_response_callback =
            std::bind(&MoveRobotActionClient::goal_response_callback, this, std::placeholders::_1);

        send_goal_options.feedback_callback =
            std::bind(&MoveRobotActionClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2);

        send_goal_options.result_callback =
            std::bind(&MoveRobotActionClient::result_callback, this, std::placeholders::_1);

        client_ptr_->async_send_goal(goal_msg, send_goal_options);
    }

private:
    rclcpp_action::Client<MoveRobot>::SharedPtr client_ptr_;

    // 修改参数类型
    void goal_response_callback(GoalHandleMoveRobot::SharedPtr goal_handle)
    {
        if (!goal_handle) {
            RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
        } else {
            RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result...");
        }
    }

    void feedback_callback(
        GoalHandleMoveRobot::SharedPtr,
        const std::shared_ptr<const MoveRobot::Feedback> feedback)
    {
        RCLCPP_INFO(this->get_logger(), "Feedback: progress=%.1f%%, status=%s, count=%d",
                    feedback->progress, feedback->execution_status.c_str(), feedback->count);
    }

    void result_callback(const GoalHandleMoveRobot::WrappedResult &result)
{
    switch (result.code) {
        case rclcpp_action::ResultCode::SUCCEEDED:
            RCLCPP_INFO(this->get_logger(), "Result: SUCCEEDED");
            break;
        case rclcpp_action::ResultCode::ABORTED:
            RCLCPP_INFO(this->get_logger(), "Result: ABORTED");
            break;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_INFO(this->get_logger(), "Result: CANCELED");
            break;
        default:
            RCLCPP_INFO(this->get_logger(), "Result: UNKNOWN");
            break;
    }

    auto res = result.result;
    if (res) {
        RCLCPP_INFO(this->get_logger(),
                    "error_code=%d, message=%s, status=%s, progress=%.1f, count=%d, start_time=%ld",
                    res->error_code, res->error_message.c_str(), res->execution_status.c_str(),
                    res->progress, res->count, res->start_time);
    }

    // 不再调用 rclcpp::shutdown();
}
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto action_client = std::make_shared<MoveRobotActionClient>();

    // 例子：run（基于时间）
    // action_client->send_goal("run", 0, 50, 10.5, 0, 0);

    // 例子：push_up（基于次数）
    action_client->send_goal("status.routine.body.push_up", 1, 0, 0.0, 0, 5);

    rclcpp::spin(action_client);
    rclcpp::shutdown();
    return 0;
}

