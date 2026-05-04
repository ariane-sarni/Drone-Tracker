// window.cpp
#include "window.h"
#include "../context.h"



void throw_error(int ret, const std::string &message) {
  std::cerr << "FATAL ERROR: " << message << " (Error code: " << ret << (")");
  std::exit(EXIT_FAILURE);
}

RenderWindow::RenderWindow(const char* title, int width, int height) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        throw_error(0, "SDL could not open window.");
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        throw_error(0, "SDL Could not create renderer.");
    }
}

SDL_Renderer* RenderWindow::getRenderer() {
    return renderer;
}

void RenderWindow::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
    SDL_Quit();
}

// Need following functions:
// Call to initialzie 
// Configure the texture 
// Main loop
// Clean up


void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw_error(0, "SDL_Init failed.");
    }
}

RenderWindow createWindow(const char* title, int width, int height) {
    return RenderWindow(title, width, height);
}

SDL_Renderer* createRenderer(RenderWindow& window) {
    SDL_Renderer* renderer = window.getRenderer();
    if (renderer == NULL) {
        throw_error(0, "Renderer creation failed!");
    }
    return renderer;
}

SDL_Texture* createTexture(SDL_Renderer* &renderer, int width, int height) {
    return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
}


void windowLoop(SDL_Texture* &cameraTexture, SDL_Renderer* &renderer, int width) {
    bool windowOpen = true;
    SDL_Event event; 
    while (windowOpen) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                windowOpen = false;
            }
        }
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            if (newFrameAvailable) {
                SDL_UpdateTexture(cameraTexture, NULL, sharedFrameData.data(), width * 3);
                newFrameAvailable = false;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, cameraTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}
