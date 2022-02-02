#!/bin/bash

sudo docker stop cv4_00
sudo docker rm cv4_00

sudo docker run -d \
   --runtime=nvidia \
   --restart=always --memory=2g \
   -v /mnt:/mnt \
   --hostname cv4-00 \
   --tmpfs /tmp \
   -it --name cv4_00 cv4:latest

sudo docker start cv4_00
sudo docker exec -it cv4_00 bash