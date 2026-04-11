start ..\ffmpeg\bin\ffplay -i udp://127.0.0.1:1234 -window_title %1 -vf scale=iw:ih
..\ffmpeg\bin\ffmpeg -y -re -i dummy_video.mp4 ^
-filter_complex "[v]split=2[out1][out2]" ^
-map "[out1]" -map 0:a -c:v libx264 -preset veryfast -c:a copy %2 ^
-map "[out2]" -preset veryfast -f mpegts -omit_video_pes_length 1 udp://127.0.0.1:1234
..\ffmpeg\bin\ffmpeg -y -i %2 -vf "thumbnail=1000,scale=240:180:force_original_aspect_ratio=increase,crop=240:180" -frames:v 1 %2.jpg
