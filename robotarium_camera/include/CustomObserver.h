#ifndef AVT_VMBAPI_MYFRAMEOBSERVER
#define AVT_VMBAPI_MYFRAMEOBSERVER

#include "VimbaCPP/Include/VimbaCPP.h"
#include "ProgramConfig.h"
#include "TransformImage.h"
#include <cv.hpp>
#include <opencv2/opencv.hpp>

class CustomObserver : virtual public AVT::VmbAPI::IFrameObserver
{
public:
    CustomObserver( AVT::VmbAPI::CameraPtr camera, AVT::VmbAPI::Examples::FrameInfos eFrameInfos, AVT::VmbAPI::Examples::ColorProcessing eColorProcessing );
    virtual void FrameReceived(const AVT::VmbAPI::FramePtr frames );
    void getFrame(cv::Mat & grabbedFrame);
private:
    const AVT::VmbAPI::Examples::FrameInfos m_eFrameInfos;
    const AVT::VmbAPI::Examples::ColorProcessing m_eColorProcessing;
    VmbUchar_t * imgBuffer;
    VmbUint32_t height, width;
    cv::Mat frame;
};

#endif // AVT_VMBAPI_MYFRAMEOBSERVER
