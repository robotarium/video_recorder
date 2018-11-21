FROM ubuntu:16.04

# general purpose utils
RUN apt-get -y update && apt-get -y install vim inetutils-ping net-tools arp-scan cmake libboost-all-dev

# opencv
RUN apt-get -y install python3.5-dev python3-pip wget unzip \
    build-essential cmake git pkg-config libatlas-base-dev gfortran \
    libjasper-dev libgtk2.0-dev libavcodec-dev libavformat-dev \
    libswscale-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libv4l-dev \
    && pip3 install numpy

RUN wget https://github.com/Itseez/opencv/archive/3.2.0.zip -O opencv3.zip \
    && unzip -q opencv3.zip && mv /opencv-3.2.0 /opencv && rm opencv3.zip \
    && wget https://github.com/Itseez/opencv_contrib/archive/3.2.0.zip -O opencv_contrib3.zip \
    && unzip -q opencv_contrib3.zip && mv /opencv_contrib-3.2.0 /opencv_contrib && rm opencv_contrib3.zip

RUN mkdir /opencv/build && cd /opencv/build \
    && cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D BUILD_PYTHON_SUPPORT=ON \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D OPENCV_EXTRA_MODULES_PATH=/opencv_contrib/modules \
      -D BUILD_EXAMPLES=OFF \
      -D PYTHON_DEFAULT_EXECUTABLE=/usr/bin/python3 \
      -D BUILD_opencv_python3=ON \
      -D BUILD_opencv_python2=OFF \
      -D WITH_IPP=OFF \
      -D WITH_FFMPEG=ON \
      -D WITH_V4L=ON ..

RUN cd /opencv/build && make -j$(nproc) && make install && ldconfig

# robotarium cameras code
COPY robotarium_camera /home/robotarium_camera

RUN cd /home/robotarium_camera && mkdir build && cd build && cmake .. && make

RUN cd /home/robotarium_camera/Vimba_2_1/VimbaGigETL/ && ./Install.sh
ENV GENICAM_GENTL64_PATH $GENICAM_GENTL64_PATH:"/home/robotarium_camera/Vimba_2_1/VimbaGigETL/CTI/x86_64bit"
ENV QT_X11_NO_MITSHM=1

#CMD ["/bin/bash"]
#CMD ["/home/robotarium_camera/build/test_record_video", "/home/video_file/test_video_webcam.mp4"]
CMD ["/home/robotarium_camera/build/record_video", "/home/config/camera_parameters.yml"]
