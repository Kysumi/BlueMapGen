#pragma once

#include <libplatform\libplatform.h>
#include <v8.h>

class V8Wrapper
{
public:
    void startV8();

    v8::Local<v8::Context> CreateShellContext(v8::Isolate* isolate);
    bool ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
        v8::Local<v8::Value> name, bool print_result,
        bool report_exceptions);

    //void Print(const v8::FunctionCallbackInfo<v8::Value>& args);
    //void Read(const v8::FunctionCallbackInfo<v8::Value>& args);
    //void Load(const v8::FunctionCallbackInfo<v8::Value>& args);
    //void Quit(const v8::FunctionCallbackInfo<v8::Value>& args);
    //void Version(const v8::FunctionCallbackInfo<v8::Value>& args);

    //v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name);

    void ReportException(v8::Isolate* isolate, v8::TryCatch* handler);
};

