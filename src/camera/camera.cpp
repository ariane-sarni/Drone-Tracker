#include "camera.h"
#include <iostream>

using namespace libcamera;

void throw_error(int ret, const std::string &message) {
    std::cerr << "FATAL ERROR: " << message << " (Error code: " << ret << (")");
    std::exit(EXIT_FAILURE);
}


std::unique_ptr<CameraManager> createManager(){
    std::unique_ptr<CameraManager> cm = std::make_unique<CameraManager>();
    int ret = cm->start();
    if (ret < 0){
        throw_error(ret, "Failed to initialize camera manager.");
    }
    return cm;
}


std::vector<std::shared_ptr<Camera>> getCameraList(CameraManager &Manager){
    auto cameras = Manager.cameras();
    if (cameras.empty()) {
        throw_error(0, "Failed to find a camera. Did you initialize the camera manager properly?");
        Manager.stop();
    }
    return cameras;
}

std::string getCameraID(Camera &Cam) {
    return Cam.id();
}

std::shared_ptr<Camera> obtainCamera(CameraManager &Manager, std::string cameraID){
    auto camera = Manager.get(cameraID);
    if (!camera){
        throw_error(0, "Failed to get camera. Either Camera Manager wasn't initialized, or no cameras are plugged in.");
    }
    return camera;
}

void acquireCamera(Camera &Cam){
    int ret = Cam.acquire();
    if (ret < 0) {
        throw_error(0, "Failed to acquire camera. Was it unplugged?");
    }
} 



int main() {

    // 1. Create camera manager, intialize it.
    // 2. Get camera list. 
    // 3. Get Camera id, acquire camera. 
    // 4. Set up config
    auto cameraManager = createManager();
    auto cameraList = getCameraList(*cameraManager);
    std::string cameraID = getCameraID(*cameraList[0]);

    auto camera = obtainCamera(*cameraManager, cameraID);
    acquireCamera(*camera);

    // Now we can move onto configuration
    



}