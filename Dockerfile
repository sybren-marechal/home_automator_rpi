FROM resin/rpi-raspbian:latest
ENTRYPOINT []

RUN apt-get update && \
    apt-get -qy install git build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev \
       libcurl4-gnutls-dev dh-autoreconf

RUN cd && git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd paho.mqtt.c && make && make install

RUN cd && git clone https://github.com/eclipse/paho.mqtt.cpp.git && \
    cd paho.mqtt.cpp && make

RUN cd ~/paho.mqtt.cpp && install -m 644 lib/libpaho-mqttpp3.so.1.0.0 /usr/local/lib/ && \
    /sbin/ldconfig /usr/local/lib && cd /usr/local/lib && ln -s libpaho-mqttpp3.so.1 libpaho-mqttpp3.so && \
    cp -r ~/paho.mqtt.cpp/src/mqtt /usr/local/include && chmod 644 /usr/local/include/mqtt/* && \
    echo '/usr/local/lib' | tee /etc/ld.so.conf.d/mqttpp.conf && /sbin/ldconfig

RUN cd && git clone git://git.drogon.net/wiringPi && cd ~/wiringPi && ./build

RUN cd /usr/local/src && git clone https://github.com/nlohmann/json.git && cp json/src/json.hpp /usr/local/include/

RUN cd /usr/local/src && git clone https://github.com/BioBoost/thread_safe.git && cp -r thread_safe/include/thread_safe /usr/local/include

RUN cd /usr/local/src && git clone https://github.com/mrtazz/restclient-cpp.git && cd restclient-cpp && ./autogen.sh && \
  ./configure && make install

RUN cd /usr/local/src && git clone https://github.com/BioBoost/bios_logger.git && cd bios_logger && \
  make && sudo make install

RUN cd && git clone https://github.com/BioBoost/home_automator_rpi.git && \
    cd home_automator_rpi && make

CMD /root/home_automator_rpi/bin/home_automator
