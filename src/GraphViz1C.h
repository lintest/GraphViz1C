#ifndef __GRAPHVIZ1C_H__
#define __GRAPHVIZ1C_H__

#include "AddInNative.h"
#include <iostream>
#include <sstream>

class GraphViz1C:
    public AddInNative
{
private:
    static std::stringbuf buffer;
    static std::ostream errors;
    static std::vector<std::u16string> names;
    GraphViz1C();
private:
    void render(const std::string &text, const std::string& format, const std::string& layout);
public:
    static void error(char* errMsg);
};
#endif //__GRAPHVIZ1C_H__