COMPILER_ARGS=-std=c99 -Wall -lcjson
LIBCURL_COMPILER_ARGS=$(shell curl-config --libs --cflags)
SRC_FOLDER=src
MANIFEST_SOURCE=$(SRC_FOLDER)/firefoxext.c
OUTPUT_NAME=firefoxext
OUTPUT_FOLDER=bin
INSTALL_DIR=/usr/bin

build:
	mkdir -p $(OUTPUT_FOLDER)
	gcc $(COMPILER_ARGS) $(LIBCURL_COMPILER_ARGS) $(MANIFEST_SOURCE) -o $(OUTPUT_FOLDER)/$(OUTPUT_NAME)
install:
	sudo cp bin/firefoxext /usr/bin/firefoxext
