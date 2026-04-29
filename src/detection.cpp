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
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace libcamera;
using namespace std::chrono_literals;

// Camera manager runs all of the cameras. Enumerates them into one, handles all. 
// Making an instance of a static camera before main. 
// CameraManager is a unique pointer so that once its no longer used (falls out of scope), memory is gone. 
static std::shared_ptr<Camera> camera;

int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;

class RenderWindow {
	public:
		RenderWindow(const char* title, int width, int height);
		SDL_Renderer* GetRenderer();
		void close();

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
};

RenderWindow::RenderWindow(const char* title, int width, int height) : window(NULL), renderer(NULL){
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "SDL could not be created." << std::endl;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		std::cout << "SDL could not create renderer!" << std::endl;
	}
}

SDL_Renderer* RenderWindow::GetRenderer() {
	return renderer;
}

void RenderWindow::close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	SDL_Quit();
}

// Re-read this later to understand. 
static void requestComplete(Request *request) {
	if (request->status() == Request::RequestCancelled) {
		return;
	}

	const std::map<const Stream *, FrameBuffer *> &buffers = request->buffers();

	for (auto bufferPair : buffers) {
		FrameBuffer *buffer = bufferPair.second;
		const FrameMetadata &metadata = buffer->metadata();
		std::cout << " seq: " << std::setw(6) << std::setfill('0') << metadata.sequence << " bytesused: ";
		unsigned int nplane = 0;
		for (const FrameMetadata::Plane &plane : metadata.planes()) {
			std::cout << plane.bytesused;
			if (nplane++ < metadata.planes().size()) std::cout << "/";
		}
		std::cout << std::endl;
	}

	request->reuse(Request::ReuseBuffers);
	camera->queueRequest(request);


}


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

	std::unique_ptr<CameraConfiguration> config = camera->generateConfiguration({StreamRole::Viewfinder});
	StreamConfiguration &streamConfig = config->at(0);
	std::cout << "Default viewfinder config is: " << streamConfig.toString() << std::endl;

	// Change settings, validate.
	streamConfig.size.width = 1920;
	streamConfig.size.height = 1080;

	config->validate();
	std::cout << "Post validated config = " << streamConfig.toString() << std::endl;

	camera->configure(config.get());

	FrameBufferAllocator *allocator = new FrameBufferAllocator(camera);

	for (StreamConfiguration &cfg : *config) {
		int ret = allocator->allocate(cfg.stream());
		if (ret < 0) {
			std::cerr << "Can't allocate buffers" << std::endl;
			return -1;

		}
		size_t allocated = allocator->buffers(cfg.stream()).size();
		std::cout << "Allocated " << allocated << " buffers for stream" << std::endl;
	}

	// Capture the actual frame.
	Stream *stream = streamConfig.stream();
	const std::vector<std::unique_ptr<FrameBuffer>> &buffers = allocator->buffers(stream);
	std::vector<std::unique_ptr<Request>> requests;

	for (unsigned int i = 0; i < buffers.size(); i++) {
		std::unique_ptr<Request> request = camera->createRequest();
		if (!request) {
			std::cerr << "Can't create request." << std::endl;
			return -1;
		}
		const std::unique_ptr<FrameBuffer> &buffer = buffers[i];
		int ret = request->addBuffer(stream, buffer.get());
		if (ret < 0) {
			std::cerr << "Can't set buffer for request" << std::endl;
			return -1;
		}

		requests.push_back(std::move(request));
	}

	camera->requestCompleted.connect(requestComplete);
	camera->start();

	for (std::unique_ptr<Request> &request : requests) {
		camera->queueRequest(request.get());
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Nope!" << std::endl;
		return 0;
	}

	RenderWindow window("SDL Window", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer* renderer = window.GetRenderer();

	if (renderer == NULL) {
		std::cout << "Nope again!" << std::endl;
	}

	bool gameRunning = true;
	SDL_Event event;

	while (gameRunning) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				gameRunning = false;
			}
		}
		SDL_SetRenderDrawColor(renderer, 169, 169, 169, 1);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
	window.close();
	
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}





	camera->stop();
	allocator->free(stream);
	delete allocator;
	camera->release();
	camera.reset();
	cm->stop();
	return 0;
}
