cmake_minimum_required(VERSION 2.8)
project(restaurantConcurrente)


set(CMAKE_CXX_FLAGS         "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



#set(HEADER_FILES Pipe&Fifo/Pipe.h)  #lsoslas
set(SOURCE_FILES
        Pipe&Fifo/Pipe.cpp
        Pipe&Fifo/Fifo.cpp
        Pipe&Fifo/FifoEscritura.cpp
        Pipe&Fifo/FifoLectura.cpp
        Lock/LockFile.cpp
        Semaforos/Semaforo.cpp
        SharedMem/MemoriaCompartida2.h
        Signal/SignalHandler.cpp)


#add_library(foo STATIC ${HEADER_FILES}  ${SOURCE_FILES} )


add_executable(restaurantConcurrente main.cpp ${SOURCE_FILES} )#${HEADER_FILES})

#include_directories(Pipe&Fifo)

#add_subdirectory(src)
#add_subdirectory(test)
