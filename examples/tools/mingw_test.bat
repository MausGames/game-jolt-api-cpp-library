g++ -O2 -s -std=c++11 -Wall ../../source/*.cpp ../main.cpp -I../../source -I../../libraries/windows/include -L../../libraries/windows -lLibcURL -o mingw_test.exe
PAUSE