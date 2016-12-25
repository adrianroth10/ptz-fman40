#!/bin/bash

if [ "$#" -ne 1 ];
then
    echo "Illegal number of parameters"
    exit 1
fi

for i in {1..25};
do
	echo -n "Press enter, image ${i}"
	read
	curl -u root:pass http://83.233.133.248:81/axis-cgi/jpg/image.cgi?camera=${1} -o images_${1}/xx${i}.jpg
done
