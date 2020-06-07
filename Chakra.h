#pragma once
#include <ChakraCommon.h>
#include <queue>
#include <string>
#include "Task.h"

class Chakra
{
public:
	Chakra();
	void runScript(std::string fileName);

	void shutDown();
	void pushTask(Task *task);
	
private:	
	unsigned currentSourceContext = 0;
	JsRuntimeHandle runtime;
	JsContextRef context;
	std::queue<Task*> taskQueue;
};

