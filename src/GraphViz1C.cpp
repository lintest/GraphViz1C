#include "GraphViz1C.h"
#include <common/types.h>
#include <cgraph/cgraph.h>
#include <gvc/gvc.h>
#include <gvc/gvio.h>
#include <gvc/gvplugin.h>

extern "C" {
	extern char* gvplugin_list(GVC_t* gvc, api_t api, const char* str);
	extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
	extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
	extern gvplugin_library_t gvplugin_core_LTX_library;
	extern gvplugin_library_t gvplugin_gdiplus_LTX_library;
	extern char* Gvfilepath;  /* Per-process path of files allowed in image attributes (also ps libs) */
	extern char* Gvimagepath; /* Per-graph path of files allowed in image attributes  (also ps libs) */
}

namespace AddIn1C {

	static int error(char* errMsg)
	{
		GraphViz1C::errors << errMsg;
		return 0;
	}

	lt_symlist_t lt_preloaded_symbols[] = {
		{ "gvplugin_dot_layout_LTX_library", (void*)(&gvplugin_dot_layout_LTX_library) },
		{ "gvplugin_neato_layout_LTX_library", (void*)(&gvplugin_neato_layout_LTX_library) },
		{ "gvplugin_core_LTX_library", (void*)(&gvplugin_core_LTX_library) },
		{ "gvplugin_gdiplus_LTX_library", (void*)(&gvplugin_gdiplus_LTX_library) },
		{ 0, 0 }
	};

	typedef struct {
		const char* data;
		int len;
		int cur;
	} rdr_t;

	static int memiofread(void* chan, char* buf, int bufsize)
	{
		if (bufsize == 0) return 0;
		rdr_t* s = (rdr_t*)chan;
		if (s->cur >= s->len)
			return 0;
		int l = 0;
		const char* ptr = s->data + s->cur;
		char* optr = buf;
		char c = *ptr++;
		do {
			*optr++ = c;
			l++;
		} while ((c != '\n') && (l < bufsize) && (c = *ptr++));
		s->cur += l;
		return l;
	}

	static Agraph_t* read(const char* data, unsigned int size)
	{
		rdr_t rdr{ data, size, 0 };
		Agiodisc_t memIoDisc = { memiofread, AgIoDisc.putstr, AgIoDisc.flush };
		Agdisc_t disc{ &AgMemDisc, &AgIdDisc, &memIoDisc };
		agsetfile(NULL);
		return agread(&rdr, &disc);
	}
}

#pragma comment(lib, "vcruntime.lib")

std::vector<std::u16string> GraphViz1C::names = {
	AddComponent(u"GraphViz1C", []() { return new GraphViz1C; }),
};

GraphViz1C::GraphViz1C()
{
	static std::string imagepath;

	AddProperty(
		u"ImagePath", u"ПутиКартинок",
		[&](VH var) { var = imagepath; },
		[&](VH var) {
			imagepath = var;
			Gvfilepath = (char*)imagepath.c_str();
			Gvimagepath = (char*)imagepath.c_str();
		}
	);
	AddProperty(
		u"Errors", u"Ошибки",
		[&](VH var) { var = buffer.str(); }
	);
	AddFunction(u"Render", u"Сформировать",
		[&](VH source, VH format, VH layout) { this->render(source, format, layout); },
		{ { 1, u"svg"}, { 2, u"dot"  } }
	);
	AddFunction(u"Format", u"Формат",
		[&](VH api) { this->formats(api); },
		{ { 0, (int64_t)3 } }
	);

	gvc = gvContextPlugins(AddIn1C::lt_preloaded_symbols, 0);
}

GraphViz1C::~GraphViz1C()
{
	if (gvc) gvFreeContext(gvc);
}

std::stringbuf GraphViz1C::buffer;

std::ostream GraphViz1C::errors(&buffer);

void GraphViz1C::render(VH source, const std::string& format, const std::string& layout)
{
	buffer.str({});
	agseterrf(AddIn1C::error);

	std::string text;
	const char* input = nullptr;
	char* output = nullptr;
	unsigned int size = 0;
	switch (source.type()) {
	case VTYPE_BLOB:
		input = source.data();
		size = source.size();
		break;
	case VTYPE_PWSTR:
		text = source;
		input = text.c_str();
		size = text.size();
		break;
	default:
		result = "Unsupported data type";
		return;
	}

	if (!gvc) {
		result = "Failed to initialize context plugins";
		return;
	}

	agmemread("\0");
	auto g = AddIn1C::read(input, size);
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
	if (result.type() == VTYPE_EMPTY) {
		result = buffer.str();
	}
}

void GraphViz1C::formats(int64_t api)
{
	if (gvc) {
		auto str = gvplugin_list(gvc, (api_t)api, "");
		if (str) result = std::string(str);
	}
}
