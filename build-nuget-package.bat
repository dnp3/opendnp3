setx OPENDNP3_DIR %~dp0\build\lib

IF EXIST build RMDIR build /s /q
MKDIR build
CD build
MKDIR lib
cmake .. -DCMAKE_INSTALL_PREFIX=lib -DDNP3_TLS=ON -DSECAUTH=ON -DCMAKE_BUILD_TYPE=Release -G"Visual Studio 12 2013"
msbuild opendnp3.sln /p:Configuration=Release /p:Platform=Win32
msbuild INSTALL.vcxproj /p:Configuration=Release /p:Platform=Win32
cd ..
msbuild dotnet\bindings.sln /p:Configuration=Release /p:Platform=Win32
nuget pack dotnet\nuget\opendnp3.nuspec -Version %1

