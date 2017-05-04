pushd %~dp0\..\
cls
REM rm all asset file since some may be readonly
rd "frameworks\runtime-src\proj.android\assets" /s /q
cocos run -p android
popd