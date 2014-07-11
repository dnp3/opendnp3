@echo off
REM 	This batch files takes the .lib and .h files from the opendnp3 repo and copies them to a local folder
REM   It's very useful to update the local library repository for creating custom applications using opendnp3.
 
REM   Remember to set the environment variable OPENDNP3_DIR to a local lib folder. If undefined, this copies
REM   the result in a folder in the current location. 

REM	Autotrol, July 2014
REM   Version 1.0
REM 	Pablo Giudice

set LIB_NAME=opendnp3-lib
set INC_NAME=opendnp3-include

REM 	does this folder exist? if it doesn't, just store the results in the current folder.
if %OPENDNP3_DIR% == "" (
	echo warning: there is no %OPENDNP3_DIR% environment variable. The result will be saved in the current location on folders: %LIB_NAME% and %INC_NAME%
	set OPENDNP3_DIR=%cd%
)

REM 	First search for all the .lib files and copy them to LIB_TARGET_LOCATION
set LIB_TARGET_LOCATION=%OPENDNP3_DIR%\%LIB_NAME%\
echo .lib files will be saved in: %LIB_TARGET_LOCATION%
for /r %%x in (*.lib) do xcopy "%%x" %LIB_TARGET_LOCATION% /Y

REM 	Then search for all the .h files and copy to INC_TARGET_LOCATION
set INC_TARGET_LOCATION=%OPENDNP3_DIR%\%INC_NAME%\
echo .h files will be saved in: %INC_TARGET_LOCATION%
REM 	Copy ALL files ...
xcopy cpp\libs\asiodnp3\src\*.h %INC_TARGET_LOCATION% /SY
xcopy cpp\libs\asiopal\src\*.h 	%INC_TARGET_LOCATION% /SY
xcopy cpp\libs\opendnp3\src\*.h %INC_TARGET_LOCATION% /SY
xcopy cpp\libs\openpal\src\*.h	%INC_TARGET_LOCATION% /SY

echo ---------------------------------------------------
echo Installation finished succesffully at %date% %time%
echo ---------------------------------------------------