@echo off
:: ���Ҫ�ļ�������·��������Ҫ��dir��һ���%%~nxi�����Ķ�
::                  code by FBY && RMW
if exist binFileNameList.txt del binFileNameList.txt /q
::for /f "delims=" %%i in ('dir *.bin /b /a-d /s') do echo %%~nxi >>binFileNameList.txt
for /f "delims=" %%i in ('dir *.bin /b /a-d /s') do echo %%~dpi%%~nxi >>binFileNameList.txt
if not exist binFileNameList.txt goto no_file
start binFileNameList.txt
exit

:no_file
cls
echo       %cur_dir% �ļ�����û�е������ļ�
pause 