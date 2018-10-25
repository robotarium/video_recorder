FROM ubuntu:16.04

RUN apt-get update && apt-get -y upgrade

COPY ./Vimba_2_1 /Vimba_2_1

RUN apt-get install -y git python2.7 python-pip

RUN git clone https://github.com/robotarium/video_recorder

RUN ./Vimba_2_1/VimbaGigETL/Install.sh

RUN python2 -m pip install opencv-python
