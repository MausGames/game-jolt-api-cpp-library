g++ -O2 -s -std=c++0x -Wall ../source/*.cpp main.cpp -I../source -I../libraries/windows/include -L../libraries/windows -llibcurl -o mingw_test.exe
PAUSE