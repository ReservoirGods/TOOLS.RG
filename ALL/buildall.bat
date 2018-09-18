@echo off

pushd ..\..

pushd GODLIB.SPL
call buildall.bat %*
popd

pushd TOOLS.RG
echo in tools.rg
call buildall %*
popd

pushd dev.rg
call buildall %*
popd

popd
