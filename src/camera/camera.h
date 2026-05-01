#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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



// Okay. Things to do.
// Initialize Camera Manager.
//  I could list all camera ids. Not neccesary.
// Initialize cameras.
// Acquire camera.
// Create configuration for camera, change its settings, validate them.
// Allocate frames.
// Set up camera. Connect it and start it.
// Request from camera.
// Clean up.

// According to good value semantics for C++, following should happen:
// Modern C++, objects are usually cheap to move.
// Want data to generally flow in one direction in functions (out)
// RAII - Object should be fully functional moment its constructed.
// Example of that is for manager for camera list.
// As of C++17, returning by value isnt that slow honestly.
// Basically we are following Value-Based design.
// Treat complex objects the same way you'd treat an int.
// Piece of data that can be created, returned, or passed around without worring
// about pointers/dynamic memory

// Needs integration with SDL. Work out later, for now, simplify camera logic.,
// Minimum ~6 functions.

// Done!
// Creates manager and starts it.
// Args: None
// Returns: Unique pointer to camera manager
std::unique_ptr<libcamera::CameraManager> createManager();

// Creates a camera object with CameraID type from the Camera Manager, and grabs
// it. Args: Initialized Manager, Camera ID Returns: Pointer to Camera object
std::shared_ptr<libcamera::Camera>
obtainCamera(libcamera::CameraManager &Manager, std::string cameraID);

// Initialize actual camera.
// Args: Reference to Camera
// Rets: Camera ID
std::string getCameraID(libcamera::Camera &Cam);


// Done!
// Makes a list of cameras based off camera manager.
// Args: Reference to Camera Manager
// Rets: Vector of shared pointers all being cameras.
std::vector<std::shared_ptr<libcamera::Camera>>
getCameraList(libcamera::CameraManager &Manager);

// Done!
// Acquires camera.
// Args: Reference to a camera
// Rets: Nothing
void acquireCamera(libcamera::Camera &Cam);

// Function to pass in configuration files
// Args: Reference to Camera
// Rets: Fail or success
int configureCamera();

// Makes configuration based off camera.
// Args: Reference to camera
// Returns: Unique pointer for camera configuration
std::unique_ptr<libcamera::CameraConfiguration> makeConfig(libcamera::Camera &cam);

// Returns Stream configuration of a config.
// Args: Camera configuration
// Returns: Stream configuration object of said camera config. 
libcamera::StreamConfiguration getStreamConfig(libcamera::CameraConfiguration &config);

// Changes config based on inputs. Currently hardcoded for testing purposes
// Args: Reference to stream configuration file based off cameras config
// Returns: None
void changeConfig(libcamera::StreamConfiguration &config);

// Validates a configuration of a given config for a camera.
// Args: Camera reference, Unique pointer to camera config 
// Returns: Nothing
void validateConfig(libcamera::Camera &cam, std::unique_ptr<libcamera::CameraConfiguration> &config);

// Returns a pointer to a FrameBufferAllocator. So amount of memory from camera config.
// Args: Ref to camera you want to grab memory from. 
// Returns: Pointer for frames that have been allocated.
libcamera::FrameBufferAllocator* FrameAllocatorCreator(std::shared_ptr<libcamera::Camera> &cam);

// Allocates memory based on configuration to the buffer we made.
// Args: Allocator, camera config
// Returns: Nothing
void allocateFrameMemory(libcamera::FrameBufferAllocator &allocator, libcamera::CameraConfiguration &config);

// Creates stream object based off configuration file. 
// Args: Stream configuration file for camera
// Returns: Stream pointer
libcamera::Stream* createStream(libcamera::StreamConfiguration &streamConfig);

// Creates buffer vector for camera
// Args: Stream pointer, allocator pointer
// Returns: Reference to frame buffer vector
const std::vector<std::unique_ptr<libcamera::FrameBuffer>> &createBufferVector(libcamera::Stream *stream, libcamera::FrameBufferAllocator *allocator);

// Creates a request vector.
// Args: None
// Returns: A request vector. Vector full of camera requests.
std::vector<std::unique_ptr<libcamera::Request>> createRequestVector();

// Fills request vector.
// Args: Request vector, buffer, camera, stream
// Returns: None
void fillRequests(std::vector<std::unique_ptr<libcamera::Request>> &requests, const std::vector<std::unique_ptr<libcamera::FrameBuffer>> &buffers, libcamera::Camera &camera, libcamera::Stream* &stream);

// Calls completeRequest on camera object. 
// Args: Camera reference
// Rets: Nothing 
void completeCameraRequest(libcamera::Camera &camera);