#pragma once
#include <codecvt>
#include <iostream>
#include <fstream>

class Helper
{
public:
    static std::wstring getFileContents(const char* fileName)
    {
        std::ifstream in(fileName, std::ios::in | std::ios::binary);
        if (in)
        {
	        const auto string = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(string);
        }

        std::cout << "Could not load file: " << fileName << std::endl;
    	
        return L"";
    }

	static const std::wstring stdStringToWString(std::string string)
    {
	    return std::wstring(string.begin(), string.end());
    }
};

