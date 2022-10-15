## 各目录说明

- common 目录，其中 51config.h 为配置文件，为各外设统一分配端口；main.h 设置工程需要的所有头文件以及进行全局类型定义
- source 目录下是所有源文件
- include 目录是 source 下相应源文件的头文件，向外提供各外设 API
- project 是工程目录，建议把工程建到这个文件夹下
- output 是输出目录，在 keil 中可以设置 hex 输出到此

## 使用说明  
  **各模块 API 请参考对应的 include 目录下的头文件**

## 注意

- at24c02(EEPROM) 还未测试，欢迎测试，并提交结果。其他均由本人进行过测试，并在实际中使用过，当然也欢迎提交 bug
- 欢迎以同样思路**封装模块代码**，并提交上来，请尽量尊重本工程代码的风格

## 其他

### 去除函数未被调用的警告

1. Options -> Device -> √Use Extended Linker(LX51)instead of BL51
2. Options -> LX51 Misc -> Misc controls -> 填入REMOVEUNUSED 
