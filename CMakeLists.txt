cmake_minimum_required(VERSION 3.30)
project(image_processing_veclin_moussy_int1 C)

set(CMAKE_C_STANDARD 11)

add_executable(image_processing_veclin_moussy_int1
        main.c
        bmp8.c
        bmp8.h
        bmp24.c
        bmp24.h
        utils.c
        utils.h)
