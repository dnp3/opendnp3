@ECHO OFF

SETLOCAL

SET CONFIGURATION=Release
IF NOT [%1]==[] (SET CONFIGURATION=%1)
SET PLATFORM=Win32
IF NOT [%2]==[] (SET PLATFORM=%2)

where cmake 1>nul
IF ERRORLEVEL 1 (
	ECHO cmake could not be found
	GOTO :QUIT
)
IF ["%OSSL_LIB32_DIR%"]==[] (
	ECHO OpenSSL could not be found
	GOTO :QUIT
)
IF ["%VS140COMNTOOLS%"]==[] (
	ECHO VS2015 could not be found
	GOTO: QUIT
)

:BUILD
CALL "%VS140COMNTOOLS%VsDevCmd.bat"

IF EXIST build RMDIR build /s /q
MKDIR build\lib
CD build

cmake .. -DCMAKE_INSTALL_PREFIX=lib -DDNP3_TEST=ON -DDNP3_TLS=ON -DCMAKE_BUILD_TYPE=%CONFIGURATION% -G"Visual Studio 14 2015"
msbuild opendnp3.sln /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%
msbuild INSTALL.vcxproj /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%
msbuild ..\dotnet\bindings.sln /target:Clean
msbuild ..\dotnet\bindings.sln /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%

:QUIT
ENDLOCAL