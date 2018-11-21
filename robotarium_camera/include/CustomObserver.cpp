#include "CustomObserver.h"

CustomObserver::CustomObserver( AVT::VmbAPI::CameraPtr camera, AVT::VmbAPI::Examples::FrameInfos eFrameInfos, AVT::VmbAPI::Examples::ColorProcessing eColorProcessing )
	: AVT::VmbAPI::IFrameObserver(camera), m_eFrameInfos(eFrameInfos), m_eColorProcessing(eColorProcessing)
{
}

void CustomObserver::FrameReceived(const AVT::VmbAPI::FramePtr frames)
{
    frames->GetBuffer(imgBuffer);
    
    frames->GetHeight(height);
    frames->GetWidth(width);

    this->frame = cv::Mat(height, width, CV_8UC3, imgBuffer, cv::Mat::AUTO_STEP);

    m_pCamera->QueueFrame(frames);
}

void CustomObserver::getFrame(cv::Mat & grabbedFrame)
{
    grabbedFrame = this->frame;
}
