cmake_minimum_required(VERSION 3.16)
project(Counter-UAS)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)
pkg_check_modules(LIBCAMERA_BASE REQUIRED IMPORTED_TARGET libcamera-base)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(LIBJPEG REQUIRED IMPORTED_TARGET libjpeg)

add_executable(Counter-UAS 
        src/main.cpp
        src/camera/camera.cpp
        src/window/window.cpp)

#add_executable(Camera-Test src/camera/camera.cpp)

target_include_directories(Counter-UAS PRIVATE ${SDL2_INCLUDE_DIRS})

set(COMMON_LIBS
        PkgConfig::LIBCAMERA
        PkgConfig::LIBCAMERA_BASE
        PkgConfig::SDL2
        PkgConfig::SDL2_IMAGE
        PkgConfig::LIBJPEG
)

target_link_libraries(Counter-UAS ${COMMON_LIBS})
#target_link_libraries(Camera-Test ${COMMON_LIBS})