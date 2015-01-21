@ECHO OFF
REM
REM

SETLOCAL

PUSHD %~dp0

SET COVDIR=C:\cov-analysis-win32-7.6.0

CALL "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" x86

SET MSBUILD_SWITCHES=/nologo

"%COVDIR%\bin\cov-build.exe" --dir cov-int MSBuild "Project_SkyFire.sln" %MSBUILD_SWITCHES%

GOTO End

:End
POPD
ECHO. & ECHO Press any key to close this window...
PAUSE >NUL
ENDLOCAL
EXIT /B