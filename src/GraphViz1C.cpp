#include "GraphViz1C.h"
#include <common/types.h>
#include <cgraph/cgraph.h>
#include <gvc/gvc.h>
#include <gvc/gvio.h>
#include <gvc/gvplugin.h>

#pragma comment(lib, "vcruntime.lib")

std::vector<std::u16string> GraphViz1C::names = {
	AddComponent(u"GraphViz1C", []() { return new GraphViz1C; }),
};

GraphViz1C::GraphViz1C()
{
	AddFunction(u"Render", u"Сформировать",
		[&](VH source, VH format, VH layout) { this->render(source, format, layout); },
		{ { 1, u"svg"}, { 2, u"dot"  } }
	);
	AddFunction(u"Format", u"Формат",
		[&](VH api) { this->formats(api); },
		{ { 0, (int64_t)3 } }
	);
}

std::stringbuf GraphViz1C::buffer;

std::ostream GraphViz1C::errors(&buffer);

static int error(char* errMsg)
{
	GraphViz1C::errors << errMsg;
	return 0;
}

void GraphViz1C::render(VH source, const std::string& format, const std::string& layout)
{
	buffer.str({});
	agseterrf(::error);

	std::string text;
	std::vector<char> data;
	const char* input = nullptr;
	char* output = nullptr;
	unsigned int size = 0;
	switch (source.type()) {
	case VTYPE_BLOB:
		data.resize((size_t)source.size() + 1, 0);
		memcpy(data.data(), source.data(), data.size());
		input = data.data();
		break;
	case VTYPE_PWSTR:
		text = source;
		input = text.c_str();
		break;
	default:
		result = "Unsupported data type";
		return;
	}

	auto gvc = gvContextPlugins(lt_preloaded_symbols, 0);
	if (gvc) {
		auto g = agmemread(input);
		if (g) {
			if (gvLayout(gvc, g, layout.c_str()) == 0) {
				if (gvRenderData(gvc, g, format.c_str(), &output, &size) == 0) {
					try {
						result.AllocMemory(size);
						memcpy(result.data(), output, size);
					}
					catch (...) {
						result = "Memory allocation error";
					}
					gvFreeRenderData(output);
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

extern "C" {
	extern char* gvplugin_list(GVC_t* gvc, api_t api, const char* str);
	extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
	extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
	extern gvplugin_library_t gvplugin_core_LTX_library;
	extern gvplugin_library_t gvplugin_gdiplus_LTX_library;
}

lt_symlist_t lt_preloaded_symbols[] = {
	{ "gvplugin_dot_layout_LTX_library", (void*)(&gvplugin_dot_layout_LTX_library) },
	{ "gvplugin_neato_layout_LTX_library", (void*)(&gvplugin_neato_layout_LTX_library) },
	{ "gvplugin_core_LTX_library", (void*)(&gvplugin_core_LTX_library) },
	{ "gvplugin_gdiplus_LTX_library", (void*)(&gvplugin_gdiplus_LTX_library) },
	{ 0, 0 }
};

void GraphViz1C::formats(int64_t api)
{
	auto gvc = gvContextPlugins(lt_preloaded_symbols, 0);
	if (gvc) {
		auto str = gvplugin_list(gvc, (api_t)api, "");
		if (str) result = std::string(str);
		gvFreeContext(gvc);
	}
}
