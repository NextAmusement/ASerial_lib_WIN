@echo off

set CompileFileName=.\test\test3.cpp
set exeFileName=test3

echo ===    CompileStart    === %TIME%
g++ %CompileFileName% .\WindowsSerial\WindowsSerial.cpp .\ASerial_lib_Controller_Win.cpp .\ASerial_lib_Controller_Win_MT.cpp .\ASerialCore\ASerial_packet.cpp -pthread -o %exeFileName%
echo ===    CompileEND      === %TIME%

echo ===    Run exe     ===
.\%exeFileName%.exe
echo ===    End Run exe     ===