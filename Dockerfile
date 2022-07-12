FROM ubuntu:20.04
ENV PWD=/2022-FunPalettes
WORKDIR ${PWD}
ENV DEBIAN_FRONTEND noninteractive

ADD . / ./
RUN apt-get update -y \
    && apt-get install -y wget tar libx11-dev libeigen3-dev libsdl2-dev mesa-utils build-essential libgl-dev
RUN wget https://cmake.org/files/v3.20/cmake-3.20.6-linux-x86_64.tar.gz \
    && tar -zxvf cmake-3.20.6-linux-x86_64.tar.gz \
    && mv cmake-3.20.6-linux-x86_64 cmake-3.20.6 \
    && ln -sf $PWD/cmake-3.20.6/bin/* /usr/bin
RUN mkdir build && cd ./build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && cmake --build . --config Release
CMD $PWD/build/FunPalttes
