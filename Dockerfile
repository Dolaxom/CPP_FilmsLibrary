FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libpq-dev \
    libasio-dev \
    libhiredis-dev

COPY CMakeLists.txt /home/
ADD src /home/src
ADD third-party /home/third-party

WORKDIR /home/

RUN cmake -B build && cmake --build build && ./filmslibrary 
