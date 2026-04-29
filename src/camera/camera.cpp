#include "camera.h"
#include <iostream>

using namespace libcamera;

int initializeManager(CameraManager &Manager) {
    Manager.start();
    return 1;
} 

int main() {
    std::unique_ptr<CameraManager> cm = std::make_unique<CameraManager>();
    initializeManager(*cm);
}