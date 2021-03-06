## GraphViz1C - внешняя компонента 1С для построения графов

Предназначена для Windows, разработана по технологии Native API.

[![Build status](https://ci.appveyor.com/api/projects/status/ydd0mry1m8848wp1?svg=true)](https://ci.appveyor.com/project/lintest/graphviz1c)

Поддерживаемые форматы экспорта: bmp, canon, cmap, cmapx, cmapx_np, dot, dot_json, emf,
emfplus, eps, fig, gif, gv, imap, imap_np, ismap, jpe, jpeg, jpg, json, json0, metafile, mp, pic,
plain, plain-ext, png, ps, ps2, svg, tif, tiff, tk, vml, xdot, xdot1.2, xdot1.4, xdot_json.

Для двоичных данных DOT-файлов реализована поддержка только кодировки UTF-8. Если исходный файл
в другой кодировке, рекомендуется прочитать файл в строку посредством объекта ЧтениеТекста.

### Подключение внешней компоненты

```bsl
&НаКлиенте
Перем ИдентификаторКомпоненты, ВнешняяКомпонента;

&НаСервере
Процедура ПриСозданииНаСервере(Отказ, СтандартнаяОбработка)
	ОбработкаОбъект = РеквизитФормыВЗначение("Объект");
	МакетКомпоненты = ОбработкаОбъект.ПолучитьМакет("GraphViz1C");
	АдресКомпоненты = ПоместитьВоВременноеХранилище(МакетКомпоненты, УникальныйИдентификатор);
КонецПроцедуры

&НаКлиенте
Процедура ПриОткрытии(Отказ)
	ИдентификаторКомпоненты = "_" + СтрЗаменить(Новый УникальныйИдентификатор, "-", "");
	ВыполнитьПодключениеВнешнейКомпоненты(Истина);
КонецПроцедуры

&НаКлиенте
Процедура ВыполнитьПодключениеВнешнейКомпоненты(ДопПараметры) Экспорт
	НачатьПодключениеВнешнейКомпоненты(
		Новый ОписаниеОповещения("ПодключенаВнешняяКомпонента", ЭтаФорма, ДопПараметры), 
		АдресКомпоненты, ИдентификаторКомпоненты, ТипВнешнейКомпоненты.Native);
КонецПроцедуры

&НаКлиенте
Процедура ПодключенаВнешняяКомпонента(Подключение, ДополнительныеПараметры) Экспорт
	Если Подключение Тогда
		ВнешняяКомпонента = Новый("AddIn." + ИдентификаторКомпоненты + ".GraphViz1C");
	ИначеЕсли ДополнительныеПараметры = Истина Тогда
		НачатьУстановкуВнешнейКомпоненты(
			Новый ОписаниеОповещения("ВыполнитьПодключениеВнешнейКомпоненты", ЭтаФорма, Ложь), 
			АдресКомпоненты);
	КонецЕсли;
КонецПроцедуры
```

## Методы
### Сформировать(Данные, ФорматКартинки, ВариантРазмещения) / Render
Возвращает картинку в виде двоичных данных, либо строку с текстом ошибки.

Параметры функции:
- **Данные** (обязательный), Тип: Двоичные данные или строка
- **ФорматКартинки** (необязательный), Тип: Строка, по умолчанию "svg"
- **ВариантРазмещения** (необязательный), Тип: Строка, по умолчанию "dot"

Тип возвращаемого значения: двоичные данные или строка
- Содержит картинку в виде двоичных данных, либо строку с текстом ошибки.

```bsl
&НаКлиенте
Процедура СформироватьКартинку(Команда)
	ДвоичныеДанные = Новый ДвоичныеДанные(ИмяФайла);
	ОписаниеОповещения = Новый ОписаниеОповещения("ПолученаКартинка", ЭтаФорма);
	ВнешняяКомпонента.НачатьВызовСформировать(ОписаниеОповещения, ДвоичныеДанные, "svg");
КонецПроцедуры

&НаКлиенте
Процедура ПолученаКартинка(РезультатВызова, ПараметрыВызова, ДополнительныеПараметры) Экспорт
	Если ТипЗнч(РезультатВызова) = Тип("ДвоичныеДанные") Тогда
		АдресКартинки = ПоместитьВоВременноеХранилище(РезультатВызова, УникальныйИдентификатор);
	ИначеЕсли ТипЗнч(РезультатВызова) = Тип("Строка") Тогда
		Сообщить(РезультатВызова);
	КонецЕсли;
КонецПроцедуры
```

### Формат(Идентификатор) / Format
Возвращает список доступных форматов по идентификатору API.

Параметры функции:
- **Идентификатор** (необязательный), Тип: Целое число, по умолчанию: 3
	* API_render = 0
	* API_layout = 1
	* API_textlayout = 2
	* API_device = 3
	* API_loadimage = 4

Тип возвращаемого значения: строка
- Содержит строку со списком идентификаторов доступных форматов, разделенных пробелами.

```bsl
&НаКлиенте
Процедура ВывестиДоступныеФорматы(Команда)
	ОписаниеОповещения = Новый ОписаниеОповещения("ПолученыФорматы", ЭтаФорма);
	ВнешняяКомпонента.НачатьВызовФормат(ОписаниеОповещения);
КонецПроцедуры

&НаКлиенте
Процедура ПолученыФорматы(РезультатВызова, ПараметрыВызова, ДополнительныеПараметры) Экспорт
	Сообщить("Доступные форматы: " + РезультатВызова);
КонецПроцедуры
```
## Свойства

### ПутиКартинок / ImagePath
Тип значения: Строка (только чтение)
- Список папок, где осуществляется поиск используемых в графе картинок.
Соответствует переменной окружения [GV_FILE_PATH](https://graphviz.org/doc/info/command.html#d:GV_FILE_PATH).
Для среды Windows значения разделяются точкой с запятой, для Linux разделяются двоеточием.
