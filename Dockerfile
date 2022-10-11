FROM ubuntu:18.04

RUN apt-get update -qq
RUN apt-get install -qq software-properties-common
RUN add-apt-repository ppa:team-gcc-arm-embedded/ppa -y
RUN apt-get update -qq
RUN apt-get install -qq gcc-arm-embedded build-essential

