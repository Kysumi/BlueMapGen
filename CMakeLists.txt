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
    message(FATAL_ERROR "You should set SFML_DIR to run this sample.
            You may set it from CMake or command line (-DSFML_DIR=<path>)
            ")
endif ()

file(GLOB CHAKRA ${PROJECT_SOURCE_DIR}/src/ChakraCore/*.cpp)

include(${PROJECT_SOURCE_DIR}/src/ChakraCore/CMakeLists.txt)

add_executable(OnGrid src/main.cpp ${ChakraSource} src/WindowManager.cpp src/WindowManager.h src/Map/Node.cpp src/Map/Node.h src/Binding/GridBinding.cpp src/Binding/GridBinding.h src/Map/Grid.cpp src/Map/Grid.h src/Binding/WindowBinding.cpp src/Binding/WindowBinding.h src/Binding/NodeBinding.cpp src/Binding/NodeBinding.h src/Binding/VectorBinding.cpp src/Binding/VectorBinding.h)

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

#file(COPY assets/scripts DESTINATION ./)

add_custom_command(TARGET OnGrid POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:OnGrid>/assets)