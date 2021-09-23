#include "nodka_io_driver/nodka_io_driver.h"

NodkaIODriver::NodkaIODriver(ros::NodeHandle nh, ros::NodeHandle private_nh)
    : nh_(nh), private_nh_(private_nh)
{
  if(private_nh_.hasParam("config_file"))
  {
    private_nh_.getParam("config_file", config_file_);  
    ROS_INFO_STREAM("get config file: " << config_file_);
  }
  else
  {
    private_nh_.param<std::string>("config_file", config_file_, "./src/nodka_io_driver/config/NP-6122-H1/nkio_config.ini");
    ROS_WARN_STREAM("no config file!!!, use default: " << config_file_);
  }

  //Return 1 if success; return -1 when failed.
  if(DIOLC_LibraryBaseInit(config_file_.c_str()))
  {
    ROS_INFO_STREAM("Init nk_io_lib successful!!!");

    //start Server thread
    boost::thread(boost::bind(NodkaIODriver::ServerThread));

    //return -1 when failed, return the other value if success.
    // if(DIOLC_OpenDevice(3, NodkaIODriver::OpenDeviceCallBack))
    // {
    //   ROS_INFO_STREAM("DIOLC Device is opening.....");
    // }
    // else
    // {
    //   ROS_ERROR_STREAM("DIOLC Open Device failed!!!");
    // }

    //init out port LOW
    DIO_PollingWriteDoByte(0x00, 0x00);
  }
  else
  {
    ROS_ERROR_STREAM("Init nk_io_ib failed!!!");
    exit(-1);
  }

  input_service_ = nh_.advertiseService("get_input_port", &NodkaIODriver::get_input_port, this);
  output_service_ = nh_.advertiseService("set_output_port", &NodkaIODriver::set_output_port, this);
}

NodkaIODriver::~NodkaIODriver()
{

}

bool NodkaIODriver::get_input_port(nodka_io_driver::getInputPort::Request &req,
                                   nodka_io_driver::getInputPort::Response &res)
{
  res.status = DIO_PollingReadDiBit(req.channel, req.port);
  return true;
}

bool NodkaIODriver::set_output_port(nodka_io_driver::setOutputPort::Request &req,
                                    nodka_io_driver::setOutputPort::Response &res)
{
  //res.re = req.port + req.status;
  uint8_t status = req.status? 1 : 0;
  DIO_PollingWriteDoBit(req.channel, req.port, status);
  res.re = req.status;
  return true;
}

void NodkaIODriver::OpenDeviceCallBack(LC_CALLBACK_ARG_T arg)
{
  if (arg.openComCallbackArg.error)
  {
    ROS_ERROR("Open port %d failed: %d",
              arg.openComCallbackArg.portNum,
              arg.openComCallbackArg.errorId);
  }
  else
  {
    ROS_INFO("Hardware version: %d.%d.%d\tFirmware version: %d.%d.%d",
             arg.openComCallbackArg.hardwareMajorVer,
             arg.openComCallbackArg.hardwareMinorVer,
             arg.openComCallbackArg.hardwareRevVer,
             arg.openComCallbackArg.firmwareMajorVer,
             arg.openComCallbackArg.firmwareMinorVer,
             arg.openComCallbackArg.firmwareRevVer);
  }
}

void NodkaIODriver::CloseDeviceCallBack(LC_CALLBACK_ARG_T arg)
{
  if (arg.openComCallbackArg.error)
  {
    ROS_ERROR("Close port %d failed: %d",
              arg.openComCallbackArg.portNum,
              arg.openComCallbackArg.errorId);
  }
  else
  {
    ROS_INFO("Close port %d successful!", arg.openComCallbackArg.portNum);
  }
}

void NodkaIODriver::ServerThread()
{
  while (ros::ok())
  {
    DIOLC_Process();
    boost::this_thread::sleep_for(boost::chrono::milliseconds{1});
  }
  
  DIOLC_CloseDevice(3, NodkaIODriver::CloseDeviceCallBack);
  DIOLC_LibraryBaseDeinit();
}