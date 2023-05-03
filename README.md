# OCRimg

## 

## 安装`tencentcloud-sdk-cpp`

```shell
sudo apt-get install libcurl4-openssl-dev -y
sudo apt-get install libssl-dev -y
sudo apt-get install uuid-dev -y
git clone https://github.com/TencentCloud/tencentcloud-sdk-cpp.git
cd tencentcloud-sdk-cpp
mkdir sdk_build
cmake -DBUILD_MODULES="core;ocr" ..
make -j 6
sudo make install
```

## 配置安装`conan`

```shell
pip install conan==1.59.0
conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI
```

## 配置tencent keys

`conf/tencent_key.json.exp`

```
{
  "secretId": "填写你的secretId",
  "secretKey": "填写你的secretKey"
}
```

```shell
sudo mkdir /etc/keys
cp conf/tencent_key.json.exp /etc/keys/tencent_key.json
```



## 编译安装

```shell
mkdir build
cd build
conan install ..
cmake ..
make -j 6
make install
cd bin
sudo cp ocrimg /usr/bin
sudo chmod 777 /usr/bin/ocrimg
```

### 测试

```bash
./ocrimg -h
```

```shell
ocrimg -h 查看帮助
ocrimg -i=input.png 输入图片名字
ocrimg -i=input.png  -o=aa.txt 输入图片名字 输出文件识别内容到文件
```



```
./ocrimg 
```

```shell
[2023-03-12 11:33:40.104] [error] mast input image name
```



```
./ocrimg -i=aaa
```

```
[2023-03-12 11:34:27.608] [error]  image name not exists
```



```
 ./ocrimg -i=test.png
```

```
[2023-03-12 11:36:17.119] [info] load secretId and secretKey success
[2023-03-12 11:36:17.120] [info] this->fileName is test.png
[2023-03-12 11:36:18.384] [info] get info success


目前，数字通信由于它相对模拟通信有抗干扰能力强、 
差错可控、易于集成化、 易于加密等优点已成为现代通信的 
主流。因此,系统中常要用到数字调制。
数字信号的传输方式分为基带传输和带通传输。实际信 
道中,大多数信道 (如无线信道 因具有带通特性而不能直接 
传输基带信号， 这是因为数字基带信号往往具有 丰富的低频
分量。为了使数字信号在带通信道中传输， 必须用载波对数 
字基带信号进行调制， 以使信号可以在带通信道中传输。这 
种用数字基带信号控制载波， 把数字基带信号变换为数字带 
通信号的过程称为数字调制。 在接收端通过解调器把带通信 
号还原成为数字基带信号的过程称为数字解调。 
一般来说， 数字调制与模拟调制的基本原理相同， 但是
数字信号有离散取值的特点。 因此数字调制技术有两种方 
法 :①利用模拟调制的方法去实现数字式调制,即把数字调 
制看成是模拟调制的一个特例， 把数字基带信号当作模拟信 
号的特殊情况处理 ②利用数字信号的离散取值特点通过开 
关键控载波，从而实现数字调制。 这种方法通常称为键控法， 
比如对载波的振幅、频率和相位进行键控， 便可获得振幅键 
控 (ASK)、频率键控 FSK) 和相位键控 (PSK 或 DPSK)。带通 
```

```shell
./ocrimg -i=test_txt.png -f
ls
```

```
ocrimg  test.png  test.txt
```

