start ..\ffmpeg\bin\ffplay -i udp://127.0.0.1:1234 -window_title %1 -vf scale=iw:ih
..\ffmpeg\bin\ffmpeg -y -f dshow -rtbufsize 1G -vsync cfr -async 1 -pix_fmt yuyv422 -i video="USB Video":audio="HDMI CC Digital Audio Interface" -framerate 60 ^
-filter_complex "[v]split=2[out1][out2]" ^
-map "[out1]" -map 0:a -c:v libx264 -preset veryfast -b:v 10M -c:a aac -b:a 128k %2 ^
-map "[out2]" -preset veryfast -f mpegts -omit_video_pes_length 1 udp://127.0.0.1:1234
..\ffmpeg\bin\ffmpeg -y -i %2 -vf "thumbnail=1000,scale=240:180:force_original_aspect_ratio=increase,crop=240:180" -frames:v 1 %2.jpg
