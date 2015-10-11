
setx OPENDNP3_DIR build\lib
setx OSSL_LIB32_DIR %~1

RMDIR build /s /q
MKDIR build
CD build
MKDIR lib
cmake ../ -DCMAKE_INSTALL_PREFIX=lib -DDNP3_TLS=ON -DSECAUTH=ON -DCMAKE_BUILD_TYPE=Release
msbuild opendnp3.sln
msbuild INSTALL.vcxproj
cd ..
msbuild dotnet\bindings.sln /p:Configuration=Release

