#pragma once
// For some reason this import is required.
#include <SFML/Graphics.hpp>
#include <ChakraCommon.h>

namespace og
{
	class GridBinding
	{
	public:
		static void bind();
		static JsValueRef JSGridPrototype;
	};
}
