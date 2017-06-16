project(object_reg)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -O2 -march=native" )

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS 
   image_transport
   roscpp 
   std_msgs
   cv_bridge
   image_transport
   message_generation
   sensor_msgs
)

# opencv
set(OpenCV_DIR /usr/local/opencv-2.4.9/share/OpenCV)
find_package(OpenCV 2.4 REQUIRED)
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS} /usr/local/opencv-2.4.9/include)
link_directories(${OpenCV_LIBRARY_DIRS} /usr/local/opencv-2.4.9/lib)

add_message_files(
   FILES
   pictureflag.msg
   pictureresult.msg
)

generate_messages(
   DEPENDENCIES
   std_msgs
)

catkin_package(
   INCLUDE_DIRS include
   LIBRARIES object_reg
   CATKIN_DEPENDS cv_bridge image_transport message_runtime sensor_msgs
   DEPENDS system_lib 
)

include_directories(${catkin_INCLUDE_DIRS})

# apriltags
set(apriltags_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../apriltags_ros/apriltags)
include_directories( ${apriltags_SOURCE_DIR}/include )
#link_directories( ${apriltags_SOURCE_DIR}/build/lib )

# object_reg
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
include_directories( ${PROJECT_SOURCE_DIR}/include 
   	   	     "/usr/include/eigen3")
add_library(${PROJECT_NAME} SHARED 
    src/DetectBlueCircle.cpp
    src/DetectMarker.cpp
    src/setting.cpp
)
target_link_libraries(${PROJECT_NAME}
    apriltags
    ${OpenCV_LIBS}
)

#mono
# add_executable(mono src/mono.cpp)
# target_link_libraries(mono
#     ${PROJECT_NAME} 
#     apriltags 
#     ${OpenCV_LIBS}
# )


# node
add_executable(${PROJECT_NAME}_node nodes/object_reg_node.cpp)
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_node 
    ${PROJECT_NAME} 
    apriltags 
    ${OpenCV_LIBS}
)












