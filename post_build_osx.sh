#!/bin/sh
#
# @deprecated - use ./deploy_osx.sh
#

BUILD=$1

if [ "$BUILD" != "debug" ] && [ "$BUILD" != "release" ]; then
  echo "usage ./post_build_osx.sh [debug | release]"
  exit 1
fi

DYLIBCRYPTO=libcrypto.1.0.0.dylib
DYLIBSSL=libssl.1.0.0.dylib

QT_VER_MAJOR=5
QT_VER=$QT_VER_MAJOR.4.2

APP=./$BUILD/ct_file_enc.app
EXE=$APP/Contents/MacOS/ct_file_enc

LIBCRYPTO=/usr/local/opt/openssl/lib/$DYLIBCRYPTO
LIBSSL=/usr/local/opt/openssl/lib/$DYLIBSSL

QTDIR=/usr/local/opt/qt-$QT_VER
QTCORE_FW=$QTDIR/lib/QtCore.framework
QTGUI_FW=$QTDIR/lib/QtGui.framework
QTWIDGETS_FW=$QTDIR/lib/QtWidgets.framework

QTCORE=$QTCORE_FW/Versions/$QT_VER_MAJOR/QtCore
QTGUI=$QTGUI_FW/Versions/$QT_VER_MAJOR/QtGui
QTWIDGETS=$QTWIDGETS_FW/Versions/$QT_VER_MAJOR/QtWidgets

echo "preparing $BUILD..."

PLATFORMS=$APP/Contents/plugins/platforms/
mkdir -p $PLATFORMS
cp $QTDIR/plugins/platforms/libqcocoa.dylib $PLATFORMS

FRAMEWORK=$APP/Contents/Frameworks/
mkdir $FRAMEWORK
cp $LIBCRYPTO $FRAMEWORK
cp $LIBSSL $FRAMEWORK

install_name_tool -change $LIBCRYPTO @executable_path/../Frameworks/$DYLIBCRYPTO $EXE
install_name_tool -change $LIBSSL @executable_path/../Frameworks/$DYLIBSSL $EXE

cp -R $QTCORE_FW $FRAMEWORK
cp -R $QTGUI_FW $FRAMEWORK
cp -R $QTWIDGETS_FW $FRAMEWORK

install_name_tool -change $QTCORE @executable_path/../Frameworks/QtCore.framework/Versions/$QT_VER_MAJOR/QtCore $EXE
install_name_tool -change $QTGUI @executable_path/../Frameworks/QtGui.framework/Versions/$QT_VER_MAJOR/QtGui $EXE
install_name_tool -change $QTWIDGETS @executable_path/../Frameworks/QtWidgets.framework/Versions/$QT_VER_MAJOR/QtWidgets $EXE

install_name_tool -change $QTCORE @executable_path/../Frameworks/QtCore.framework/Versions/$QT_VER_MAJOR/QtCore $FRAMEWORK/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -change $QTGUI @executable_path/../Frameworks/QtGui.framework/Versions/$QT_VER_MAJOR/QtGui $FRAMEWORK/QtWidgets.framework/Versions/5/QtWidgets

install_name_tool -change $QTCORE @executable_path/../Frameworks/QtCore.framework/Versions/$QT_VER_MAJOR/QtCore $FRAMEWORK/QtGui.framework/Versions/5/QtGui

otool -L $EXE
otool -L $FRAMEWORK/QtWidgets.framework/Versions/5/QtWidgets
otool -L $FRAMEWORK/QtGui.framework/Versions/5/QtGui

if [ "$2" == "package" ]; then
  echo "packaging..."
  hdiutil create -size 100MB -fs HFS+ -volname "CT File Encrypt" ./$BUILD/tmp.dmg
  hdiutil attach ./$BUILD/tmp.dmg
  cp -R ./$BUILD/ct_file_enc.app /Volumes/CT\ File\ Encrypt/CT\ File\ Encrypt.app
  ln -s /Applications /Volumes/CT\ File\ Encrypt/
  diskutil eject /Volumes/CT\ File\ Encrypt/
  hdiutil convert -format UDBZ -o ./$BUILD/ct-file-encrypt.dmg ./$BUILD/tmp.dmg
  rm ./$BUILD/tmp.dmg
  rm -rf ./$BUILD/ct_file_enc.app
fi

echo "done."
exit 0