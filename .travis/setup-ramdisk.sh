#!/bin/bash


export BLDDIR=/home/travis/build

df -h
du -sh "$HOME"
du -sh "$BLDDIR"
sudo mv "$BLDDIR" "$BLDDIR.ori"
sudo mkdir -p "$BLDDIR"
sudo mount -t tmpfs -o size=8192m tmps "$BLDDIR"
time sudo cp -R "$BLDDIR.ori/." "$BLDDIR"
sudo chown -R travis:travis "$BLDDIR"
df -h
du -sh "$HOME"
du -sh "$BLDDIR"
