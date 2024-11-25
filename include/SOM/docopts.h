#pragma once
#ifndef DOCOPTS_H
#define DOCOPTS_H

#include <SOM/BaseTypes.h>
#include <SOM/coding.h>
#include <map>
#include <set>
class DocOpts
{
public:
    inline DocOpts(CONST_C_STRING help) : mHelp(help) {}
    bool process(INT32 argc, const CONST_C_STRING* argv, INT32 start = 1);
    bool getValue(CONST_C_STRING& value, CHAR key) const;
    inline bool isSet(CHAR key) const
    {
        return mSwitches.find(key) != mSwitches.end();
    }
    void reset();
    inline const CONST_C_STRING* args() const { return mArgs; }
    inline INT32 argc() const { return mArgc; }
    void toShell() const;
    void help(CONST_C_STRING argv0) const;
    inline const std::set<CHAR>& activeSwitches() const { return mSwitches; }
private:
    std::map<CHAR, CONST_C_STRING> mValues;
    std::set<CHAR> mSwitches;
    std::set<CHAR> mValueKeys;
    std::set<CHAR> mSwitchKeys;
    bool mOk = false;
    const CONST_C_STRING* mArgs = nullptr;
    INT32 mArgc = 0;
    const CONST_C_STRING mHelp = nullptr;

    NODEF(DocOpts)
    NOCOPY(DocOpts)
};
#endif // _H
