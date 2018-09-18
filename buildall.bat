@echo off
for /D %%i in (*) do (
	if exist "%%i\FAST" (
		pushd "%%i\FAST"
		call build.bat %*
		popd
	)
)
