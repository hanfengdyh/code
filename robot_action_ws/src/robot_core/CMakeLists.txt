cmake_minimum_required(VERSION 3.8)
project(robot_core)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(robot_control_interfaces REQUIRED)
find_package(nlohmann_json REQUIRED)

# 手动找到 hiredis
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIB hiredis)

add_executable(robot_core
  src/robot_core.cpp  
)
ament_target_dependencies(robot_core
  rclcpp
  rclcpp_action
  robot_control_interfaces
  nlohmann_json
)

target_include_directories(robot_core PRIVATE
  ${HIREDIS_INCLUDE_DIR}
)
target_link_libraries(robot_core
  nlohmann_json::nlohmann_json
   ${HIREDIS_LIB}
)
install(TARGETS robot_core
  DESTINATION lib/${PROJECT_NAME})
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
