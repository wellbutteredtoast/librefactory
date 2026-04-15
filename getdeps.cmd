@echo off
setlocal
cls

rem This assumes that you've got git installed on your system already.
rem If not, you can get Git 2.10 for Windows here vv
rem https://github.com/git-for-windows/git/releases/tag/v2.10.0.windows.1
rem

set "DEPS_DIR=%~dp0deps"

if exist "%DEPS_DIR%\" (
  echo Dependencies folder already exists, skipping.
  exit /b 0
)

echo Preparing to clone dependencies...
ping -n 2 127.0.0.1 >nul
md "%DEPS_DIR%"
pushd "%DEPS_DIR%"

echo Cloning miniaudio...
git clone --depth 1 https://github.com/mackron/miniaudio.git

echo Cloning Lua 5.2.3...
git clone --depth 1 --branch v5.2.3 https://github.com/lua/lua.git

popd
