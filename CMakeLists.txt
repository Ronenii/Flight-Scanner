cmake_minimum_required(VERSION 3.13)  # CMake version check
project(Flight-Scanner VERSION 1.0)     # Create project
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)              # Enable c++20 standard

#add custom lib
add_subdirectory(lib)

# Add main.cpp file of project root directory as source file
set(MAIN_FILE src/main.cpp)
set(SCRIPT_FILE flight_scanner.sh)

#add all src files
file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(Flight-Scanner ${MAIN_FILE} ${SRC_FILES})

target_link_libraries(Flight-Scanner PUBLIC lib)

target_include_directories(Flight-Scanner PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/"
        "/usr/local/include/"
        )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_FILE} ${CMAKE_BINARY_DIR}/${SCRIPT_FILE} COPYONLY)