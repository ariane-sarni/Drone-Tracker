#pragma once 

#include <iostream>
#include <memory>
#include <thread>
#include <iomanip>
#include <libcamera/libcamera.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <libcamera/formats.h>
#include <mutex>
#include <cstring>
#include <sys/mman.h>
#include <jpeglib.h>
#include <stdexcept>


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
// Piece of data that can be created, returned, or passed around without worring about pointers/dynamic memory



// Needs integration with SDL. Work out later, for now, simplify camera logic.,
// Minimum ~6 functions. 

// Done!
// Creates manager and starts it.
// Args: None 
// Returns: Unique pointer to camera manager
std::unique_ptr<libcamera::CameraManager> createManager();

// Creates a camera object with CameraID type from the Camera Manager, and grabs it.
// Args: Initialized Manager, Camera ID
// Returns: Pointer to Camera object
std::shared_ptr<libcamera::Camera> obtainCamera(libcamera::CameraManager &Manager, std::string cameraID);


// Initialize actual camera.
// Args: Reference to Camera
// Rets: Camera ID 
std::string getCameraID(libcamera::Camera &Cam);


// Done!
// Makes a list of cameras based off camera manager. 
// Args: Reference to Camera Manager 
// Rets: Vector of shared pointers all being cameras.
std::vector<std::shared_ptr<libcamera::Camera>> getCameraList(libcamera::CameraManager &Manager);

// Done!
// Acquires camera.
// Args: Reference to a camera
// Rets: Nothing
void acquireCamera(libcamera::Camera &Cam);

// Function to pass in configuration files
// Args: Reference to Camera
// Rets: Fail or success
 int configureCamera();
