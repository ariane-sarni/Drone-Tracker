#include "camera.h"

using namespace libcamera;

namespace {
std::map<FrameBuffer*, void*> mappedBuffers;
std::mutex frameMutex;
static std::shared_ptr<Camera> camera;
std::vector<uint8_t> sharedFrameData;
bool newFrameAvailable = false;
}

void throw_error(int ret, const std::string &message) {
  std::cerr << "FATAL ERROR: " << message << " (Error code: " << ret << (")");
  std::exit(EXIT_FAILURE);
}

std::unique_ptr<CameraManager> createManager() {
  std::unique_ptr<CameraManager> cm = std::make_unique<CameraManager>();
  int ret = cm->start();
  if (ret < 0) {
    throw_error(ret, "Failed to initialize camera manager.");
  }
  return cm;
}

std::vector<std::shared_ptr<Camera>> getCameraList(CameraManager &Manager) {
  auto cameras = Manager.cameras();
  if (cameras.empty()) {
    throw_error(0, "Failed to find a camera. Did you initialize the camera "
                   "manager properly?");
    Manager.stop();
  }
  return cameras;
}

std::string getCameraID(Camera &Cam) { return Cam.id(); }

std::shared_ptr<Camera> obtainCamera(CameraManager &Manager,
                                     std::string cameraID) {
  auto camera = Manager.get(cameraID);
  if (!camera) {
    throw_error(0, "Failed to get camera. Either Camera Manager wasn't "
                   "initialized, or no cameras are plugged in.");
  }
  return camera;
}

void acquireCamera(Camera &Cam) {
  int ret = Cam.acquire();
  if (ret < 0) {
    throw_error(0, "Failed to acquire camera. Was it unplugged?");
  }
}

// Research this later, not sure what streamrole is. 
std::unique_ptr<CameraConfiguration> makeConfig(Camera &cam){
  return cam.generateConfiguration({StreamRole::Viewfinder});
}

StreamConfiguration getStreamConfig(CameraConfiguration &config) {
  return config.at(0);
}


// make a way to change this to what you want later.
void changeConfig(StreamConfiguration &config) {
  config.size.width = 1280;
  config.size.height = 720;
  config.pixelFormat = formats::MJPEG;
}

void validateConfig(Camera &cam, std::unique_ptr<CameraConfiguration> &config) {
  cam.configure(config.get());
}

FrameBufferAllocator* FrameAllocatorCreator(std::shared_ptr<Camera> &cam) {
  return new FrameBufferAllocator(cam);
}

void allocateFrameMemory(FrameBufferAllocator &allocator, CameraConfiguration &config){
  for (StreamConfiguration &cfg : config) {
    int ret = allocator.allocate(cfg.stream());
    if (ret < 0) {
      throw_error(0, "Can't allocate buffers.");
    }
  }
}

Stream* createStream(StreamConfiguration &streamConfig){
  return streamConfig.stream();
}

const std::vector<std::unique_ptr<FrameBuffer>> &createBufferVector(Stream *stream, FrameBufferAllocator *allocator){
  return allocator->buffers(stream);
}

std::vector<std::unique_ptr<Request>> createRequestVector(){
  std::vector<std::unique_ptr<Request>> requests; 
  return requests;
}

void fillRequests(std::vector<std::unique_ptr<Request>> &requests, const std::vector<std::unique_ptr<FrameBuffer>> &buffers, Camera &camera, Stream* &stream) {
  
  for (unsigned int i = 0; i < buffers.size(); i++) {
  
    std::unique_ptr<Request> request = camera.createRequest();
  
    if (!request) {
      throw_error(0, "Can't follow that request.");
    }
  
    const std::unique_ptr<FrameBuffer> &buffer = buffers[i];

    int fd = buffer->planes()[0].fd.get();
    size_t length = buffer->planes()[0].length;
    void *memory = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    mappedBuffers[buffer.get()] = memory;

    int ret = request->addBuffer(stream, buffer.get());
    if (ret < 0) {
      throw_error(0, "Can't set buffer for request");
    }

    requests.push_back(std::move(request));



  }
}

// Take a request.
static void requestComplete(Request *request) {
  // If it has been cancelled, finish.
  if (request->status() == Request::RequestCancelled) {
    return;
  }

  // Make a hash map with a stream pointer as a key, and frame buffer as value.
  const std::map<const Stream *, FrameBuffer*> &buffers = request->buffers();

  for (auto bufferPair: buffers) {
    FrameBuffer *buffer = bufferPair.second; 
    void *mem = mappedBuffers[buffer];
    size_t length = buffer->metadata().planes()[0].bytesused;

    if (mem != nullptr && length > 0) {
      struct jpeg_decompress_struct cinfo; 
      struct jpeg_error_mgr jerr; 
      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_decompress(&cinfo);
      jpeg_mem_src(&cinfo, (unsigned char*)mem, length);
      jpeg_read_header(&cinfo, TRUE);
      cinfo.out_color_space = JCS_RGB;
      jpeg_start_decompress(&cinfo);

      int rowStride = cinfo.output_width * cinfo.output_components;
      std::vector<uint8_t> decoded(cinfo.output_width * cinfo.output_height * cinfo.output_components);

      uint8_t *rowPtr = decoded.data();
      while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &rowPtr, 1);
        rowPtr += rowStride; 

      }
      jpeg_finish_decompress(&cinfo);
      jpeg_destroy_decompress(&cinfo);
      std::lock_guard<std::mutex> lock(frameMutex);
      sharedFrameData = std::move(decoded);
      newFrameAvailable = true;
    }
  }
  request->reuse(Request::ReuseBuffers);
  camera->queueRequest(request);
}

void completeCameraRequest(Camera &camera) {
  camera.requestCompleted.connect(requestComplete);
  camera.start();
}


void cameraTest() {
  // 1. Create camera manager, intialize it.
  // 2. Get camera list.
  // 3. Get Camera id, acquire camera.
  // 4. Set up config
  auto cameraManager = createManager();
  auto cameraList = getCameraList(*cameraManager);
  std::string cameraID = getCameraID(*cameraList[0]);

  camera = obtainCamera(*cameraManager, cameraID);
  acquireCamera(*camera);


  // Configuration stuff

  auto config = makeConfig(*camera);
  auto streamConfig = getStreamConfig(*config);
  changeConfig(streamConfig);
  validateConfig(*camera, config);
  
  // Frame buffer allocating for videos
  // Basically need to make a FrameBufferAllocator
  // Give it the neccesary memory from config 
  // 2 functions, creator allocator, allocate memory
  
  auto allocator = FrameAllocatorCreator(camera);
  allocateFrameMemory(*allocator, *config);


  // Then we can filling the request buffer
  // 1. Create request vector
  // 2. Create stream object 
  // 3. Create buffer vector 
  // 4. Fill request vector 

  auto stream = createStream(streamConfig);
  const auto &buffers = createBufferVector(stream, allocator);
  auto requests = createRequestVector();
  fillRequests(requests, buffers, *camera, stream);
  completeCameraRequest(*camera);

  // After this, everything else is an SDL task. Should be done in Window file.

}

int main() {
  cameraTest();
  return 0;
}