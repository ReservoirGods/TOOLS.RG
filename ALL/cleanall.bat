@echo off

pushd ..\..

pushd GODLIB.SPL
call cleanall.bat
popd

pushd TOOLS.RG
call cleanall.bat
popd

pushd dev.rg
call cleanall.bat
popd

popd
