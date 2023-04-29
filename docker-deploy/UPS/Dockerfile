FROM ubuntu:20.04
RUN mkdir /code
ADD . /code/
WORKDIR /code

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC
RUN ls 
RUN apt-get update 
RUN apt-get -y install build-essential libpqxx-dev postgresql 
RUN apt-get install -y autoconf automake libtool curl make g++
RUN apt-get install -y build-essential zlib1g-dev
RUN apt-get install -y  pkg-config
# install protobuf
# install protobuf
#RUN cd protobuf-3.20.0
RUN cd protobuf-3.20.0 && chmod +x configure && ./configure && make && make install && ldconfig


WORKDIR /code
RUN chmod u+r+x run.sh

