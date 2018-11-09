FROM ubuntu:16.04

RUN apt-get update && apt-get -y upgrade

COPY ./Vimba_2_1 /Vimba_2_1

RUN apt-get install -y git python2.7 python-pip libglib2.0-0 libsm-dev libxrender-dev libgtk-3-dev libxt6
 
RUN python2 -m pip install opencv-python 

ADD https://api.github.com/repos/robotarium/video_recorder/git/refs/heads/master version.json
RUN git clone https://github.com/robotarium/video_recorder

RUN cd /Vimba_2_1/VimbaGigETL && ./Install.sh
ENV GENICAM_GENTL64_PATH $GENICAM_GENTL64_PATH:"/Vimba_2_1/VimbaGigETL/CTI/x86_64bit"
ENV QT_X11_NO_MITSHM=1

ADD https://api.github.com/repos/morefigs/pymba/git/refs/heads/master version.json
RUN git clone https://github.com/morefigs/pymba && cd pymba && python -m pip install .


#ENTRYPOINT ["/bin/sh", "-c", "\". /etc/profile.d/VimbaGigETL_64bit.sh\"", "&&", "/bin/sh"]
