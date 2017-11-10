#!/bin/bash

clear;
COM="compiled";
FCNT=$(($(ls -1 sample/CZE/????_out.txt sample/CZE/????_in.txt | wc -l) / 2));

#build
gcc -std=c99 -O2 -Wall -Wextra -pedantic main.c -o $COM;
if test 0 -ne $?; then echo -e "Compilation error\n"; exit 1; fi;

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
		exit 1;
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
