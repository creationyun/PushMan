#!/bin/sh

g++ -c -g *.cpp
g++ *.o -o PushMan -lglut -lGLU -lGL -lGLEW -g
