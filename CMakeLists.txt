cmake_minimum_required(VERSION 3.0.2)
project(nodka_io_driver)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

# System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS system)

# Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   InputPortStatus.msg
# )

# Generate services in the 'srv' folder
add_service_files(
  FILES
  setOutputPort.srv
  getInputPort.srv
)

# Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

###################################
## catkin specific configuration ##
###################################

catkin_package(
# INCLUDE_DIRS include
# LIBRARIES nodka_io_driver
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
# DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node 
  src/nodka_io_driver_node.cpp
  src/nodka_io_driver.cpp
)

set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "nodka_io_driver_")

add_dependencies(${PROJECT_NAME}_node 
  ${${PROJECT_NAME}_EXPORTED_TARGETS} 
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${PROJECT_SOURCE_DIR}/lib/libNKIOLCLIB.a
)