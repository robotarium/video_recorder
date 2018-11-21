#include <stdio.h>
#include <stdlib.h>
#include <csignal>
#include <memory>
#include "RobotariumCamera.h"

unique_ptr<RobotariumCamera> robotariumCamera;
VideoWriter videoWriter;
volatile sig_atomic_t stop_recording = 0;

void signalHandler(int signum) {
    cout << "Termination signal (" << signum << ") received.\n";
    stop_recording = 1;
}

int main(int argc, char** argv)
{
    signal(SIGTERM, signalHandler);
    
    string config_file_name;

    if (argc < 2){
        cout << "Not enough input arguments." << endl;
        cout << "\trecordVideo config_file\n\t(e.g. recordVideo config/camera_parameters.yml)" << endl;
        return -1;
    } else {
        config_file_name = argv[1];
    }

    FileStorage fs(config_file_name, FileStorage::READ);

    cout << "\n\n---\n\n";
    cout << "Trying to connect to camera at " << (string)fs["CameraIp"] << " and record video '" << (string)fs["VideoFileName"] << "'..." << endl;
    
    robotariumCamera = unique_ptr<RobotariumCamera>(new RobotariumCamera(fs));

    double fps = 30.0;
    int width = 1600, height = 1200;
    robotariumCamera->getVideoParameters(fps, width, height);

    videoWriter.open((string)fs["VideoFileName"], CV_FOURCC('H','2','6','4'), fps, Size(width,height));
    // namedWindow("image");

    fs.release();

    cout << "\n\n---\n\n";

    Mat img;
    unsigned long int counter = 0;
    // while (char(waitKey(1000.0/fps)) != 'x')
    while (true)
    {
        robotariumCamera->getFrame(img);
        if (!img.empty())
        {
            videoWriter << img;
            // imshow("image", img);
            counter++;
        }
        if (counter%100==0){
            cout << counter << " frames recorded" << endl;
        }
        if (stop_recording){
            break;
        }

    }

    cout << "Releasing camera ...\n";
    robotariumCamera->~RobotariumCamera();
    cout << "Releasing video ...\n";
    videoWriter.release();
    cout << "... done.\n";

    return 0;
}
