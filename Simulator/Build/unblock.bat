@echo off
title Unblock EXE and BAT Files
echo Started...
powershell -Command "Get-ChildItem -Recurse | Unblock-File"
echo Done!
pause
