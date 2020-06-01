#include <v8.h>
#include <assert.h>
#include <stdio.h>
#include <libplatform\libplatform.h>


static bool run_shell;

int main(int argc, char* argv[]) {
    
    run_shell = (argc == 1);
    int result;
    {
        // HACKS
        v8::Local<v8::String> file_name =
            v8::String::NewFromUtf8(isolate, "test.js", v8::NewStringType::kNormal)
            .ToLocalChecked();

        v8::Local<v8::String> source;

        if (!ReadFile(isolate, "test.js").ToLocal(&source)) {
            fprintf(stderr, "Error reading '%s'\n", "test.js");
            //continue;
        }

        bool success = ExecuteString(isolate, source, file_name, true, true);
        while (v8::platform::PumpMessageLoop(platform.get(), isolate)) continue;
        if (!success)
            result = 1;
        else
            result = 0;
        // END
    }
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;

    return result;
}
// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}
