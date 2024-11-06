#pragma once
#ifndef FIO_H
#define FIO_H

#include <SOM/BaseTypes.h>
#include <string>
#include <fstream>

bool read(std::string& target, CONST_C_STRING file, bool err=true);

inline bool read(std::string& target, const std::string& file, bool err=true)
{
    return read(target, file.c_str(), err);
}

bool checkos(const std::ofstream& os, CONST_C_STRING file);

inline bool checkos(const std::ofstream& os, const std::string& file)
{
    return checkos(os, file.c_str());
}

#endif // _H
