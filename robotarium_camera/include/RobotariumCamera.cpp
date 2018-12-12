#include "RobotariumCamera.h"

RobotariumCamera::RobotariumCamera(FileStorage fs)
{
    VimbaSystem & system = VimbaSystem::GetInstance();
    camera.reserve(1);
    camera.push_back(CameraPtr());

    FeaturePtr pFeature;
    VmbInt64_t nPLS;
    const int NUM_FRAMES = 3;

    if (system.Startup() == VmbErrorSuccess){
        if (system.OpenCameraByID(((string)fs["CameraIp"]).c_str(), VmbAccessModeFull, camera[0]) == VmbErrorSuccess){
            FeaturePtr pCommandFeature;
            if (camera[0]->GetFeatureByName("GVSPAdjustPacketSize", pCommandFeature) == VmbErrorSuccess){
                if (pCommandFeature->RunCommand() == VmbErrorSuccess){
                    bool bIsCommandDone = false;
                    do {
                        if (pCommandFeature->IsCommandDone( bIsCommandDone ) != VmbErrorSuccess) {
                            break;
                        }
                    } while (bIsCommandDone == false);
                }
            }
            cout << "Camera opened" << endl;
        }
        else{
            cout << "Failed to open camera" << endl;
        }
    }

    // set camera parameters
    FileNode camera_parameters_node = fs["CameraParameters"];
    for(FileNodeIterator it = camera_parameters_node.begin(); it != camera_parameters_node.end(); it++)
    {
        if (camera[0]->GetFeatureByName(((string)(*it)["name"]).c_str(),pFeature) == VmbErrorSuccess){
            if (strcmp(((string)(*it)["type"]).c_str(), "string") == 0) {
                pFeature->SetValue(((string)(*it)["value"]).c_str());
                std::cout << (string)(*it)["name"] << " set to " << (string)(*it)["value"] << std::endl;
            }
            else if (strcmp(((string)(*it)["type"]).c_str(), "int") == 0) {
                pFeature->SetValue((int)(*it)["value"]);
                std::cout << (string)(*it)["name"] << " set to " << (int)(*it)["value"] << std::endl;
            }
            else if (strcmp(((string)(*it)["type"]).c_str(), "float") == 0) {
                pFeature->SetValue((float)(*it)["value"]);
                std::cout << (string)(*it)["name"] << " set to " << (float)(*it)["value"] << std::endl;
            }
        }
        else {
          std::cout << "!!! Error while setting " << (string)(*it)["name"] << "." << std::endl;
        }
    }
    
    // get camera parameters
    if (camera[0]->GetFeatureByName("PayloadSize",pFeature) == VmbErrorSuccess){
        pFeature->GetValue(nPLS);
        std::cout << "Got PayloadSize: " << nPLS << std::endl;
    }
    else {
      std::cout << "Cannot get PayloadSize value" << std::endl;
    }

    frames.reserve(NUM_FRAMES);
    for (int i = 0; i < NUM_FRAMES; i++)
        frames.push_back(FramePtr());

    PrepareCamera();

    obs = new CustomObserver(camera[0], AVT::VmbAPI::Examples::FrameInfos(AVT::VmbAPI::Examples::FrameInfos_Off), AVT::VmbAPI::Examples::ColorProcessing(AVT::VmbAPI::Examples::ColorProcessing_Off));

    IFrameObserverPtr pObserver0(obs);

    // camera[0]->StartContinuousImageAcquisition(NUM_FRAMES, IFrameObserverPtr(obs));
    camera[0]->StartContinuousImageAcquisition(NUM_FRAMES, pObserver0);

    for (FramePtrVector::iterator iter = frames.begin(); iter != frames.end(); iter++){
        (*iter).reset(new Frame(nPLS));
        (*iter)->RegisterObserver(pObserver0);
        camera[0]->AnnounceFrame(*iter);
    }

    this->file_storage = fs;
}

RobotariumCamera::~RobotariumCamera()
{
    camera[0]->StopContinuousImageAcquisition();
    if (camera[0]->Close() == VmbErrorSuccess){
        cout << "Camera closed" << endl;
    }
}

void RobotariumCamera::getVideoParameters(double& fps, int& width, int& height)
{
    FileNode camera_parameters_node = this->file_storage["CameraParameters"];
    for(FileNodeIterator it = camera_parameters_node.begin(); it != camera_parameters_node.end(); it++)
    {
        if (strcmp(((string)(*it)["name"]).c_str(), "ExposureTimeAbs") == 0) {
            fps = 1000000.0 / (float)(*it)["value"];
        }
        else if (strcmp(((string)(*it)["name"]).c_str(), "Width") == 0) {
            width = (int)(*it)["value"];
        }
        else if (strcmp(((string)(*it)["name"]).c_str(), "Height") == 0) {
            height = (int)(*it)["value"];
        }
    }
}

void RobotariumCamera::getFrame(Mat& image)
{
    obs->getFrame(image);
    for (FramePtrVector::iterator iter = frames.begin(); iter != frames.end(); iter++){
        camera[0]->QueueFrame(*iter);
    }
}

bool RobotariumCamera::isFrameReady()
{
    return obs->isFrameReceived();
}

void RobotariumCamera::SetIntFeatureValueModulo2(const CameraPtr &pCamera, const char* const& Name)
{
    FeaturePtr feature;
    VmbInt64_t value_min,value_max;
    SP_ACCESS(pCamera)->GetFeatureByName(Name, feature);
    SP_ACCESS(feature)->GetRange(value_min, value_max);
    value_max = (value_max>>1)<<1;
    SP_ACCESS(feature)->SetValue(value_max);
}

void RobotariumCamera::PrepareCamera()
{
    SetIntFeatureValueModulo2(camera[0],"Width");
    SetIntFeatureValueModulo2(camera[0],"Height");
}
