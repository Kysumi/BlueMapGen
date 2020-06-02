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

    static v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name) {
        v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(
            isolate, Helper::getFileContents("test.js").c_str(), v8::NewStringType::kNormal);
        return result;
    }
};

