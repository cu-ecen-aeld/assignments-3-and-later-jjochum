#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage:"
	echo "finder.sh <directory> <string>"
	exit 1;
fi;

filesdir=$1
searchstr=$2

if [ ! -d $filesdir ]; then
	echo "Directory ${filesdir} does not exist";
	exit 1;
fi; 


matchcount=0
files=()
for line in $(grep -r $searchstr $filesdir 2>/dev/null | awk -F: '{print $1}'); do
	let matchcount++
        files[${#files[@]}]="${line}"
done
unique_files=`for entry in "${files[@]}"; do
	echo $entry
done | uniq | wc -l`
echo "The number of files are ${unique_files} and the number of matching lines are ${matchcount}";
exit 0
