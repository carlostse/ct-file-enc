#!/bin/sh
BUILD=$1

if [ "$BUILD" != "debug" ] && [ "$BUILD" != "release" ]; then
  echo "usage ./post_build_osx.sh [debug | release]"
  exit 1
fi

DYLIBCRYPTO=libcrypto.1.0.0.dylib
DYLIBSSL=libssl.1.0.0.dylib

QT_VER_MAJOR=5
QT_VER=$QT_VER_MAJOR.4.2

EXE=./$BUILD/ct_file_enc.app/Contents/MacOS/ct_file_enc

LIBCRYPTO=/usr/local/opt/openssl/lib/$DYLIBCRYPTO
LIBSSL=/usr/local/opt/openssl/lib/$DYLIBSSL

QTCORE_FW=/usr/local/opt/qt-$QT_VER/lib/QtCore.framework
QTGUI_FW=/usr/local/opt/qt-$QT_VER/lib/QtGui.framework
QTWIDGETS_FW=/usr/local/opt/qt-$QT_VER/lib/QtWidgets.framework

QTCORE=$QTCORE_FW/Versions/$QT_VER_MAJOR/QtCore
QTGUI=$QTGUI_FW/Versions/$QT_VER_MAJOR/QtGui
QTWIDGETS=$QTWIDGETS_FW/Versions/$QT_VER_MAJOR/QtWidgets

echo "preparing $BUILD..."

mkdir ./$BUILD/ct_file_enc.app/Contents/Frameworks

cp $LIBCRYPTO ./$BUILD/ct_file_enc.app/Contents/Frameworks/
cp $LIBSSL ./$BUILD/ct_file_enc.app/Contents/Frameworks/

install_name_tool -change $LIBCRYPTO @executable_path/../Frameworks/$DYLIBCRYPTO $EXE
install_name_tool -change $LIBSSL @executable_path/../Frameworks/$DYLIBSSL $EXE

cp -R $QTCORE_FW ./$BUILD/ct_file_enc.app/Contents/Frameworks/
cp -R $QTGUI_FW ./$BUILD/ct_file_enc.app/Contents/Frameworks/
cp -R $QTWIDGETS_FW ./$BUILD/ct_file_enc.app/Contents/Frameworks/

install_name_tool -change $QTCORE @executable_path/../Frameworks/QtCore.framework/Versions/$QT_VER_MAJOR/QtCore $EXE
install_name_tool -change $QTGUI @executable_path/../Frameworks/QtGui.framework/Versions/$QT_VER_MAJOR/QtGui $EXE
install_name_tool -change $QTWIDGETS @executable_path/../Frameworks/QtWidgets.framework/Versions/$QT_VER_MAJOR/QtWidgets $EXE

otool -L $EXE

if [ "$2" == "package" ]; then
  echo "packaging..."
  hdiutil create -size 100MB -fs HFS+ -volname "CT File Encrypt" ./release/tmp.dmg
  hdiutil attach ./release/tmp.dmg
  cp -R ./release/ct_file_enc.app /Volumes/CT\ File\ Encrypt/CT\ File\ Encrypt.app
  ln -s /Applications /Volumes/CT\ File\ Encrypt/
  diskutil eject /Volumes/CT\ File\ Encrypt/
  hdiutil convert -format UDBZ -o ./release/ct-file-encrypt.dmg ./release/tmp.dmg
  rm ./release/tmp.dmg
  rm -rf release/ct_file_enc.app
fi

echo "done."
exit 0