@echo off
echo.
echo - %~dp0
echo.
::@for /d /r "./prj-vs-rc" %%i in (.vs,Debug,x64) do @if exist %%i ( rd /s /q %%i & echo kick - %%i) 
::@for /r "./prj-vs-rc" %%i in (*.db) do (del /f /s /q %%i)
@for /d /r . %%i in (bin,obj,.vs,Debug,x64) do @if exist %%i ( rd /s /q %%i & echo kick - %%i) 

echo.
