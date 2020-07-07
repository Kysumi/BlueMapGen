#include <ChakraCommon.h>

class Chakra {
public:
    static void Init(std::string scriptsDir);
    static bool RunScript(const std::string& filePath);

private:
    static HRESULT CreateRuntime(JsRuntimeHandle *runtime);
    static HRESULT CreateParserStateAndRunScript(const char* fileName, LPCSTR fileContents, size_t fileLength, JsFinalizeCallback fileContentsFinalizeCallback, char *fullPath);
    static HRESULT GetSerializedBuffer(LPCSTR fileContents, JsFinalizeCallback fileContentFinalizeCallback, JsValueRef *byteCodeBuffer);
    static HRESULT CreateAndRunSerializedScript(const char* fileName, LPCSTR fileContents, size_t fileLength, JsFinalizeCallback fileContentsFinalizeCallback, char *fullPath);
    static HRESULT CreateLibraryByteCodeHeader(LPCSTR contentsRaw, JsFinalizeCallback contentsRawFinalizeCallback, DWORD lengthBytes, LPCWSTR bcFullPath, LPCSTR libraryNameNarrow);
    static HRESULT RunScript(const char *fileName, LPCSTR fileContents, size_t fileLength,
                      JsFinalizeCallback fileContentsFinalizeCallback, JsValueRef bufferValue, char *fullPath,
                      JsValueRef parserStateCache);

    static HRESULT GetParserStateBuffer(LPCSTR fileContents, JsFinalizeCallback fileContentsFinalizeCallback, JsValueRef *parserStateBuffer);
    static HRESULT CreateParserState(LPCSTR fileContents, size_t fileLength, JsFinalizeCallback fileContentsFinalizeCallback, LPCWSTR fullPath);

    static std::string GetFilePath(const std::string& filePath);

    static std::string scriptsDir;
    static JsRuntimeHandle chRuntime;
    static BOOL doTTRecord;
    static JsRuntimeAttributes jsrtAttributes;

    static JsRuntimeHandle runtime;
    static JsContextRef context;

    typedef struct {
        void *scriptBody;
        JsFinalizeCallback scriptBodyFinalizeCallback;
        bool freeingHandled;
    } SerializedCallbackInfo;
};