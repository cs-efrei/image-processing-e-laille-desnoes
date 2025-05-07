cmake_minimum_required(VERSION 3.30)
project(LailleDesnoes_projetimage C)

set(CMAKE_C_STANDARD 99)

add_executable(LailleDesnoes_projetimage main.c
        t_bmp8.c
        t_bmp8.h)
