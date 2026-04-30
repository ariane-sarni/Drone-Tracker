#include "camera.h"
#include <iostream>

using namespace libcamera;

void throw_error(int ret, const std::string &message) {
    std::cerr << "FATAL ERROR: " << message << " (Error code: " << ret << (")");
    std::exit(EXIT_FAILURE);
}


void initializeManager(CameraManager &Manager) {
    int ret = Manager.start();
    if (ret < 0) {
        throw_error(ret, "Failed to initialize camera manager.");
    }
} 

std::vector<std::shared_ptr<Camera>> getCameraList(const libcamera::CameraManager &Manager){
    auto cameras = Manager.cameras();
    if (cameras.empty()) {
        throw_error(0, "Failed to find a camera.");
    }
    return cameras;
}

int main() {
    // Making the actual camera manager. Could make this a function too maybe? 
    std::unique_ptr<CameraManager> cm = std::make_unique<CameraManager>();
    
    // Passing cm as a pointer dereferences it. 
    // Dereferencing an object returns a reference to it (lvalue)
    initializeManager(*cm);

    auto cameras = getCameraList(*cm);


}