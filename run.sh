#!/bin/sh

gcc sgl-simulator.c sgl/*.c -o sgl-simulator $(sdl2-config --cflags --libs)
./sgl-simulator
