#!/usr/bin/bash

count=0
char="AA"
current=$char


function isValid(){
	local var=$1
	if [[ 65 -le $var && $var -le 90 ]]
	then
		echo 1
		return
		

	else
		if [[ 97 -le $var && $var -le 122 ]]
		then
		 echo 2
			return 
		fi
	fi
	echo 0
}

function countFileTXT(){
	local var=$( find . -type f -name '*.txt' -printf x | wc -c )
	echo $var
}

function removeFile(){
	$(rm -f ".txt")
}

function chr(){
	local var=$(printf "\x$(printf %x $1)")
	echo $var
}

paramater=$1
echo $paramater
if [[ $paramater = "create" ]]
then
	for i in {22..122}
	do
		touch "$i.txt"
	done
fi


echo $paramater
if [[ $paramater = "modify" ]]
then 

	for i in {22..121}
	do
		sth=$(chr $i)
		if [[ $(isValid $i) -eq "1" ]]
		then
			echo $sth	
			touch "$sth.txt"
	#removeFile
		elif [[ $(isValid $i) -eq "2" ]]
		then
			echo $sth
			touch "${sth}1.txt"
		else
			current="$current$char"
			touch "$current.txt"
		fi
	done
fi


