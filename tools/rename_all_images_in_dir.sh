#!/bin/bash


DIR=$1
FILES_IN_DIR=$(ls ${DIR})

cd ${DIR}

index=0
for f in ${FILES_IN_DIR}
do
	filename=$(basename "${f}")
	extension="${filename##*.}"
	newFileName="${index}.${extension}"
	index=$(expr ${index} + 1)
	echo "${filename} -> ${newFileName}"
done

index=0
read -p "Do you wish to rename these files? (Yy/Nn) " yn
case ${yn} in
	[Yy]* ) 
		for f in ${FILES_IN_DIR}
		do
			filename=$(basename "${f}")
			extension="${filename##*.}"
			newFileName="${index}.${extension}"
			index=$(expr ${index} + 1)
			mv ${filename} ${newFileName}
		done;;
	[Nn]* ) exit;;
esac
	