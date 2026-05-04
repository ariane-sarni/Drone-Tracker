#pragma once

#include <vector>
#include <mutex>
#include <cstdint>
#include <map>
#include <memory>
#include <libcamera/libcamera.h>

void throw_error(int ret, const std::string &message);

inline std::mutex frameMutex;
inline std::vector<uint8_t> sharedFrameData;
inline bool newFrameAvailable = false;
inline std::map<libcamera::FrameBuffer*, void*> mappedBuffers;
inline std::shared_ptr<libcamera::Camera> camera;