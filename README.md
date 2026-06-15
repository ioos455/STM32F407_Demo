# STM32F407_Demo
基于STM32F407的FreeRTOS+LVGL多传感器采集、屏幕显示与串口日志综合工程

## 项目简介
本工程以STM32F407单片机为硬件核心，搭载FreeRTOS实时操作系统，搭配LVGL嵌入式图形库实现触摸屏可视化；集成多传感器数据采集功能，同时支持串口实时日志打印调试，适用于嵌入式工控、便携式数据采集终端类开发学习。
- RTOS：FreeRTOS 多任务调度（传感器采集、GUI刷新、串口打印任务分离）
- GUI：LVGL图形界面，实时渲染传感器数值
- 外设：多传感器I2C采集、UART日志输出
- 开发环境：STM32CubeMX + MDK-ARM / STM32CubeIDE
- 协议：I2C传感器通信、串口异步打印

## 仓库地址
https://github.com/ioos455/STM32F407_Demo.git

## 功能特性
1. **FreeRTOS多任务管理**
   - 独立传感器采集任务、LVGL界面刷新任务、串口日志输出任务
   - 使用信号量/互斥锁实现多任务数据同步，避免I2C总线死锁、界面刷新冲突
2. **多传感器数据采集**
   - 支持光照、加速度等传感器数据周期性读取
   - 数据统一缓存，供GUI与串口同步读取
3. **LVGL嵌入式图形显示**
   - 屏幕实时展示传感器原始数据与换算数值
   - 轻量化界面，适配STM32F407片内RAM资源
4. **串口调试日志**
   - 实时打印传感器数值、任务运行状态、异常报错信息
   - 支持上位机串口工具直接观测运行数据
5. 工程标准化
   - CubeMX自动生成底层驱动，分层代码结构，便于二次开发
   - 规范.gitignore过滤编译产物、工程临时文件

## 环境与硬件
### 硬件平台
- 主控：STM32F407系列单片机
- 外设：I2C接口传感器模块、TFTLCD触摸屏、串口调试接口
### 软件开发环境
1. STM32CubeMX：芯片外设初始化配置
2. 编译工具：Keil MDK-ARM / STM32CubeIDE
3. 版本控制：Git + GitHub
4. 上位工具：串口助手（SSCOM/串口调试助手）

常见问题
LVGL 界面卡顿 / 花屏
调整 GUI 任务优先级，使用互斥锁保护显示缓存
降低屏幕刷新帧率，减少 CPU 占用
I2C 读取传感器卡死
检查互斥锁是否完整释放，避免多任务抢占 I2C 总线
Git 推送报错 protocol 不支持
删除损坏远程地址，重新添加纯净 HTTPS 仓库地址
串口无日志输出
核对 CubeMX 串口引脚、波特率配置，检查硬件接线

## 快速上手
### 1. 克隆项目到本地
```bash
git clone https://github.com/ioos455/STM32F407_Demo.git
cd STM32F407_Demo
STM32F407_Demo/
├── UART1/                      # CubeMX生成底层驱动
│   ├── Middlewares/
│   │   └── Third_Party/FreeRTOS/  # FreeRTOS内核源码
│   ├── Core/                   # 应用主逻辑、任务创建
│   ├── Drivers/                # STM32标准外设库
│   └── UART1.ioc               # CubeMX配置文件
├── README.md                   # 项目说明文档
├── LICENSE                     # MIT开源协议
└── .gitignore                  # Git忽略文件配置
