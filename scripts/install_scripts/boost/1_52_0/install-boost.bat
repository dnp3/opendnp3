cd temp\boost_1_52_0

call ./bootstrap.bat

bjam.exe toolset=msvc link=static,shared --with-program_options --with-system --with-date_time --with-test stage

set libdir=%TOOLS_HOME%\boostlib\boost_1_52_0\windows

echo Installing libs to %libdir%
md %libdir%
copy stage\lib\* %libdir%

pause
