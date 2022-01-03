SOURCE_FILE=src/manifest.c
BUILD_OPTIONS_CURL=$(shell curl-config --cflags --libs)
BUILD_OPTIONS=-Wall
build:
	mkdir -p bin
	gcc $(BUILD_OPTIONS) $(BUILD_OPTIONS_CURL) $(SOURCE_FILE) -o bin/firefoxext