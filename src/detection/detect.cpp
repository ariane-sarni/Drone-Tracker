//detect.cpp 

#include "detect.h"

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

int main(){
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
}