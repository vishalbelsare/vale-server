.PHONY: build

all: build

build:
	rm -rf *.AppImage
	cp '/home/jdkato/Desktop/Git/build-systray-Desktop_Qt_5_15_2_GCC_64bit-Release/Vale Server' vale-server/usr/bin/vale-server
	linuxdeployqt vale-server/usr/share/applications/vale-server.desktop -appimage -no-translations -bundle-non-qt-libs -extra-plugins=imageformats,iconengines,platformthemes/libqgtk3.so -always-overwrite -qmake=/home/jdkato/Qt/5.15.2/gcc_64/bin/qmake