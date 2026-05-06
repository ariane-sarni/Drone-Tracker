// detect.h

#include <cstring>
#include <iomanip>
#include <iostream>
#include <jpeglib.h>
#include <libcamera/formats.h>
#include <libcamera/libcamera.h>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <stdio.h>
#include <sys/mman.h>
#include <thread>
#include <onnxruntime/onnxruntime_cxx_api.h>

// Using onnx run time and YOLO trained data!