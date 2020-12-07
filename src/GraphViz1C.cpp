#include "GraphViz1C.h"

std::vector<std::u16string> GraphViz1C::names = {
	AddComponent(u"AddInNative", []() { return new GraphViz1C; }),
	AddComponent(u"SimpleAlias", []() { return new GraphViz1C; }),
};

GraphViz1C::GraphViz1C()
{
	AddProperty(
		u"Version", u"Версия",
		[&](VH var) { var = this->version(); });

	AddProperty(
		u"Text", u"Текст",
		[&](VH var) { var = this->getTestString(); },
		[&](VH var) { this->setTestString(var); });

	AddProperty(
		u"Number", u"Число",
		[&](VH var) { var = this->value; },
		[&](VH var) { this->value = var; });

	AddFunction(u"GetText", u"ПолучитьТекст", [&]() { this->result = this->getTestString(); });

	AddProcedure(u"SetText", u"УстановитьТекст", [&](VH par) { this->setTestString(par); }, {{0, u"default: "}});
}

#include <iostream>
#include <ctime>

std::u16string GraphViz1C::getTestString()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[255];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	return text + MB2WCHAR(buffer);
}

void GraphViz1C::setTestString(const std::u16string &text)
{
	this->text = text;
}
