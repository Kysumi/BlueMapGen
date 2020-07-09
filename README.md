# OnGrid

## Setup

### ChakraCore

Clone the master version here https://github.com/microsoft/ChakraCore

### SFML

Download SFML from here here https://www.sfml-dev.org/download/sfml/2.5.1/

### Side Notes

Once it has finished compiling ChakraCore you'll need to copy the
ChakraCore.dll into the cmake build DIR


The DLL will be in /ChakraCoreSource/Build/VcBuild/bin/x64_release/


Ensure you are compiling for x64 on clion this is under

Settings -> Build, Execution, Deployment -> Toolchains -> "Architecture"


## Compiling 

Depending on your operating system you will need to do different steps 
as this project is depending on this cmake config

https://github.com/microsoft/Chakra-Samples/tree/master/ChakraCore%20Samples/Hello%20World/CMake