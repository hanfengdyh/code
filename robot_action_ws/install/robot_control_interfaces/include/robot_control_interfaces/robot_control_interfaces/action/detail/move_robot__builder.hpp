// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_control_interfaces:action/MoveRobot.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_CONTROL_INTERFACES__ACTION__DETAIL__MOVE_ROBOT__BUILDER_HPP_
#define ROBOT_CONTROL_INTERFACES__ACTION__DETAIL__MOVE_ROBOT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_control_interfaces/action/detail/move_robot__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_Goal_times
{
public:
  explicit Init_MoveRobot_Goal_times(::robot_control_interfaces::action::MoveRobot_Goal & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_Goal times(::robot_control_interfaces::action::MoveRobot_Goal::_times_type arg)
  {
    msg_.times = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Goal msg_;
};

class Init_MoveRobot_Goal_amplitude
{
public:
  explicit Init_MoveRobot_Goal_amplitude(::robot_control_interfaces::action::MoveRobot_Goal & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Goal_times amplitude(::robot_control_interfaces::action::MoveRobot_Goal::_amplitude_type arg)
  {
    msg_.amplitude = std::move(arg);
    return Init_MoveRobot_Goal_times(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Goal msg_;
};

class Init_MoveRobot_Goal_duration
{
public:
  explicit Init_MoveRobot_Goal_duration(::robot_control_interfaces::action::MoveRobot_Goal & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Goal_amplitude duration(::robot_control_interfaces::action::MoveRobot_Goal::_duration_type arg)
  {
    msg_.duration = std::move(arg);
    return Init_MoveRobot_Goal_amplitude(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Goal msg_;
};

class Init_MoveRobot_Goal_speed
{
public:
  explicit Init_MoveRobot_Goal_speed(::robot_control_interfaces::action::MoveRobot_Goal & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Goal_duration speed(::robot_control_interfaces::action::MoveRobot_Goal::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_MoveRobot_Goal_duration(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Goal msg_;
};

class Init_MoveRobot_Goal_mode
{
public:
  explicit Init_MoveRobot_Goal_mode(::robot_control_interfaces::action::MoveRobot_Goal & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Goal_speed mode(::robot_control_interfaces::action::MoveRobot_Goal::_mode_type arg)
  {
    msg_.mode = std::move(arg);
    return Init_MoveRobot_Goal_speed(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Goal msg_;
};

class Init_MoveRobot_Goal_command
{
public:
  Init_MoveRobot_Goal_command()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_Goal_mode command(::robot_control_interfaces::action::MoveRobot_Goal::_command_type arg)
  {
    msg_.command = std::move(arg);
    return Init_MoveRobot_Goal_mode(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Goal msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_Goal>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_Goal_command();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_Result_start_time
{
public:
  explicit Init_MoveRobot_Result_start_time(::robot_control_interfaces::action::MoveRobot_Result & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_Result start_time(::robot_control_interfaces::action::MoveRobot_Result::_start_time_type arg)
  {
    msg_.start_time = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Result msg_;
};

class Init_MoveRobot_Result_count
{
public:
  explicit Init_MoveRobot_Result_count(::robot_control_interfaces::action::MoveRobot_Result & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Result_start_time count(::robot_control_interfaces::action::MoveRobot_Result::_count_type arg)
  {
    msg_.count = std::move(arg);
    return Init_MoveRobot_Result_start_time(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Result msg_;
};

class Init_MoveRobot_Result_progress
{
public:
  explicit Init_MoveRobot_Result_progress(::robot_control_interfaces::action::MoveRobot_Result & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Result_count progress(::robot_control_interfaces::action::MoveRobot_Result::_progress_type arg)
  {
    msg_.progress = std::move(arg);
    return Init_MoveRobot_Result_count(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Result msg_;
};

class Init_MoveRobot_Result_execution_status
{
public:
  explicit Init_MoveRobot_Result_execution_status(::robot_control_interfaces::action::MoveRobot_Result & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Result_progress execution_status(::robot_control_interfaces::action::MoveRobot_Result::_execution_status_type arg)
  {
    msg_.execution_status = std::move(arg);
    return Init_MoveRobot_Result_progress(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Result msg_;
};

class Init_MoveRobot_Result_error_message
{
public:
  explicit Init_MoveRobot_Result_error_message(::robot_control_interfaces::action::MoveRobot_Result & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Result_execution_status error_message(::robot_control_interfaces::action::MoveRobot_Result::_error_message_type arg)
  {
    msg_.error_message = std::move(arg);
    return Init_MoveRobot_Result_execution_status(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Result msg_;
};

class Init_MoveRobot_Result_error_code
{
public:
  Init_MoveRobot_Result_error_code()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_Result_error_message error_code(::robot_control_interfaces::action::MoveRobot_Result::_error_code_type arg)
  {
    msg_.error_code = std::move(arg);
    return Init_MoveRobot_Result_error_message(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Result msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_Result>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_Result_error_code();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_Feedback_count
{
public:
  explicit Init_MoveRobot_Feedback_count(::robot_control_interfaces::action::MoveRobot_Feedback & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_Feedback count(::robot_control_interfaces::action::MoveRobot_Feedback::_count_type arg)
  {
    msg_.count = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Feedback msg_;
};

class Init_MoveRobot_Feedback_error_code
{
public:
  explicit Init_MoveRobot_Feedback_error_code(::robot_control_interfaces::action::MoveRobot_Feedback & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Feedback_count error_code(::robot_control_interfaces::action::MoveRobot_Feedback::_error_code_type arg)
  {
    msg_.error_code = std::move(arg);
    return Init_MoveRobot_Feedback_count(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Feedback msg_;
};

class Init_MoveRobot_Feedback_execution_status
{
public:
  explicit Init_MoveRobot_Feedback_execution_status(::robot_control_interfaces::action::MoveRobot_Feedback & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_Feedback_error_code execution_status(::robot_control_interfaces::action::MoveRobot_Feedback::_execution_status_type arg)
  {
    msg_.execution_status = std::move(arg);
    return Init_MoveRobot_Feedback_error_code(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Feedback msg_;
};

class Init_MoveRobot_Feedback_progress
{
public:
  Init_MoveRobot_Feedback_progress()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_Feedback_execution_status progress(::robot_control_interfaces::action::MoveRobot_Feedback::_progress_type arg)
  {
    msg_.progress = std::move(arg);
    return Init_MoveRobot_Feedback_execution_status(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_Feedback msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_Feedback>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_Feedback_progress();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_SendGoal_Request_goal
{
public:
  explicit Init_MoveRobot_SendGoal_Request_goal(::robot_control_interfaces::action::MoveRobot_SendGoal_Request & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Request goal(::robot_control_interfaces::action::MoveRobot_SendGoal_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Request msg_;
};

class Init_MoveRobot_SendGoal_Request_goal_id
{
public:
  Init_MoveRobot_SendGoal_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_SendGoal_Request_goal goal_id(::robot_control_interfaces::action::MoveRobot_SendGoal_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_MoveRobot_SendGoal_Request_goal(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_SendGoal_Request>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_SendGoal_Request_goal_id();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_SendGoal_Response_stamp
{
public:
  explicit Init_MoveRobot_SendGoal_Response_stamp(::robot_control_interfaces::action::MoveRobot_SendGoal_Response & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Response stamp(::robot_control_interfaces::action::MoveRobot_SendGoal_Response::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Response msg_;
};

class Init_MoveRobot_SendGoal_Response_accepted
{
public:
  Init_MoveRobot_SendGoal_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_SendGoal_Response_stamp accepted(::robot_control_interfaces::action::MoveRobot_SendGoal_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_MoveRobot_SendGoal_Response_stamp(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_SendGoal_Response>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_SendGoal_Response_accepted();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_SendGoal_Event_response
{
public:
  explicit Init_MoveRobot_SendGoal_Event_response(::robot_control_interfaces::action::MoveRobot_SendGoal_Event & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Event response(::robot_control_interfaces::action::MoveRobot_SendGoal_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Event msg_;
};

class Init_MoveRobot_SendGoal_Event_request
{
public:
  explicit Init_MoveRobot_SendGoal_Event_request(::robot_control_interfaces::action::MoveRobot_SendGoal_Event & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_SendGoal_Event_response request(::robot_control_interfaces::action::MoveRobot_SendGoal_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_MoveRobot_SendGoal_Event_response(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Event msg_;
};

class Init_MoveRobot_SendGoal_Event_info
{
public:
  Init_MoveRobot_SendGoal_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_SendGoal_Event_request info(::robot_control_interfaces::action::MoveRobot_SendGoal_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_MoveRobot_SendGoal_Event_request(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_SendGoal_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_SendGoal_Event>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_SendGoal_Event_info();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_GetResult_Request_goal_id
{
public:
  Init_MoveRobot_GetResult_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::robot_control_interfaces::action::MoveRobot_GetResult_Request goal_id(::robot_control_interfaces::action::MoveRobot_GetResult_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_GetResult_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_GetResult_Request>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_GetResult_Request_goal_id();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_GetResult_Response_result
{
public:
  explicit Init_MoveRobot_GetResult_Response_result(::robot_control_interfaces::action::MoveRobot_GetResult_Response & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_GetResult_Response result(::robot_control_interfaces::action::MoveRobot_GetResult_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_GetResult_Response msg_;
};

class Init_MoveRobot_GetResult_Response_status
{
public:
  Init_MoveRobot_GetResult_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_GetResult_Response_result status(::robot_control_interfaces::action::MoveRobot_GetResult_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_MoveRobot_GetResult_Response_result(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_GetResult_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_GetResult_Response>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_GetResult_Response_status();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_GetResult_Event_response
{
public:
  explicit Init_MoveRobot_GetResult_Event_response(::robot_control_interfaces::action::MoveRobot_GetResult_Event & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_GetResult_Event response(::robot_control_interfaces::action::MoveRobot_GetResult_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_GetResult_Event msg_;
};

class Init_MoveRobot_GetResult_Event_request
{
public:
  explicit Init_MoveRobot_GetResult_Event_request(::robot_control_interfaces::action::MoveRobot_GetResult_Event & msg)
  : msg_(msg)
  {}
  Init_MoveRobot_GetResult_Event_response request(::robot_control_interfaces::action::MoveRobot_GetResult_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_MoveRobot_GetResult_Event_response(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_GetResult_Event msg_;
};

class Init_MoveRobot_GetResult_Event_info
{
public:
  Init_MoveRobot_GetResult_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_GetResult_Event_request info(::robot_control_interfaces::action::MoveRobot_GetResult_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_MoveRobot_GetResult_Event_request(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_GetResult_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_GetResult_Event>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_GetResult_Event_info();
}

}  // namespace robot_control_interfaces


namespace robot_control_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveRobot_FeedbackMessage_feedback
{
public:
  explicit Init_MoveRobot_FeedbackMessage_feedback(::robot_control_interfaces::action::MoveRobot_FeedbackMessage & msg)
  : msg_(msg)
  {}
  ::robot_control_interfaces::action::MoveRobot_FeedbackMessage feedback(::robot_control_interfaces::action::MoveRobot_FeedbackMessage::_feedback_type arg)
  {
    msg_.feedback = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_FeedbackMessage msg_;
};

class Init_MoveRobot_FeedbackMessage_goal_id
{
public:
  Init_MoveRobot_FeedbackMessage_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveRobot_FeedbackMessage_feedback goal_id(::robot_control_interfaces::action::MoveRobot_FeedbackMessage::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_MoveRobot_FeedbackMessage_feedback(msg_);
  }

private:
  ::robot_control_interfaces::action::MoveRobot_FeedbackMessage msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_interfaces::action::MoveRobot_FeedbackMessage>()
{
  return robot_control_interfaces::action::builder::Init_MoveRobot_FeedbackMessage_goal_id();
}

}  // namespace robot_control_interfaces

#endif  // ROBOT_CONTROL_INTERFACES__ACTION__DETAIL__MOVE_ROBOT__BUILDER_HPP_
