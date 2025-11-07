@echo off
:: generate_json.bat - Create VSCode/Clang IntelliSense setup
setlocal enabledelayedexpansion

:: Define folder and file
set "VSCODE_DIR=.vscode"
set "JSON_FILE=%VSCODE_DIR%\c_cpp_properties.json"

:: Make sure .vscode exists
if not exist "%VSCODE_DIR%" (
    mkdir "%VSCODE_DIR%"
    echo Created folder: %VSCODE_DIR%
)

:: Overwrite JSON file
(
echo {
echo     "env": {
echo       "myIncludePath": [
echo         "${workspaceFolder}/include",
echo         "${workspaceFolder}/src"
echo       ],
echo       "myDefines": [
echo         "DEBUG",
echo         "MY_FEATURE=1"
echo       ]
echo     },
echo.
echo     "configurations": [
echo       {
echo         "name": "Clang (Windows)",
echo         "compilerPath": "C:/Program Files/LLVM/bin/clang.exe",
echo         "intelliSenseMode": "clang-x64",
echo.
echo         "cStandard": "c17",
echo            "compilerArgs": [
echo            "-std=c17",
echo            "-Wall",
echo            "-Wextra",
echo            "-Werror",
echo            "-g",
echo            "-o"
echo         ],
echo         "includePath": [
echo            "${workspaceFolder}/include",
echo            "${myIncludePath}",
echo            "${workspaceFolder}/**",
echo            "${workspaceFolder}/src",
echo            "C:/Program Files/LLVM/include"
echo         ],
echo         "defines": [
echo           "${myDefines}",
echo           "_CRT_SECURE_NO_WARNINGS"
echo         ],
echo.
echo         "browse": {
echo           "path": [
echo             "${myIncludePath}",
echo             "${workspaceFolder}"
echo           ],
echo           "limitSymbolsToIncludedHeaders": false
echo         },
echo.
echo         "configurationProvider": "ms-vscode.makefile-tools"
echo       }
echo     ],
echo.
echo     "version": 4,
echo     "enableConfigurationSquiggles": false
echo }
) > "%JSON_FILE%"

echo.
echo Generated %JSON_FILE% with Clang (C23) IntelliSense setup
echo.

endlocal
