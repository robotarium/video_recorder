#include "CustomObserver.h"

CustomObserver::CustomObserver( AVT::VmbAPI::CameraPtr camera, AVT::VmbAPI::Examples::FrameInfos eFrameInfos, AVT::VmbAPI::Examples::ColorProcessing eColorProcessing )
	: AVT::VmbAPI::IFrameObserver(camera), m_eFrameInfos(eFrameInfos), m_eColorProcessing(eColorProcessing)
{
    frameReceived = false;
}

void CustomObserver::FrameReceived(const AVT::VmbAPI::FramePtr frames)
{
    frames->GetBuffer(imgBuffer);
    
    frames->GetHeight(height);
    frames->GetWidth(width);

    cv::Mat frameBG = cv::Mat(height, width, CV_8UC1, imgBuffer, cv::Mat::AUTO_STEP);
    cv::cvtColor(frameBG, this->frame, CV_BayerBG2RGB);

    m_pCamera->QueueFrame(frames);

    this->frameReceived = true;
}

void CustomObserver::getFrame(cv::Mat & grabbedFrame)
{
    grabbedFrame = this->frame;
    this->frameReceived = false;
}

bool CustomObserver::isFrameReceived()
{
    return this->frameReceived;
}
