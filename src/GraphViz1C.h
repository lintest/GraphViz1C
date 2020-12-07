#ifndef __GRAPHVIZ1C_H__
#define __GRAPHVIZ1C_H__

#include "AddInNative.h"

class GraphViz1C:
    public AddInNative
{
private:
    static std::vector<std::u16string> names;
    GraphViz1C();
    int64_t value;
private:
    std::u16string text;
    std::u16string getTestString();
    void setTestString(const std::u16string &text);
};
#endif //__GRAPHVIZ1C_H__