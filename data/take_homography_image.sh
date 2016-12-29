#!/bin/bash

curl -u root:pass http://83.233.133.248:81/axis-cgi/jpg/image.cgi?camera=1 -o homography/camera_1.jpg
curl -u root:pass http://83.233.133.248:81/axis-cgi/jpg/image.cgi?camera=2 -o homography/camera_2.jpg
