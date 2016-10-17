cmake_minimum_required(VERSION 2.8)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES
        foo_tests/basic_check.cpp
        foo_tests/FooFixture.cpp
        recepcionista_test/RecepcionistaFixture.cpp
        mozo_test/MozoRealizaPedidoComida.cpp)


add_executable(run_tests main.cpp ${SOURCE_FILES} )

add_subdirectory(lib/googletest)
include_directories(foo_tests)


target_link_libraries(run_tests gtest gtest_main libRestaurantConcurrente)