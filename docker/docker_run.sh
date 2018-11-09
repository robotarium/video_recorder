docker run --rm -ti \
	--env="DISPLAY" \
	-v /home/robotarium-workstation/git/video_recorder/record_video.py:/record_video.py \
	-v /tmp/.X11-unix/:/tmp/.X11-unix/:rw \
	--net=host \
robotarium:video_recorder
