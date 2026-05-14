#include "context.h"
#include "window/window.h"
#include "camera/camera.h"
#include "detection/detect.h"


int main(){
    int width = 800;
    int height = 448;
    auto cameraManager = createManager();
    auto cameraList = getCameraList(*cameraManager);
    std::string cameraID = getCameraID(*cameraList[0]);

    camera = obtainCamera(*cameraManager, cameraID);
    acquireCamera(*camera);

    auto config = makeConfig(*camera);
    auto &streamConfig = getStreamConfig(*config);
    changeConfig(streamConfig, width, height);
    validateConfig(*camera, config);
    auto allocator = FrameAllocatorCreator(camera);
    allocateFrameMemory(*allocator, *config);
    auto stream = createStream(streamConfig);
    const auto &buffers = createBufferVector(stream, allocator);
    auto requests = createRequestVector();
    fillRequests(requests, buffers, *camera, stream);
    completeCameraRequest(*camera);
    populateRequests(requests, *camera);

    // Now for window part 
    initSDL();
    auto window = createWindow("Testing", width, height);
    auto renderer = createRenderer(window);
    auto texture = createTexture(renderer, width, height);
    windowLoop(texture, renderer, width);
    
    // This should be done within the window loop somehow, or while it's looping essentially.
    // So now we have the session. We want to actually run it with a tensor. We cannot do that just yet
    // Before we do that, we must prepare the tensor.
    
    // So we need to move the MJPEG Frame in WindowLoop to RGB - Done! Just changed to RGB888.

    // Then we need to convert the rgb to float32 and normalize it. - Need to try this. 
    // We have everything in sharedFrameData. Do it with that. Making this function:  std::vector<std::vector<float>> getPixels(std::vector<uint8_t> data)

    // TFrom there, convert HWC to CHW. 
    // Then we can create the tensor and run the session.
    auto detectEnvironment = prepareEnviornment();
    

    // This is the ending stuff
    window.close();
    camera->stop();
    for (const auto& buffer : buffers) {
        munmap(mappedBuffers[buffer.get()], buffer->planes()[0].length);
    }
    allocator->free(stream);
    delete allocator;
    camera->release();
    camera.reset();
    cameraManager->stop();
    


    return 0;
}