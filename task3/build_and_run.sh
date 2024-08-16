#!/bin/bash

# Компиляция исходных файлов в объектные файлы
g++ -c MuteConverter.cpp -o MuteConverter.o
g++ -c MixConverter.cpp -o MixConverter.o

g++ -c ConfigParser.cpp -o ConfigParser.o
g++ -c SoundProcessor.cpp -o SoundProcessor.o

g++ -c WAVFileReader.cpp -o WAVFileReader.o
g++ -c WAVFileWriter.cpp -o WAVFileWriter.o

g++ -c main.cpp -o main.o

# Компоновка объектных файлов в исполняемый файл
g++ main.o ConfigParser.o MuteConverter.o MixConverter.o SoundProcessor.o WAVFileReader.o WAVFileWriter.o -o audio_processor

# Запуск программы с указанием файлов конфигурации, входных и выходного файлов
./audio_processor testfiles/config.txt testfiles/output.wav testfiles/input3.wav testfiles/input4.wav
