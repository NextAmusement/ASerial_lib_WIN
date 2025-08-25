@echo off

set CompileFileName=.\test\test.cpp
set exeFileName=test

echo ===    CompileStart    === %TIME%
g++ %CompileFileName% .\WindowsSerial\WindowsSerial.cpp .\ASerial_lib_Controller_Win.cpp .\ASerialCore\ASerial_packet.cpp -o %exeFileName%
echo ===    CompileEND      === %TIME%

echo ===    Run exe     ===
.\%exeFileName%.exe
echo ===    End Run exe     ===