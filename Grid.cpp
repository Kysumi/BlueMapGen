#include "Grid.h"
#include <cassert>
#include "Binding.h"
#include "Setting.h"

JsValueRef Grid::JSGridPrototype;

Grid::Grid(sf::Vector2i size)
{
	this->size = size;

	grid = std::vector<std::vector<Node>>(size.x, std::vector<Node>(size.y));

	srand(time(NULL));
	
	for (auto xAxis = 0; xAxis < size.x; xAxis++) {		
		for (auto yAxis = 0; yAxis < size.y; yAxis++) {
			
			Node node(xAxis, yAxis, nodeSize);

			/* generate secret number between 1 and 10: */
			auto number = rand() % 10 + 1;

			if (number > 7)
			{
				node.born();
			}
			
			grid[xAxis][yAxis] = node;
		}
	}
}

Grid::~Grid()
{
	
}

void Grid::Draw(sf::RenderWindow& window)
{
	for (auto xAxis = 0; xAxis < size.x; xAxis++) {
		for (auto yAxis = 0; yAxis < size.y; yAxis++) {
			auto node = grid[xAxis][yAxis];
			node.draw(window);
		}
	}
}

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
	JsSetPrototype(output, Grid::JSGridPrototype);

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
	}else
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

void Grid::BindToJS()
{
	std::vector<const wchar_t*> memberNames;
	std::vector<JsNativeFunction> memberFuncs;
	
	memberNames.push_back(L"process");
	memberFuncs.push_back(JSProcess);
	
	memberNames.push_back(L"getNeighbours");
	memberFuncs.push_back(JSGetNeighbours);

	memberNames.push_back(L"draw");
	memberFuncs.push_back(JSDraw);
	
	// projectNativeClass(L"Point", JSPointConstructor, JSPointPrototype, memberNames, memberFuncs);
	
	Binding::ProjectNativeClass(L"Grid", JSGridConstructor, JSGridPrototype, memberNames, memberFuncs);
	
}

void Grid::Process()
{
	// Replicate grid state to copy
	auto gridCopy = grid;

	for (auto xAxis = 0; xAxis < size.x; xAxis++) {
		for (auto yAxis = 0; yAxis < size.y; yAxis++) {
			
			auto node = gridCopy[xAxis][yAxis];
			auto count = getAliveNeighbours(sf::Vector2i(xAxis, yAxis)).size();

			if (count < 2) {
				gridCopy[xAxis][yAxis].kill();
			}
			else if (count > 3) {
				gridCopy[xAxis][yAxis].kill();
			}
			else if (count == 3) {
				gridCopy[xAxis][yAxis].born();
			}
		}
	}

	// Flip array back again
	grid = gridCopy;
}

std::vector<Node> Grid::getAliveNeighbours(sf::Vector2i position)
{
	auto neighbours = getNeighbours(position);
	std::vector <Node> aliveNodes;

	for (auto node : neighbours)
	{
		if (node.isAlive())
		{
			aliveNodes.push_back(node);
		}
	}

	return aliveNodes;
}

std::vector<Node> Grid::getNeighbours(sf::Vector2i position)
{
	return getNeighbours(position.x, position.y);
}

std::vector<Node> Grid::getNeighbours(int x, int y)
{
	std::vector<Node> neigbours;
	
	for (auto xAxis = x - 1; xAxis <= x + 1; xAxis++)
	{
		for (auto yAxis = y - 1; yAxis <= y + 1; yAxis++)
		{
			if (xAxis < 0 || yAxis < 0)
			{
				continue;
			}

			if (xAxis > grid.size() - 1 || yAxis > grid[0].size() - 1)
			{
				continue;
			}

			// Its the node we are trying to find the neighbours of
			if (xAxis == x && yAxis == y)
			{
				continue;
			}
			
			neigbours.push_back(grid[xAxis][yAxis]);
		}
	}

	return neigbours;
}
