FROM resin/rpi-raspbian:latest
ENTRYPOINT []

# Configure the main working directory. This is the base
# directory used in any further RUN, COPY, and ENTRYPOINT
# commands.
RUN mkdir -p /app
WORKDIR /app

RUN apt-get update && \
    apt-get -qy install git build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev \
       libcurl4-gnutls-dev dh-autoreconf

RUN cd /usr/local/src && \
    git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd paho.mqtt.c && \
    make && \
    make install

RUN cd /usr/local/src && \
    git clone https://github.com/eclipse/paho.mqtt.cpp.git && \
    cd paho.mqtt.cpp && make

RUN cd /usr/local/src/paho.mqtt.cpp && \
    install -m 644 lib/libpaho-mqttpp3.so.1.0.0 /usr/local/lib/ && \
    /sbin/ldconfig /usr/local/lib && \
    cd /usr/local/lib && \
    ln -s libpaho-mqttpp3.so.1 libpaho-mqttpp3.so && \
    cp -r /usr/local/src/paho.mqtt.cpp/src/mqtt /usr/local/include && \
    chmod 644 /usr/local/include/mqtt/* && \
    echo '/usr/local/lib' | tee /etc/ld.so.conf.d/mqttpp.conf && \
    /sbin/ldconfig

RUN cd /usr/local/src && \
    git clone git://git.drogon.net/wiringPi && \
    cd wiringPi && \
    ./build

RUN cd /usr/local/src && \
    git clone https://github.com/nlohmann/json.git && \
    cp json/src/json.hpp /usr/local/include/

RUN cd /usr/local/src && \
    git clone https://github.com/BioBoost/thread_safe.git && \
    cp -r thread_safe/include/thread_safe /usr/local/include

RUN cd /usr/local/src && \
    git clone https://github.com/mrtazz/restclient-cpp.git && \
    cd restclient-cpp && \
    ./autogen.sh && \
    ./configure && \
    make install

RUN cd /usr/local/src && \
    git clone https://github.com/BioBoost/bios_logger.git && \
    cd bios_logger && \
    make && \
    make install

# Copy the main application and compile it
COPY . ./
RUN make clean && make

CMD ./bin/home_automator
