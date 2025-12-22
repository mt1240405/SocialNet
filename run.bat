@echo off

if exist main.exe del main.exe
g++ -std=c++17 -O2 -Wall *.cpp -o main
echo ....Running SocialNet, use commands in README....
echo ....Use Ctrl + Z to quit....
main.exe
