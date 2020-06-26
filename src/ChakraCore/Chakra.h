#include <ChakraCommon.h>

class Chakra {
public:
    static bool runScripty();

private:

    static HRESULT RunScript(const char *fileName, LPCSTR fileContents, size_t fileLength,
                      JsFinalizeCallback fileContentsFinalizeCallback, JsValueRef bufferValue, char *fullPath,
                      JsValueRef parserStateCache);

    static JsRuntimeHandle chRuntime;
    static const size_t ttUriBufferLength = MAX_PATH * 3;
    static char ttUri[ttUriBufferLength];
    static UINT32 startEventCount;
    static BOOL doTTRecord;
    static size_t ttUriLength;
    static JsRuntimeAttributes jsrtAttributes;

    static bool CHAKRA_CALLBACK DummyJsSerializedScriptLoadUtf8Source(
            JsSourceContext sourceContext,
            JsValueRef *scriptBuffer,
            JsParseScriptAttributes *parseAttributes);

    static HRESULT RunBgParseSync(LPCSTR fileContents, UINT lengthBytes, const char* fileName);

    typedef struct {
        void *scriptBody;
        JsFinalizeCallback scriptBodyFinalizeCallback;
        bool freeingHandled;
    } SerializedCallbackInfo;
};