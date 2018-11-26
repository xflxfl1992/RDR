@echo off
:: 如果要文件名带上路径，则需要在dir这一句的%%~nxi上作改动
::                  code by FBY && RMW
if exist binFileNameList.txt del binFileNameList.txt /q
::for /f "delims=" %%i in ('dir *.bin /b /a-d /s') do echo %%~nxi >>binFileNameList.txt
for /f "delims=" %%i in ('dir *.bin /b /a-d /s') do echo %%~dpi%%~nxi >>binFileNameList.txt
if not exist binFileNameList.txt goto no_file
start binFileNameList.txt
exit

:no_file
cls
echo       %cur_dir% 文件夹下没有单独的文件
pause 