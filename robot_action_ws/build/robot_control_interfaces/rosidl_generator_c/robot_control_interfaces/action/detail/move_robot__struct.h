// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_control_interfaces:action/MoveRobot.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_CONTROL_INTERFACES__ACTION__DETAIL__MOVE_ROBOT__STRUCT_H_
#define ROBOT_CONTROL_INTERFACES__ACTION__DETAIL__MOVE_ROBOT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'command'
#include "rosidl_runtime_c/string.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_Goal
{
  /// 指令名称: "run", "status.routine.body.push_up", "status.routine.body.wave"
  rosidl_runtime_c__String command;
  /// 模式: 0=基于时间, 1=基于次数
  int32_t mode;
  /// 速度 (适用 run / wave)
  int32_t speed;
  /// 持续时间 (仅当 mode=0 时有效)
  float duration;
  /// 振幅 (适用 wave)
  int32_t amplitude;
  /// 执行次数 (仅当 mode=1 时有效)
  int32_t times;
} robot_control_interfaces__action__MoveRobot_Goal;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_Goal.
typedef struct robot_control_interfaces__action__MoveRobot_Goal__Sequence
{
  robot_control_interfaces__action__MoveRobot_Goal * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_Goal__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'error_message'
// Member 'execution_status'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_Result
{
  /// 错误码, 0=成功, 非0=失败
  int32_t error_code;
  /// 错误描述
  rosidl_runtime_c__String error_message;
  /// "succeeded" / "failed" / "aborted"
  rosidl_runtime_c__String execution_status;
  /// 完成度 0~100
  float progress;
  /// 已完成次数 (适用于 push_up / wave)
  int32_t count;
  /// 任务开始时间戳
  int64_t start_time;
} robot_control_interfaces__action__MoveRobot_Result;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_Result.
typedef struct robot_control_interfaces__action__MoveRobot_Result__Sequence
{
  robot_control_interfaces__action__MoveRobot_Result * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_Result__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'execution_status'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_Feedback
{
  /// 实时进度百分比
  float progress;
  /// "running" / "idle" / "failed"
  rosidl_runtime_c__String execution_status;
  /// 错误码 (执行中出现问题)
  int32_t error_code;
  /// 已完成次数 (适用于基于次数的动作)
  int32_t count;
} robot_control_interfaces__action__MoveRobot_Feedback;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_Feedback.
typedef struct robot_control_interfaces__action__MoveRobot_Feedback__Sequence
{
  robot_control_interfaces__action__MoveRobot_Feedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_Feedback__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'goal'
#include "robot_control_interfaces/action/detail/move_robot__struct.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_SendGoal_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
  robot_control_interfaces__action__MoveRobot_Goal goal;
} robot_control_interfaces__action__MoveRobot_SendGoal_Request;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_SendGoal_Request.
typedef struct robot_control_interfaces__action__MoveRobot_SendGoal_Request__Sequence
{
  robot_control_interfaces__action__MoveRobot_SendGoal_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_SendGoal_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_SendGoal_Response
{
  bool accepted;
  builtin_interfaces__msg__Time stamp;
} robot_control_interfaces__action__MoveRobot_SendGoal_Response;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_SendGoal_Response.
typedef struct robot_control_interfaces__action__MoveRobot_SendGoal_Response__Sequence
{
  robot_control_interfaces__action__MoveRobot_SendGoal_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_SendGoal_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  robot_control_interfaces__action__MoveRobot_SendGoal_Event__request__MAX_SIZE = 1
};
// response
enum
{
  robot_control_interfaces__action__MoveRobot_SendGoal_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_SendGoal_Event
{
  service_msgs__msg__ServiceEventInfo info;
  robot_control_interfaces__action__MoveRobot_SendGoal_Request__Sequence request;
  robot_control_interfaces__action__MoveRobot_SendGoal_Response__Sequence response;
} robot_control_interfaces__action__MoveRobot_SendGoal_Event;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_SendGoal_Event.
typedef struct robot_control_interfaces__action__MoveRobot_SendGoal_Event__Sequence
{
  robot_control_interfaces__action__MoveRobot_SendGoal_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_SendGoal_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_GetResult_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
} robot_control_interfaces__action__MoveRobot_GetResult_Request;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_GetResult_Request.
typedef struct robot_control_interfaces__action__MoveRobot_GetResult_Request__Sequence
{
  robot_control_interfaces__action__MoveRobot_GetResult_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_GetResult_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'result'
// already included above
// #include "robot_control_interfaces/action/detail/move_robot__struct.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_GetResult_Response
{
  int8_t status;
  robot_control_interfaces__action__MoveRobot_Result result;
} robot_control_interfaces__action__MoveRobot_GetResult_Response;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_GetResult_Response.
typedef struct robot_control_interfaces__action__MoveRobot_GetResult_Response__Sequence
{
  robot_control_interfaces__action__MoveRobot_GetResult_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_GetResult_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
// already included above
// #include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  robot_control_interfaces__action__MoveRobot_GetResult_Event__request__MAX_SIZE = 1
};
// response
enum
{
  robot_control_interfaces__action__MoveRobot_GetResult_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_GetResult_Event
{
  service_msgs__msg__ServiceEventInfo info;
  robot_control_interfaces__action__MoveRobot_GetResult_Request__Sequence request;
  robot_control_interfaces__action__MoveRobot_GetResult_Response__Sequence response;
} robot_control_interfaces__action__MoveRobot_GetResult_Event;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_GetResult_Event.
typedef struct robot_control_interfaces__action__MoveRobot_GetResult_Event__Sequence
{
  robot_control_interfaces__action__MoveRobot_GetResult_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_GetResult_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'feedback'
// already included above
// #include "robot_control_interfaces/action/detail/move_robot__struct.h"

/// Struct defined in action/MoveRobot in the package robot_control_interfaces.
typedef struct robot_control_interfaces__action__MoveRobot_FeedbackMessage
{
  unique_identifier_msgs__msg__UUID goal_id;
  robot_control_interfaces__action__MoveRobot_Feedback feedback;
} robot_control_interfaces__action__MoveRobot_FeedbackMessage;

// Struct for a sequence of robot_control_interfaces__action__MoveRobot_FeedbackMessage.
typedef struct robot_control_interfaces__action__MoveRobot_FeedbackMessage__Sequence
{
  robot_control_interfaces__action__MoveRobot_FeedbackMessage * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_interfaces__action__MoveRobot_FeedbackMessage__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_CONTROL_INTERFACES__ACTION__DETAIL__MOVE_ROBOT__STRUCT_H_
