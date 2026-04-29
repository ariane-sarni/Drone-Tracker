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

// Needs integration with SDL. Work out later, for now, simplify camera logic.,
// Minimum ~6 functions. 

// Initialize camera manager
// Args: Reference to CameraManager class. 
// Rets: Fail or success 
int initializeManager(libcamera::CameraManager &Manager);

// Initialize actual camera.
// Args: Reference to Camera
// Rets: Fail or success 
int initializeCamera();

// Function to pass in configuration files
// Args: Reference to Camera
// Rets: Fail or success
 int configureCamera();
