#!/usr/bin/env bash
#
#change desktop background
dir="/home/lawlient/Pictures/Wallpapers/"

#export DBUS_SESSION_BUS_ADDRESS environment variable
PID=$(pgrep gnome-session)
export DBUS_SESSION_BUS_ADDRESS=$(grep -z DBUS_SESSION_BUS_ADDRESS /proc/$PID/environ|cut -d= -f2-)

# random a wallpaper
wallpaper=`find "${dir}" -type f | shuf -n1`

# need gnome3 not gnome2

# other option : centered, scaled, spanned, stretched, wallpaper
gsettings set org.gnome.desktop.background picture-options "zoom"

gsettings set org.gnome.desktop.background picture-uri "file://${wallpaper}"
