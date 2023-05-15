#!/bin/bash

# TO USE THIS TEST SCRIPT

make clean
make all

total=0
score=0

# Usage: run_test $1 $2 $3 $4
# Args: $1=executable name $2=executable arg $3=expected return value $4=expected message

# This function takes in the arguments stated above and runs the given executable
# with the one specified arguments.
# the output code and string are captured
# and these are compared to the given expected code and string
# displaying a helpful message to the user
# and updating both the number of tests run and the number of passes
run_unit_test () 
    { 
        message=$($1 $2) 
        echo "Test case: $1 $2" 
        $1 $2 > null 
        out=$?
        total=$(( $total+2 ))

        if [ $out == $3 ]
        then
            echo "PASSED [return value]: expected output value \"$3\" got $out"
            (( score++ ))
        else
            echo "FAILED [return value]: expected output value \"$3\" got $out"
        fi

        if [ "$message" == "$4" ] 
        then
            echo "PASSED [output message]: expected printout message \"$4\" got \"$message\""
            (( score++ ))
        else
            echo "FAILED [output message]: expected printout message \"$4\" got \"$message\""
        fi
    }
# you can remove or comment out any executables you don't want to test
# full list of executables: pgmEcho pgmComp etc.
# E.g. EXES = (pgmEcho pgmComp)
EXES=(map1)

for testExecutable in ${EXES[@]}
do 
    path="test_data/"
    echo "-------------- UNIT TESTING $testExecutable --------------"

    echo ""
    echo "Usage"
    run_unit_test ./$testExecutable "" 0 "Usage: ./$testExecutable input.map"

    echo ""
    echo "Bad Arguments"
    run_unit_test ./$testExecutable "1 2" 1 "ERROR: Bad Argument Count"

    echo ""
    echo "Bad Filename"
    run_unit_test ./$testExecutable "1" 2  "ERROR: Bad File Name (1)"

    echo ""
    echo "Bad Label1"
    filename="bad_label1.map"
    full_path=$path$filename
    run_unit_test ./$testExecutable $full_path 9 "ERROR: Bad Labels ($full_path)"
    
    echo ""
    echo "Bad Label2"
    filename="bad_label2.map"
    full_path=$path$filename
    run_unit_test ./$testExecutable $full_path 9 "ERROR: Bad Labels ($full_path)"

    echo ""
    echo "Bad Attributes1"
    filename="bad_attributes.map"
    full_path=$path$filename
    run_unit_test ./$testExecutable $full_path 4 "ERROR: Bad Attributes ($full_path)"

done
    ### Functionality Tests ###

###### DO NOT REMOVE - restoring permissions
# git will be unable to deal with files when we don't have permissions
# so to prevent you having to deal with untracked files, we will restore
# permissions after running the tests.

# chmod +(w / r) means 'give all users write / read permission for the file'
chmod +w test_data/pgms/bad.out
chmod +r test_data/pgms/ascii/test.pgm
###### YOU CAN EDIT BELOW THIS POINT

# the run_test function has been incrementing both a counter for how many tests have run
# and a counter for passes. We will display this (along with a message for clarity).
echo "------------------------------------------------------------------------------"
echo "You passed $score out of $total"
echo "IMPORTANT: this is a count of tests passed - it is not your final grade."
echo "------------------------------------------------------------------------------"


# OPTIONAL - neither of these will affect your tests
# Many programmers prefer to work from a relatively 'tidy' directory
# and so will ensure any temp files created by the tests are removed
# and any object files are removed.

# remove the files we created during the tests
rm tmp null
# and run make clean to remove object files
make clean
