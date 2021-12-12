@echo off
@echo Cleaning bcd directories
@echo .
rmidr /q /s .vs
rmdir /q /s Debug
rmdir /q /s Release
rmdir /q /s x64
rmdir /q /s UnitTest\Debug
rmdir /q /s UnitTest\Release
rmdir /q /s UnitTest\x64

echo .
echo Ready cleaning bcd
