rem cmake -E remove_directory build32W
rem cmake -E remove_directory build64W
rem del bin\Release\GraphViz1cWin32.dll
rem del bin\Release\GraphViz1cWin64.dll

mkdir build32W
cd build32W
cmake .. -A Win32 -DMySuffix2=32 -DVERSION="2.44.2" -DDATE="2020-12-07"
cmake --build . --config Release --target GraphViz1c
cd ..

mkdir build64W
cd build64W
cmake .. -A x64 -DMySuffix2=64 -DVERSION="2.44.2" -DDATE="2020-12-07"
cmake --build . --config Release --target GraphViz1c
cd ..

oscript .\tools\ZipLibrary.os GraphViz1c
mkdir .\Example\Templates\GraphViz1C
mkdir .\Example\Templates\GraphViz1C\Ext
copy /b .\AddIn.zip .\Example\Templates\GraphViz1C\Ext\Template.bin
oscript .\tools\Compile.os .\