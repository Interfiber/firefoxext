SRC_FOLDER=src
MANIFEST_SOURCE=$(SRC_FOLDER)/firefoxext.c
BUILD_ARGS=$(shell pkg-config --cflags --libs libcurl) -lcjson
OUTPUT_NAME=firefoxext
OUTPUT_FOLDER=bin
INSTALL_DIR=/usr/bin

build:
	mkdir -p $(OUTPUT_FOLDER)
	gcc -Wall $(MANIFEST_SOURCE) $(BUILD_ARGS) -o $(OUTPUT_FOLDER)/$(OUTPUT_NAME)
install:
	sudo cp bin/firefoxext /usr/bin/firefoxext
