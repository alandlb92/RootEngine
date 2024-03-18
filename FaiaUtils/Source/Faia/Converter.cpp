#include "pch.h"
#include "Converter.h"
#include <stdexcept>

LPCWSTR Faia::Converter::CharToLPCWSTR(const char* cStr)
{
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, cStr, -1, NULL, 0);
    if (bufferSize == 0) {
        throw std::invalid_argument("Faile to get string size");
    }

    wchar_t* wideCharBuffer = new wchar_t[bufferSize];
    if (MultiByteToWideChar(CP_UTF8, 0, cStr, -1, wideCharBuffer, bufferSize) == 0) {
        throw std::invalid_argument("Faile to convert string");
        delete[] wideCharBuffer;
    }

    LPCWSTR vertexShaderNameWide = wideCharBuffer;

    return vertexShaderNameWide;
}

const char* Faia::Converter::LPCWSTRToChar(LPCWSTR wideStr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);
    if (bufferSize == 0) {
        throw std::invalid_argument("Falha ao obter o tamanho da string");
    }

    char* buffer = new char[bufferSize];
    if (WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, buffer, bufferSize, NULL, NULL) == 0) {
        delete[] buffer;
        throw std::invalid_argument("Falha ao converter a string");
    }

    return buffer;
}