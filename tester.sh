#
#
#	This tester is created for unit testing of c or cpp programs
#	
#	It is created to accept exactly 3 arguments
#	./tester.sh [-c/-cpp] [file] [directory] [--continue]
#
#	Options:
#		-c		Uses gcc -std=c99 compiler
#		
#		-cpp		Uses g++ -std=c++11 compiler
#
#		--continue	Continue testing even if test failed (optional)
#
#
#	File:
#		Expects regular compilable *.c or *.cpp file
#
#
#	Directory:
#		Expects directory with folder named "sample", that contains test files 
#		(e.g. sample/CZE/0000_in.txt)
#
#


#!/bin/bash

clear;
acnt=3
cont=0

#input check
if [ $# -ne $acnt -a $# -ne $(($acnt + 1)) ]; then
	echo -e "ERROR: Invalid count of parameters\nExpected $acnt or $((acnt + 1))\nGot $#\n"
	exit 1
elif [ $1 != "-c" -a $1 != "-cpp" ]; then
	echo -e "ERROR: Invalid type of option\nExpected -c or -cpp\n"
	exit 1
elif [ ! -f "$2" ]; then
	echo -e "ERROR: Invalid type of argument\nExpected valid *.c or *.cpp file\n"
	exit 2
elif [ ! -d "$3" ]; then 
	echo -e "ERROR: Invalid type of argument\nExpected directory where id directory \"sample\" with unit test files\n"
	exit 2
fi

if [[ $# == 4 && $4 != "--continue" ]]; then
	echo -e "ERROR: Invalid type of option\n"
	exit 2
fi

if [ ! -f $3\sample/CZE/0000_in.txt -o ! -f $3\sample/CZE/0000_out.txt ]; then 
	echo -e "ERROR: Invalid tester files hierarchy in $3\n"
	exit 1
fi

COM=$3\compiled;
FCNT=$(($(ls -1 $3\sample/CZE/????_out.txt $3\sample/CZE/????_in.txt | wc -l) / 2));

#build
if [ $1 == "-c" ]; then
	gcc -std=c99 -O2 -Wall -Wextra -pedantic "$2" -o $COM;
else
	g++ -std=c++11 -O2 -Wall -Wextra -pedantic "$2" -o $COM;
fi
if test 0 -ne $?; then echo -e "Compilation error\n"; exit 3; fi;

#test
for ((i = 0; i < $FCNT; i++))
do
	inFile=$3\sample/CZE/000$i\_in.txt;
	outFile=$3$i\_res.txt;
	refFile=$3\sample/CZE/000$i\_out.txt;	

	echo -e "\n\nTest #$i:";
	time $(./$COM < $inFile > $outFile);
	echo -e "\n"
	diff $outFile $refFile;
	if [ 0 -ne $? ]
	then
		echo -e "\nError test $i!\nInput data:\n$(cat $inFile)";
		echo -e "\n$TM\n";
		if [ $# -ne 4 ]; then
			exit 2
		fi
	fi
done

#clear
for ((i = 0; i < $FCNT; i++)); do
	file="_res.txt";
	if [ -f $3$i$file ]; then
		rm -rf $3$i$file;
	else echo -e "File $3$i$file not found\n";
	fi
done
rm -rf $COM;
exit 0;


#
#		(c) Dominik Čermák 2017
#
