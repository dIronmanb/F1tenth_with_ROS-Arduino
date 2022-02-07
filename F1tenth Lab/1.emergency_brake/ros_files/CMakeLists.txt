cmake_minimum_required(VERSION 3.0.2)
project(lab1)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_package(catkin REQUIRED COMPONENTS
  ackermann_msgs
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs  
  message_generation
)


add_message_files(FILES scan_range.msg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
#  INCLUDE_DIRS include
   LIBRARIES lab1
   CATKIN_DEPENDS roscpp std_msgs message_runtime
   DEPENDS system_lib
)

include_directories(
   include ${catkin_INCLUDE_DIRS}
)

add_executable(lidar_processing src/lidar_processing.cpp)
target_link_libraries(lidar_processing ${catkin_LIBRARIES})
add_dependencies(lidar_processing lab1_generate_messages_cpp)

add_executable(safety_node src/safety_node.cpp)
target_link_libraries(safety_node ${catkin_LIBRARIES})
