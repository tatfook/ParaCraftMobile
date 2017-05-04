pushd %~dp0\..\
cls
REM rm all asset file since some may be readonly
rd "frameworks\runtime-src\proj.android\assets" /s /q

REM keytool.exe is under D:\Program Files (x86)\Java\jdk1.8.0_11\bin (needs to add to path). Run following
REM keytool -genkey -v -keystore paracraft-release-key.keystore -alias paracraft -keyalg RSA -keysize 2048 -validity 100000
REM when prompted: use D:\lxzsrc\ParaEngineGit\Mobile\trunk\ParaCraftMobile\build\paracraft-release-key.keystore
REM all passwords are: paracraft

REM cocos compile -p android -m release --ndk-mode NDK_DEBUG=1
cocos compile -p android -m release
popd