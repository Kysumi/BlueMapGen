#pragma once
#include <libplatform/libplatform.h>
#include <assert.h>
#include <v8.h>

class Helper
{
public:
    static std::string getFileContents(const char* filename)
    {
        std::ifstream in(filename, std::ios::in | std::ios::binary);
        if (in)
        {
            return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
        }
        throw(errno);
    }

    static const char* ToCString(const v8::String::Utf8Value& value) {
        return *value ? *value : "<string conversion failed>";
    }
};

