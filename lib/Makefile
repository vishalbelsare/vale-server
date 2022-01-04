LAST_TAG=$(shell git describe --abbrev=0 --tags)
CURR_SHA=$(shell git rev-parse --verify HEAD)

LDFLAGS=-ldflags "-s -w -X main.version=v0.1.0"
BUILD_DIR=./build

.PHONY: build test release

all: build

test:
	go test valelib/lib

# make release tag=v0.4.3
release:
	git tag $(tag)
	git push origin $(tag)

# make build os=darwin
# make build os=windows
# make build os=linux
build:
	GOOS=$(os) GOARCH=amd64 go build ${LDFLAGS} -o bin ./cmd/valelib

upx:
	GOOS=$(os) GOARCH=amd64 go build ${LDFLAGS} -o bin ./cmd/valelib
ifeq ($(os),windows)
	upx bin/valelib.exe
else
	upx bin/valelib
endif
