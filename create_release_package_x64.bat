@ECHO OFF
SET SOURCE=%CD%\ChocolateBox\bin\x64\Release
CD ReleasePackage
RMDIR /S /Q Package
MKDIR Package
CD Package
COPY %SOURCE%\*.dll .
COPY %SOURCE%\ChocolateBox.exe .
COPY %SOURCE%\*.ini .
COPY %SOURCE%\*.xml .
COPY %SOURCE%\*.vsh .
MKDIR data
XCOPY /S %SOURCE%\data .\data
DEL data\uids.txt
CD ..
COPY *.ini .\Package
ECHO .
ECHO NOTE! The releasable package can now be found in the ReleasePackage\Package folder. Just zip it and upload it.
PAUSE