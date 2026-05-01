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


class RenderWindow {
    public:
        RenderWindow(const char* title, int width, int height);
        SDL_Renderer* getRenderer();
        void close();
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
};