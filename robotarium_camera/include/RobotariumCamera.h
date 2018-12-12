#ifndef ROBOTARIUMCAMERA
#define ROBOTARIUMCAMERA

#include <opencv/cv.hpp>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "CustomObserver.h"

using namespace std;
using namespace cv;
using namespace AVT::VmbAPI;

class RobotariumCamera
{
public:
    RobotariumCamera(FileStorage fs);
    ~RobotariumCamera();
    void getVideoParameters(double& fps, int& width, int& height);
    void getFrame(Mat& image);
    void SetIntFeatureValueModulo2(const CameraPtr & Camera, const char* const& Name);
    void PrepareCamera();
    bool isFrameReady();
private:
    CameraPtrVector camera;
    FramePtrVector frames;
    CustomObserver *obs;
    FileStorage file_storage;
};

#endif // ROBOTARIUMCAMERA
