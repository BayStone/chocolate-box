@ECHO OFF
ECHO Cleaning LZO...
CD ..
CD libs\lzo
CALL b\clean.bat
IF "%1"=="" PAUSE