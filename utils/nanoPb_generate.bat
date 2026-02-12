@echo off
setlocal

REM Paths relative to this script
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."
set "PROTO_DIR=%PROJECT_ROOT%\protocol-buffers\protos"
set "OUT_DIR=%PROJECT_ROOT%\src\protocolBuffers\generated"
set "INCLUDE_FILE=%PROJECT_ROOT%\src\protocolBuffers\protocolBuffers_includes.h"
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

REM echo.
REM echo Updating protocolBuffers_includes.h...

REM REM Recreate the include file
REM if exist "%INCLUDE_FILE%" del "%INCLUDE_FILE%"
REM echo // Auto-generated include file > "%INCLUDE_FILE%"
REM echo // Do not edit manually >> "%INCLUDE_FILE%"
REM echo. >> "%INCLUDE_FILE%"

REM REM Add includes for each generated .pb.h file
REM for %%h in ("%OUT_DIR%"\*.pb.h) do (
    REM echo #include "../generated/%%~nxh" >> "%INCLUDE_FILE%"
REM )


echo Done.
pause
