#!/bin/sh

BUILD=$1

if [ "$BUILD" != "debug" ] && [ "$BUILD" != "release" ]; then
  echo "usage ./deploy_osx.sh [debug | release]"
  exit 1
fi

QT_VER=5.4.2
QTDIR=/usr/local/opt/qt-$QT_VER
APP_NAME=ct_file_enc
cd $BUILD

# note -dmg will create a read-only image
$QTDIR/bin/macdeployqt $APP_NAME.app

hdiutil create -size 100MB -fs HFS+ -volname "CT File Encrypt" ./tmp.dmg
hdiutil attach ./tmp.dmg
cp -R ./$APP_NAME.app /Volumes/CT\ File\ Encrypt/CT\ File\ Encrypt.app
ln -s /Applications /Volumes/CT\ File\ Encrypt/
diskutil eject /Volumes/CT\ File\ Encrypt/
hdiutil convert -format UDBZ -o ./$APP_NAME.dmg ./tmp.dmg
rm ./tmp.dmg
echo "done"