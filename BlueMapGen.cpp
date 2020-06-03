#include <v8.h>
#include <assert.h>
#include <stdio.h>
#include <libplatform\libplatform.h>
#include "V8Wrapper.h"

int main(int argc, char* argv[]) {
    
    V8Wrapper v8;

    v8.startV8();

    v8.runScript("test.js");

    v8.shutdownV8();


    return 0;
}