#include "GridBinding.h"

#include <cassert>
#include "Binding.h"
#include "Grid.h"
#include "Setting.h"

JsValueRef og::GridBinding::JSGridPrototype;

// JsNativeFunction for Pointer constructor - Grid(x, y)
JsValueRef CALLBACK JSGridConstructor(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	assert(isConstructCall && argumentCount == 3);
	auto* output = JS_INVALID_REFERENCE;

	int x, y;
	JsNumberToInt(arguments[1], &x);
	JsNumberToInt(arguments[2], &y);

	auto* grid = new Grid(sf::Vector2i(x, y));

	JsCreateExternalObject(grid, nullptr, &output);
	JsSetPrototype(output, og::GridBinding::JSGridPrototype);

	return output;
}

JsValueRef CALLBACK JSProcess(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	assert(!isConstructCall && argumentCount == 1);
	JsValueRef output = JS_INVALID_REFERENCE;

	void* gridArg;

	if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
		auto* grid = static_cast<Grid*>(gridArg);
		grid->Process();
	};

	return output;
}

JsValueRef CALLBACK JSGetNeighbours(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	assert(!isConstructCall && argumentCount == 3);
	JsValueRef output = JS_INVALID_REFERENCE;

	void* gridArg;
	Grid* grid;

	// Load grid object from JSLAND
	if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
		grid = static_cast<Grid*>(gridArg);
	}
	else
	{
		throw "AHHHHHH no grid object";
	}

	int x, y;
	JsNumberToInt(arguments[1], &x);
	JsNumberToInt(arguments[2], &y);

	auto neighbours = grid->getNeighbours(x, y);
	JsCreateExternalObject(&neighbours, nullptr, &output);

	return output;
}

JsValueRef CALLBACK JSDraw(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	assert(!isConstructCall && argumentCount == 1);
	JsValueRef output = JS_INVALID_REFERENCE;

	void* gridArg;

	if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
		auto* grid = static_cast<Grid*>(gridArg);
		grid->Draw(*Setting::getActiveWindow());
	};

	return output;
}

void og::GridBinding::bind()
{
	std::vector<const wchar_t*> memberNames;
	std::vector<JsNativeFunction> memberFuncs;

	memberNames.push_back(L"process");
	memberFuncs.push_back(JSProcess);

	memberNames.push_back(L"getNeighbours");
	memberFuncs.push_back(JSGetNeighbours);

	memberNames.push_back(L"draw");
	memberFuncs.push_back(JSDraw);

	Binding::ProjectNativeClass(L"Grid", JSGridConstructor, JSGridPrototype, memberNames, memberFuncs);
}