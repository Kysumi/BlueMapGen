#pragma once
#include <ChakraCommon.h>
#include <string>

class Chakra
{
public:
	Chakra();
	void runScript(std::string fileName);

	void shutDown();
	
private:

	void consoleLog();
	
	unsigned currentSourceContext = 0;
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result;
};

