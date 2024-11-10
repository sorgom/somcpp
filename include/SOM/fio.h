#pragma once
#ifndef FIO_H
#define FIO_H

#include <SOM/BaseTypes.h>
#include <string>
#include <fstream>
#include <filesystem>

bool read(std::string& target, CONST_C_STRING file, bool err=true);

inline bool read(std::string& target, const std::string& file, bool err=true)
{
    return read(target, file.c_str(), err);
}

bool open(std::ofstream& os, const std::filesystem::path& file, std::ios_base::openmode m = std::ios_base::out);

inline bool open(std::ofstream& os, const std::filesystem::path&& file, std::ios_base::openmode m = std::ios_base::out)
{
    return open(os, file, m);
}

#endif // _H
