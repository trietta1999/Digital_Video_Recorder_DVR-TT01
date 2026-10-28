..\ffmpeg\bin\ffplay -f dshow -framerate 30 -video_size 1280x720 -i video="USB Video":audio="HDMI CC Digital Audio Interface" -window_title %1 -vf scale=iw:ih
