#include <SOM/BaseLib.h>
#include <ciso646>

CONST_C_STRING baseName(CONST_C_STRING path)
{
    CONST_C_STRING ret = path;
    for (CONST_C_STRING p = path; *p; ++p)
    {
        if (*p == '/' or *p == '\\')
        {
            ret = p + 1;
        }
    }
    return ret;
}
