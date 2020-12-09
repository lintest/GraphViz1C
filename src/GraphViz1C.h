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
    static std::vector<std::u16string> names;
    GraphViz1C();
private:
    void render(VH source, const std::string& format, const std::string& layout);
    void formats(int64_t api = 3);
public:
    static std::ostream errors;
};
#endif //__GRAPHVIZ1C_H__