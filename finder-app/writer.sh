#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $0 <file> <string>"
	exit 1
fi

writefile=$1
writestr=$2

createdir=`dirname "${writefile}"`
if [ ! -d $createdir ]; then
	mkdir -p "${createdir}" 2>/dev/null
	if [ $? -ne 0 ]; then
		echo "Unable to create directory $(dirname "${writefile}")"
		exit 1
	fi;
fi;

echo $writestr > "${writefile}"
exit 0
