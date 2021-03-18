// taken from https://theolizer.com/cpp-school2/cpp-school2-2/#typename.h
#pragma once
#include <string>
 
#if defined(__GNUC__)
    #include <cxxabi.h>
    std::string getNameByTypeInfo(std::type_info const& iTypeInfo)
    {
        char*   aName;
        int status = 0;
        aName = abi::__cxa_demangle(iTypeInfo.name(), 0, 0, &status);
        std::string ret(aName);
        std::free(aName);
        return ret;
    }
#else
    std::string getNameByTypeInfo(std::type_info const& iTypeInfo)
    {
        return iTypeInfo.name();
    }
#endif
 
// #define TYPENAME(dType) getNameByTypeInfo(typeid(dType))
