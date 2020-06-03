#include "Chakra.h"
#include "Helper.h"

Chakra::Chakra()
{
    // Create a runtime. 
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

    // Create an execution context. 
    JsCreateContext(runtime, &context);

    // Now set the current execution context.
    JsSetCurrentContext(context);
	
    console = Console();
}

void Chakra::runScript(std::string fileName)
{
	const auto script = Helper::getFileContents(fileName.c_str());
    JsRunScript(script.c_str(), currentSourceContext++, L"", &result);

    // Convert your script result to String in JavaScript; redundant if your script returns a String
    JsValueRef resultJSString;
    JsConvertValueToString(result, &resultJSString);

    // Project script result back to C++.
    // const wchar_t* resultWC;
	
    // size_t stringLength;
    // JsStringToPointer(resultJSString, &resultWC, &stringLength);
    //
    // std::wstring resultW(resultWC);
    // std::cout << std::string(resultW.begin(), resultW.end()) << std::endl;
}

void Chakra::shutDown()
{
    // Dispose runtime
    JsSetCurrentContext(JS_INVALID_REFERENCE);
    JsDisposeRuntime(runtime);
}
