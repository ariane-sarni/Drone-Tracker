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

