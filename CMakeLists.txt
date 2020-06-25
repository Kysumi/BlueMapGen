# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

project("OnGrid")

# set(ICU_INCLUDE_PATH ......) # -> if you have a custom ICU setup
# set(CHAKRACORE_PATH "................/Github/ChakraCore/") # -> location of the ChakraCore Repo
# set(CHAKRACORE_LIB_TYPE ....Release or Test or Debug) # -> by default Release
if (NOT CHAKRACORE_PATH)
    message(FATAL_ERROR "You should set CHAKRACORE_PATH to run this sample.
            You may set it from CMake or command line (-DCHAKRACORE_PATH=<path>)
            ")
endif ()

if (NOT SFML_DIR)
    message(FATAL_ERROR "You should set SFML_PATH to run this sample.
            You may set it from CMake or command line (-DSFML_PATH=<path>)
            ")
endif ()

file(GLOB CHAKRA ${PROJECT_SOURCE_DIR}/src/ChakraCore/*.cpp)

include(${PROJECT_SOURCE_DIR}/src/ChakraCore/CMakeLists.txt)

add_executable(OnGrid src/main.cpp ${ChakraSource})

message("-- Compiling ChakraCore..")

# Embed ChakraCore
include(ChakraCoreEmbed.cmake)

target_include_directories(OnGrid
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CHAKRACORE_INCLUDE_PATH} # add ChakraCore include path
        )

target_link_libraries(OnGrid
        ${CHAKRACORE_LINKER_OPTIONS} # add ChakraCore linker options
        )

# SFML Config
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
target_link_libraries(OnGrid sfml-graphics sfml-audio)