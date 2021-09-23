
#include <string>
#include <sstream>
#include <iostream>
#include <ros/ros.h>

#include "nodka_io_driver/nodka_io_driver.h"

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "nodka_io_driver");
  ros::NodeHandle nh, private_nh("~");

  NodkaIODriver nd(nh, private_nh);

  ros::spin();

  ROS_INFO("All finish");

  return 0;
}