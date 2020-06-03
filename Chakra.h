#pragma once
#include <ChakraCommon.h>
#include <string>
#include "Console.h"

class Chakra
{
public:
	Chakra();
	void runScript(std::string fileName);

	void shutDown();
	
private:
	Console console;

	void consoleLog();
	
	unsigned currentSourceContext = 0;
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result;
};

