// Goal here: Work on Detection.
// Detect from camera that there is a drone. 

// I will for now use connected webcam to my computer. 

// 1. Connect to camera (External or not).
// 2. Take in stream. 
// 3. View footage on a page through software.
// 4. Detect if there's a drone on camera. 

// Choices:
// Going to use Libcamera so it works with Raspberry Pis. 

#include <iostream>
#include <memory>
#include <thread>
#include <iomanip>
#include <libcamera/libcamera.h>

using namespace libcamera;
using namespace std::chrono_literals;

// Camera manager runs all of the cameras. Enumerates them into one, handles all. 
// Making an instance of a static camera before main. 
// CameraManager is a unique pointer so that once its no longer used (falls out of scope), memory is gone. 
static std::shared_ptr<Camera> camera;

int main() {
	std::unique_ptr<CameraManager> cm = std::make_unique<CameraManager>();
	cm->start();

	for (auto const &camera : cm->cameras()) {
		std::cout << camera->id() << std::endl;
	}

	auto cameras = cm->cameras();

	if (cameras.empty()) {
		std::cout << "No cameras were found" << std::endl;
	cm->stop();
	return EXIT_FAILURE;
	}

	std::string cameraId = cameras[0]->id();

	camera = cm->get(cameraId);
	std::cout << "Acquired camera: " << cameraId << std::endl;
	camera->acquire();

	while(true){

	}

	return 0;
}
