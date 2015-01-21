@ECHO OFF
REM
REM

SETLOCAL

PUSHD %~dp0

:tar
IF EXIST "Project_SkyFire.tgz" DEL "Project_SkyFire.tgz"
tar --version 1>&2 2>NUL || (ECHO. & ECHO ERROR: tar not found. try to use seven zip. & GOTO SevenZip)
tar czvf Project_SkyFire.tgz cov-int
GOTO End

:SevenZip
IF NOT EXIST "C:\Program Files (x86)\7-Zip\7z.exe" (ECHO. & ECHO ERROR: "C:\Program Files (x86)\7-Zip\7z.exe" not found & GOTO End)
IF EXIST "Project_SkyFire.tar" DEL "Project_SkyFire.tar"
IF EXIST "Project_SkyFire.tgz" DEL "Project_SkyFire.tgz"
"C:\Program Files (x86)\7-Zip\7z.exe" a -ttar Project_SkyFire.tar cov-int
"C:\Program Files (x86)\7-Zip\7z.exe" a -tgzip Project_SkyFire.tgz Project_SkyFire.tar
IF EXIST "Project_SkyFire.tar" DEL "Project_SkyFire.tar"
GOTO End

:End
POPD
ECHO. & ECHO Press any key to close this window...
PAUSE >NUL
ENDLOCAL
EXIT /B