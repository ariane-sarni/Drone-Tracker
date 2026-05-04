#include "context.h"
#include "window/window.h"
#include "camera/camera.h"


int main(){
    auto cameraManager = createManager();
    auto cameraList = getCameraList(*cameraManager);
    std::string cameraID = getCameraID(*cameraList[0]);

    camera = obtainCamera(*cameraManager, cameraID);
    acquireCamera(*camera);

    auto config = makeConfig(*camera);
    auto &streamConfig = getStreamConfig(*config);
    changeConfig(streamConfig);
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
    int width = 640;
    int height = 480;
    auto window = createWindow("Testing", width, height);
    auto renderer = createRenderer(window);
    auto texture = createTexture(renderer, width, height);
    windowLoop(texture, renderer, width);
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