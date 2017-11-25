#!/bin/bash

clear;
COM="compiled";
FCNT=$(($(ls -1 sample/CZE/????_out.txt sample/CZE/????_in.txt | wc -l) / 2));

#input check
if [ $# -ne 1 ]; then
	echo -e "ERROR: Invalid count of parameters\nExpected 1\nGot $#\n"
	exit 1
elif [ ! -f "$1" ]; then
	echo -e "ERROR: Invalid type of parameter\nExpected valid *.c or *.cpp file\n"
	exit 2
fi

#build
gcc -std=c99 -O2 -Wall -Wextra -pedantic "$1" -o $COM;
if test 0 -ne $?; then echo -e "Compilation error\n"; exit 3; fi;

#test
for ((i = 0; i < $FCNT; i++))
do
	inFile=sample/CZE/000$i\_in.txt;
	outFile=$i\_res.txt;
	refFile=sample/CZE/000$i\_out.txt;	

	echo -e "\n\nTest #$i:";
	time $(./$COM < $inFile > $outFile);
	echo -e "\n"
	diff $outFile $refFile;
	if [ 0 -ne $? ]
	then
		echo -e "\nError test $i!\nInput data:\n$(cat $inFile)";
		echo -e "\n$TM\n";
		exit 4;
	fi
done

#clear
for ((i = 0; i < $FCNT; i++)); do
	file="_res.txt";
	if [ -f "$i$file" ]; then
		rm -rf "$i$file";
	else echo -e "File $i$file not found\n";
	fi
done
rm -rf $COM;
exit 0
