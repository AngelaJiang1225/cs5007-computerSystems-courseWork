#!/bin/bash
# Sys Report Generation
title="System Information Report for $HOSTNAME"
CURRENT_TIME=$(date +"%x %r %Z")
TIME_STAMP="Generated $CURRENT_TIME, by $USER"
echo "cores: "$(($(grep "^physical id" /proc/cpuinfo | awk '{print $4}' | sort -un | tail -1)+1))
free -m | awk 'NR==2{printf "memory: %sMB/%sMB\n", $3,$2}'
df -h | awk '$NF=="/"{printf "primary partition storage: %dGB/%dGB\n", $3,$2}'


