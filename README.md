# nodka_io_driver

**注意：必须在root下启动launch或node**



## 读取输入端口状态
Service name: /get_input_port
```
uint8 channel     # 通道，该型号只有0通道
uint8 port        # 端口，DO0——DO7(0-7)
--- 
bool status       # 高电平为true
```
 
## 设置输出端口状态
Service name: /set_output_port
```
uint8 channel     # 通道，该型号只有0通道
uint8 port        # 端口，DI0——DI7(0-7)
bool status       # 接通为true，断开为false
--- 
bool re           # 无意义，复制的status
```
