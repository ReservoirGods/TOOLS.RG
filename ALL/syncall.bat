@echo off

pushd ..\..

pushd dev.rg
call sync.bat
popd
pushd GAMES.RG
call sync.bat
popd
pushd GODLIB
call sync.bat
popd
pushd GODLIB.SPL
call sync.bat
popd
pushd TOOLS.RG
call sync.bat
popd

popd