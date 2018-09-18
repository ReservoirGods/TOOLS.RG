@echo off
echo doing all buildall
pushd ..\..

pushd GODLIB.SPL
call buildall.bat %*
echo in godlibspl
dir
popd
echo after popd1
dir
pushd TOOLS.RG
echo in tools.rg
dir
call buildall %*
popd

pushd dev.rg
dir
call buildall %*
popd

popd