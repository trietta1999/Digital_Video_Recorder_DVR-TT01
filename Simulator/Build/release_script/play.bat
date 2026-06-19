set "LOCK_FILE=..\record.lock"

:LOOP
if not exist "%LOCK_FILE%" (
    goto :DO_WORK
)

timeout /t 1 /nobreak > nul
goto :LOOP

:DO_WORK
..\ffmpeg\bin\ffplay -i %2 -window_title %1 -vf scale=iw:ih -seek_interval 5
