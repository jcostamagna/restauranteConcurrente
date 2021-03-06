cmake_minimum_required(VERSION 2.8)
project(restaurantConcurrente)


set(CMAKE_CXX_FLAGS         "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions")

set(CONFIG_FILES
        config/restaurante.config )


set(SOURCE_FILES
        Pipe&Fifo/Pipe.cpp
        Pipe&Fifo/Fifo.cpp
        Pipe&Fifo/FifoEscritura.cpp
        Pipe&Fifo/FifoLectura.cpp
        Lock/LockFile.cpp
        Semaforos/Semaforo.cpp
        SharedMem/MemoriaCompartida2.h
        Signal/SignalHandler.cpp
        src/Parser.cpp
        src/Log.cpp src/Log.h
        src/Recepcionista.cpp src/Recepcionista.h
        src/Forkeable.cpp src/Forkeable.h
        src/Mozo.cpp src/Mozo.h
        src/Restaurante.cpp src/Restaurante.h
        src/Cocinero.cpp src/Cocinero.h
        src/GeneradorClientes.cpp src/GeneradorClientes.h
        Lock/LockFd.cpp Lock/LockFd.h
        src/Mesa.cpp src/Mesa.h
        Signal/Apagon_Handler.h
        Signal/Apagon_Handler_Procesos.h src/Gerente.cpp src/Gerente.h src/lockFiles.h)

add_executable(restaurantConcurrente main.cpp ${SOURCE_FILES} ${CONFIG_FILES})#${HEADER_FILES})
add_executable(apagon apagon.cpp)

add_custom_command(TARGET restaurantConcurrente POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:restaurantConcurrente>)


include_directories(
        src
        Lock
        Pipe&Fifo
        Semaforos
        SharedMem
        Signal)

add_subdirectory(test)
add_library(libRestaurantConcurrente STATIC ${SOURCE_FILES} )
