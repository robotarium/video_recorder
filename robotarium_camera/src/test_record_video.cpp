#include <stdio.h>
#include <stdlib.h>
#include <csignal>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

VideoCapture camera;
VideoWriter videoWriter;

void signalHandler(int signum) {
	cout << "Termination signal (" << signum << ") received.\n";
	cout << "Releasing camera ...\n";
	camera.release();
	cout << "Releasing video ...\n";
    videoWriter.release();
    cout << "... done.\n";
	exit(signum);
}

int main(int argc, char** argv)
{
	signal(SIGTERM, signalHandler);

    camera.open(0);
    videoWriter.open(argv[1], CV_FOURCC('H','2','6','4'), 30, Size(640,480));

    namedWindow("image");

    Mat img;
    while (char(waitKey(1)) != 'x')
    {
	    camera >> img;
        if (!img.empty())
        {
            videoWriter << img;
            imshow("image", img);
        }
    }

    cout << "Releasing camera ...\n";
    camera.release();
    cout << "Releasing video ...\n";
    videoWriter.release();
    cout << "... done.\n";

    return 0;
}
