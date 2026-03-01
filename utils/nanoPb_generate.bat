@echo off
setlocal

REM Paths relative to this script
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."
set "PROTO_DIR=%PROJECT_ROOT%\protocol-buffers\protos"
set "OUT_DIR=%PROJECT_ROOT%\src\protocolBuffers\generated"
REM not the cleanest becasuse env heltec_wifi_lora_32_V3 must have its dependancies downloaded
REM the goal is to have the nanopb generator as a library and not part of this project, also easier for updating
set "NANOPB_GENERATOR_DIR=%PROJECT_ROOT%\.pio\libdeps\heltec_wifi_lora_32_V3\Nanopb"

if not exist "%NANOPB_GENERATOR_DIR%" (
    echo Nanopb not found, make sure you have downloaded libdeps of heltec_wifi_lora_32_V3
    pause
    exit /b
)

if not exist "%OUT_DIR%" (
    mkdir "%OUT_DIR%"
)

echo removing current generated files
del "%OUT_DIR%"\* /Q


echo Generating nanopb files...
echo Proto directory: %PROTO_DIR%
echo Output directory: %OUT_DIR%
echo.

for %%f in ("%PROTO_DIR%\*.proto") do (
    echo Processing %%~nxf
    python "%NANOPB_GENERATOR_DIR%\generator\nanopb_generator.py" ^
        --proto-path "%PROTO_DIR%" ^
        --output-dir "%OUT_DIR%" ^
        "%%f" 
)




