cmake -E remove_directory build32W
cmake -E remove_directory build64W
del bin\Release\GraphViz1cWin32.dll
del bin\Release\GraphViz1cWin64.dll

mkdir build32W
cd build32W
cmake .. -A Win32 -DMySuffix2=32
cmake --build . --config Release
cd ..

mkdir build64W
cd build64W
cmake .. -A x64 -DMySuffix2=64
cmake --build . --config Release
cd ..

copy /b .\AddIn.zip .\Example\Templates\GraphViz1c\Ext\Template.bin
oscript .\tools\Compile.os .\