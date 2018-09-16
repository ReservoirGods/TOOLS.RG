@echo off
pushd
for /D %%i in (*) do (
	if exist "%%i\FAST" (
		pushd "%%i\FAST"
		call build.bat %*
		popd
	)
)
popd