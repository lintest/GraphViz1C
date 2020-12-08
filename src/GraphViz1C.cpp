#include "GraphViz1C.h"
#include <common/types.h>
#include <cgraph/cgraph.h>
#include <gvc/gvc.h>
#include <gvc/gvio.h>

#pragma comment(lib, "vcruntime.lib")
#pragma comment(lib, "msvcmrt.lib")

std::vector<std::u16string> GraphViz1C::names = {
	AddComponent(u"GraphViz1C", []() { return new GraphViz1C; }),
};

GraphViz1C::GraphViz1C()
{
	AddFunction(u"Render", u"Сформировать",
		[&](VH data, VH format, VH layout) { this->render(data, format, layout); },
		{ { 1, u"svg"}, { 2, u"dot"  } }
	);
}

std::stringbuf GraphViz1C::buffer;
std::ostream GraphViz1C::errors(&buffer);

void GraphViz1C::error(char* errMsg)
{
	errors << errMsg;
}

static int error(char* errMsg)
{
	GraphViz1C::error(errMsg);
	return 0;
}

void GraphViz1C::render(const std::string& text, const std::string& format, const std::string& layout)
{
	agseterrf(::error);
	char* data = nullptr;
	unsigned int size = 0;
	auto gvc = gvContextPlugins(lt_preloaded_symbols, 0);
	if (gvc) {
		auto g = agmemread(text.c_str());
		if (g) {
			if (gvLayout(gvc, g, layout.c_str()) == 0) {
				if (gvRenderData(gvc, g, format.c_str(), &data, &size) == 0) {
					try {
						result.AllocMemory(size);
						memcpy(result.data(), data, size);
					}
					catch (...) {
						result = "Memory allocation error";
					}
					gvFreeRenderData(data);
				}
			}
			gvFreeLayout(gvc, g);
			agclose(g);
		}
		gvFreeContext(gvc);
	}
	if (result.type() == VTYPE_EMPTY) {
		result = buffer.str();
		buffer.str({});
	}
}
