//detect.cpp 

#include "detect.h"
#include "../context.h"
//#include "../"

// Downloaded yolo weights. 
// Read this API documentation to learn how to work it
// https://onnxruntime.ai/docs/api/c/namespace_ort.html 
// Basic steps: 
// Pass frames from buffer in camera
// Resize to 640x640
// Normalize pixel values to float 0-1
// Run ONNX Inference 
// Parse bounding boxes back out 
// Draw boxes onto RGB buffer 
// Pass to SDL 

// This is wrong. Either need to make Env somewhere else and store it in the main function, then pass as reference here, then return sesh as refernce
// OR make this in a Class. Might be easiest way honestly.
Ort::Session prepareEnviornment() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING);
    Ort::SessionOptions session_options;
    OrtCUDAProviderOptions cuda_options;
    cuda_options.device_id = 0;
    session_options.AppendExecutionProvider_CUDA(cuda_options);
    Ort::Session session(env, "yolov8s.onnx", session_options);
    return session;
}

int test(){
    // 1. Create the enviornment 
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING);
    // 2. Create session options 
    Ort::SessionOptions session_options;
    // 3. Create provider options 
    OrtCUDAProviderOptions cuda_options;
    // 4. Pick device id.
    cuda_options.device_id = 0;
    // 5. Put cuda options onto session options
    session_options.AppendExecutionProvider_CUDA(cuda_options);
    // 6. Create a session in the env with cuda options, and using yolov8 data.
    Ort::Session session(env, "yolov8s.onnx", session_options);

    // So now we have the Ort session running. Now need to pass frames over here, resize images, normalize pixels, and run inference. 

    // First step is decoding the frame from MJPEG to RGB. 
    // To do this, we need the actual frames. 
    // Lets pass the frame.
    // First, need to make helpers here so I can just call them in main. 


}

// So I need to do the following:
// 1. Decode MJPEG -> RGB, since YOLO needs raw pixels, and we get MJPEG from libcamera
// 2. Convert to float32 and normalize, so divice each pixel value by 255 to get values between 0-1
// 3. Convert from HWC to CHW. Libcamera has values as [R,G, B, R, G, B] etc (Height x Width x Channels), but YOLO needs (Channels x Height x Width). So from
// HWC -> CHW
// 4. Create tensor with shape (1, 3, 640, 640)
// 5. Run session 