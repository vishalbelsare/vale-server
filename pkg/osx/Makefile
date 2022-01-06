all: build

build:
	xattr -cr "Vale Server.app"

	codesign --force --verbose --sign "${CODE_SIGN_ID}" --timestamp --deep --options runtime "${QT5}/lib/Sparkle.framework/Versions/A/Resources/Autoupdate.app"

	${QT5}/bin/macdeployqt "Vale Server.app" -codesign=${CODE_SIGN_ID} -verbose=3

	rm -rf "Vale Server.app/Contents/Frameworks/QtQml.framework"
	rm -rf "Vale Server.app/Contents/Frameworks/QtQuick.framework"
	rm -rf "Vale Server.app/Contents/Frameworks/QtVirtualKeyboard.framework"
	rm -rf "Vale Server.app/Contents/Frameworks/QtQmlModels.framework"
	rm -rf "Vale Server.app/Contents/Frameworks/QtSvg.framework"
	rm -rf "Vale Server.app/Contents/Frameworks/QtOpenGL.framework"
	rm -rf "Vale Server.app/Contents/Plugins/virtualkeyboard"
	rm -rf "Vale Server.app/Contents/Plugins/imageformats"

	strip "Vale Server.app/Contents/MacOS/Vale Server"

	# Binaries
	codesign --force --verify --verbose -s "${CODE_SIGN_ID}" --options "runtime" --entitlements entitlements.plist "Vale Server.app/Contents/MacOS/valelib"
	codesign --force --verify --verbose -s "${CODE_SIGN_ID}" --options "runtime" "Vale Server.app/Contents/MacOS/Vale Server"

	codesign --verify -vvvv "Vale Server.app"
	spctl -a -vvvv "Vale Server.app"

	create-dmg --overwrite "Vale Server.app"

	xcrun altool --notarize-app -t osx -f "$(shell find *.dmg)" --primary-bundle-id="ai.errata.Vale-Server" -u "${MAC_DEV_E}" -p "${MAC_DEV_P}"
