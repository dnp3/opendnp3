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
ECHO OSSL_LIB32_DIR is %OSSL_LIB32_DIR%

for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set MSBUILD_PATH=%%i
)

if not exist "%MSBUILD_PATH%\MSBuild\15.0\Bin\MSBuild.exe" (
  ECHO VS2017 could not be found
  GOTO :QUIT
)

IF EXIST build RMDIR build /s /q
MKDIR build\lib
CD build

SET OPENDNP3_DIR=%CD%\lib
ECHO OPENDNP3_DIR is %OPENDNP3_DIR%

cmake .. -DCMAKE_INSTALL_PREFIX=lib -DDNP3_TEST=ON -DDNP3_TLS=ON -DCMAKE_BUILD_TYPE=%CONFIGURATION% -G"Visual Studio 15 2017"
"%MSBUILD_PATH%\MSBuild\15.0\Bin\MSBuild.exe" opendnp3.sln /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%
"%MSBUILD_PATH%\MSBuild\15.0\Bin\MSBuild.exe" INSTALL.vcxproj /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%
"%MSBUILD_PATH%\MSBuild\15.0\Bin\MSBuild.exe" ..\dotnet\bindings.sln /target:Clean
"%MSBUILD_PATH%\MSBuild\15.0\Bin\MSBuild.exe" ..\dotnet\bindings.sln /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%

:QUIT
ENDLOCAL