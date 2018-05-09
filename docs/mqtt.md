# Mqtt for mac

This currently works on the Mac X.
this code is under devolopment and may still cause some errors

## Installing Mqtt development libraries

Install dependencies to allow for compilation of mqtt for mac

```shell
brew install gcc make
```

Next clone, compile and install paho C client library of MQTT

```shell
cd
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo update_dyld_shared_cache
sudo make install
cmake .
make
sudo make install
cp /usr/local/lib/libpaho-mqtt3as.so.1.0 /usr/local/lib/libpaho-mqtt3as.so.1
```


or you could invoke the compiler. It should state undefined reference to main:
```shell
g++ -lpaho-mqtt3a
```
output code gives an error, but that's normal
```shell
Undefined symbols for architecture x86_64:
  "_main", referenced from:
     implicit entry/start for main executable
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

This project currently makes use of the classic synchronous client.

Next we need to compile and install the C++ wrapper library for the MQTT libraries

```shell
cd
git clone https://github.com/eclipse/paho.mqtt.cpp.git
cd paho.mqtt.cpp
cmake -G"Unix Makefiles" -DPAHO_WITH_SSL=FALSE -DPAHO_MQTT_C_PATH="..\paho.mqtt.c" .
make
sudo make install
```


Should show:
```shell
libpaho-mqttpp3.so.1 (libc6,hard-float) => /usr/local/lib/libpaho-mqttpp3.so.1
libpaho-mqttpp3.so (libc6,hard-float) => /usr/local/lib/libpaho-mqttpp3.so
```

or you could invoke the compiler. It should state undefined reference to main:
```shell
g++ -lpaho-mqttpp3
/usr/lib/gcc/arm-linux-gnueabihf/6/../../../arm-linux-gnueabihf/crt1.o: In function `_start':
(.text+0x34): undefined reference to `main'
collect2: error: ld returned 1 exit status
```
