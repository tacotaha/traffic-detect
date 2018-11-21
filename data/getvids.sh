#!/usr/bin/env bash

# Dependency: youtube-dl (http://rg3.github.io/youtube-dl) for downloading videos
# Run 'pip install youtube-dl --user' to install it

command -v youtube-dl >/dev/null 2>&1 || { echo >&2 "youtube-dl is not installed, please install it.  Aborting."; exit 1; }
youtube-dl --no-playlist -a videolinks.txt --format mp4
ls *.mp4 | cat -n | while read n f; do mv "$f" "$n.mp4"; done 
