#include "Chakra.h"

//https://github.com/microsoft/Chakra-Samples/blob/master/ChakraCore%20Samples/JSRT%20Hosting%20Samples/C%2B%2B/ChakraCoreHost/ChakraCoreHost.cpp
//https://github.com/microsoft/ChakraCore/wiki/JavaScript-Runtime-%28JSRT%29-Overview

// Task queue example
// https://github.com/Microsoft/Chakra-Samples/tree/master/ChakraCore%20Samples/OpenGL%20Engine/OpenGLEngine

int main()
{
    Chakra chakra;
    chakra.runScript("test.js");
    chakra.shutDown();

    return 0;
}