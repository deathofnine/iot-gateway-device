# 物联网网关设备端

![Platform](https://img.shields.io/badge/platform-i.MX6ULL-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B%20%7C%20C-green)

基于 **韦东山 i.MX6ULL Pro** 开发板的物联网网关设备端代码，实现传感器采集、远程控制、云平台对接、本地 GUI。

---

## ✨ 功能特性

- **RPC 服务端**：LED 开关、DHT11 温湿度读取
- **MQTT 设备端**：对接华为云 IoT 平台（数据上报 + 云端指令）
- **Qt GUI 界面**：本地触摸屏控制面板，实时显示温湿度

---

## 🧱 支持的平台

| 项目 | 说明 |
|------|------|
| **硬件** | 韦东山 i.MX6ULL Pro（100ask） |
| **系统** | Ubuntu / Buildroot 出厂镜像 |
| **驱动** | 板级 DHT11 驱动、LED GPIO 驱动 |

> ⚠️ DHT11 和 LED 部分依赖板级驱动，其他 ARM 板需修改 GPIO 或设备树。

---

## 项目结构

- **`LED_Humi/`** - Qt GUI 程序
- **`mqtt_project/`**
  - `mqtt_device/` - MQTT 设备端代码
  - `paho.mqtt.c/` - 第三方 MQTT 库
- **`rpc_server_project/`**
  - `rpc_server/` - RPC 服务端代码
  - `libev/` - 第三方 libev 库
  - `jsonrpc-c/` - 第三方 jsonrpc 库

text

---

## 🔧 编译与部署

### 1. 克隆仓库

```bash
git clone https://github.com/deathofnine/iot-gateway-device.git
cd iot-gateway-device
```
### 2. 编译第三方库
2.1 MQTT 库（paho.mqtt.c）
bash
```bash
cd mqtt_project
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
git checkout v1.3.12
```
修改 CMakeLists.txt，在文件最前面添加：
```bash
cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_C_COMPILER /home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/arm-buildroot-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/arm-buildroot-linux-gnueabihf-g++)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```
然后编译：
```bash
bash
mkdir build && cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=output
make
make install
```
编译成功后，build/output/ 目录下会生成 lib/、include/、bin/、share/。

将库文件推送到开发板：
```bash
bash
cd output/lib
adb push libpaho-mqtt3* /lib
```
2.2 编译 libev 库
```bash
cd ../../../../rpc_server_project/libev
./configure --host=arm-buildroot-linux-gnueabihf --prefix=$PWD/tmp
make -j 16
make install
```
编译后 tmp/ 目录下生成 include/、lib/、share/。

2.3 编译 jsonrpc-c 库
```bash
cd ../jsonrpc-c
autoreconf -i
./configure --host=arm-buildroot-linux-gnueabihf --prefix=$PWD/tmp \
    CFLAGS="-I$PWD/../libev/tmp/include" \
    LDFLAGS="-L$PWD/../libev/tmp/lib"
make -j 16
make install
```
编译后 tmp/ 目录下生成 include/、lib/。

### 3. 编译项目模块
```bash
# RPC 服务端
cd rpc_server_project/rpc_server
make

# MQTT 设备端
cd ../../mqtt_project/mqtt_device
make

# Qt GUI（需要 qmake）
cd ../../LED_Humi
qmake && make
```
### 4. 部署到开发板
将三个可执行文件推送到开发板 /root 目录：

```bash
adb push rpc_server /root/
adb push mqtt_device /root/
adb push Led_Humi /root/
```
🚀 运行
登录开发板：

```bash
adb shell
加载驱动：

bash
cd /root
insmod dht11_drv.ko
insmod led_drv.ko
```
按顺序启动程序：

```bash
./rpc_server &
./Led_Humi &
./mqtt_device &
```
建议在 separate 终端中分别运行，或使用 & 放入后台。

📌 注意事项
请确保交叉编译器路径与你的环境一致

MQTT 库版本建议锁定 v1.3.12，最新版本可能编译失败

运行前确认开发板已连接网络（用于 MQTT 上报）


