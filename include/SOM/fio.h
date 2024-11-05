#pragma once
#ifndef FIO_H
#define FIO_H

#include <SOM/BaseTypes.h>
#include <string>
#include <fstream>

bool read(std::string& target, CONST_C_STRING file, bool err=true);

bool checkos(const std::ofstream& os, CONST_C_STRING file);

inline bool checkos(const std::ofstream& os, std::string& file)
{
    return checkos(os, file.c_str());
}

#endif // _H
