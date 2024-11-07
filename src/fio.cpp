#include <SOM/fio.h>
#include <SOM/TraceMacros.h>
#include <iostream>

bool read(std::string& target, const CONST_C_STRING file, const bool err)
{
    std::ifstream is(file);
    const bool ok = is.good();
    if (ok)
    {
        TRACE("<- " << file)
        target.assign((std::istreambuf_iterator<char>(is)),
            std::istreambuf_iterator<char>());
    }
    else if (err)
    {
        std::cerr << "read error: " << file << '\n';
    }
    is.close();
    return ok;
}

bool open(std::ofstream& os, const std::filesystem::path& file, std::ios_base::openmode m)
{
    os.open(file, m);
    const bool ok = os.good();
    if (ok)
    {
        TRACE("-> " << file)
    }
    else
    {
        std::cerr << "write error: " << file << '\n';
        os.close();
    }
    return ok;
}

bool checkos(const std::ofstream& os, const CONST_C_STRING file)
{
    const bool ok = os.good();
    if (ok)
    {
        TRACE("-> " << file)
    }
    else
    {
        std::cerr << "write error: " << file << '\n';
    }
    return ok;
}
