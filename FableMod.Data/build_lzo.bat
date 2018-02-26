@ECHO OFF
SET VC_ARG=%1
SET LZO_PLATFORM=%2
IF "%VC_ARG%"=="" SET VC_ARG=x64
IF "%LZO_PLATFORM%"=="" SET LZO_PLATFORM=win64
ECHO Building LZO for %1...
CD ..
CD libs\lzo
REM CALL b\clean.bat
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %VC_ARG%
REM CALL "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" %1
CALL b\%LZO_PLATFORM%\vc.bat
IF "%3"=="" PAUSE
