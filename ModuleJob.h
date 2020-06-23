#pragma once
#include <ChakraCore.h>
#include <string>

struct ModuleJob
{
	JsModuleRecord Module;

	std::string Script;

	std::string SourceUrl;

	bool IsParsed;
};
