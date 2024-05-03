#!/bin/bash

cd recordings

input_file=$(ls -t *.wav | head -n1)

length="15"

timestamp=$(date +"%T").wav

ffmpeg -loglevel quiet -ss $(echo $(ffprobe -i "$input_file" -show_entries format=duration -v quiet -of csv="p=0") - "$length" | bc ) -t 30 -i "$input_file" temp.wav  -y 

rm -f "$input_file"

mv temp.wav "$timestamp"

cd ..