call "C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"
cd temp\boost_1_43_0

call ./bootstrap.bat

bjam.exe toolset=msvc --with-program_options --with-system --with-date_time --with-thread --with-filesystem --with-test stage

set libdir=%TOOLS_HOME%\boostlib\boost_1_43\windows

echo Installing libs to %libdir%
md %libdir%
copy stage\lib\* %libdir%

pause
