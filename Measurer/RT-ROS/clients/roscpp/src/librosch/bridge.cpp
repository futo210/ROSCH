#include "ros_rosch/bridge.hpp"
#include <iostream>
#include <string>
#include <ros/ros.h>

const char* rosch::get_node_name(){
    std::string nodename = ros::this_node::getName();
    return nodename.c_str();
}
