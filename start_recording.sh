xhost +

docker run \
	--rm -it \
	--volume $(pwd)/video_file/:/home/video_file \
	--volume $(pwd)/config/:/home/config \
	--volume /tmp/.X11-unix/:/tmp/.X11-unix/:rw \
	--env="DISPLAY" \
	--net=host \
robotarium:robotarium_camera
