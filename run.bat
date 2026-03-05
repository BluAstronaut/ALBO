@echo off
cmake --build build

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b %ERRORLEVEL%
)
"build\bin\ALBO.exe" >> ALBO.log 2>&1

pause