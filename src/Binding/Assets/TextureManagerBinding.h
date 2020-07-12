//
// Created by Scott on 12/07/2020.
//

#ifndef ONGRID_TEXTUREMANAGERBINDING_H
#define ONGRID_TEXTUREMANAGERBINDING_H


#include <ChakraCommon.h>

class TextureManagerBinding {

public:
    static void bind();

private:
    static JsValueRef
    LoadFromFile(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
          void *callbackState);
};


#endif //ONGRID_TEXTUREMANAGERBINDING_H
