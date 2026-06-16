@echo off

gcc sgl-simulator.c sgl\*.c ^
    -Isdl2\include ^
    -Lsdl2\lib ^
    -lmingw32 ^
    -lSDL2main ^
    -lSDL2 ^
    -mwindows ^
    -o sgl-simulator.exe

if errorlevel 1 (
    pause
    exit /b %errorlevel%
)

start "" sgl-simulator.exe
