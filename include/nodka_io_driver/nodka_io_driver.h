#ifndef NODKA_IO_DRIVER_H
#define NODKA_IO_DRIVER_H

#include <string>
#include <sstream>
#include <iostream>
#include <ros/ros.h>
#include <ros/time.h>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "nodka_io_driver/getInputPort.h"
#include "nodka_io_driver/setOutputPort.h"

#include "NK_IO_LC_LIB.h"

class NodkaIODriver
{
public:
  NodkaIODriver(ros::NodeHandle nh, ros::NodeHandle private_nh);
  ~NodkaIODriver();

private:
  bool get_input_port(nodka_io_driver::getInputPort::Request &req,
                        nodka_io_driver::getInputPort::Response &res);
  bool set_output_port(nodka_io_driver::setOutputPort::Request &req,
                        nodka_io_driver::setOutputPort::Response &res);
  static void OpenDeviceCallBack(LC_CALLBACK_ARG_T arg);
  static void CloseDeviceCallBack(LC_CALLBACK_ARG_T arg);
  static void ServerThread();
private:
  ros::NodeHandle nh_;
  ros::NodeHandle private_nh_;
  ros::ServiceServer input_service_;
  ros::ServiceServer output_service_;

  std::string config_file_;
};

#endif // NODKA_IO_DRIVER_H