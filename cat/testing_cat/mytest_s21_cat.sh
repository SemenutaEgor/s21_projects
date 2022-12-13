#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
#LEAK_COUNTER=0
DIFF_RES=""
#VG_RES=""

declare -a multy_testing=(
"VAR 1_multy_test.txt"
"VAR 2_multy_test.txt"
"VAR 3_multy_test.txt"
)

declare -a unique_testing=(
"-s no_file.txt"
"-benst no_file.txt"
"-bn 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt"
"-benst 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt"
"1_multy_test.txt 2_multy_test.txt 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt"
"-bs 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt"
"-ns 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt"
"-benst 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt 2_multy_test.txt 1_multy_test.txt"
"4_empty_test.txt"
"-benst 4_empty_test.txt"
#"-v 4_empty_test.txt"
"4_empty_test.txt 4_empty_test.txt 4_empty_test.txt 4_empty_test.txt"
"-b -e -n -t -s 1_multy_test.txt 2_multy_test.txt no_file.txt 3_multy_test.txt 4_empty_test.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ../s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        echo "TEST   $COUNTER:   OK"
        echo "cat $t"
    else
      (( FAIL++ ))
        echo "TEST  $COUNTER:   FAIL"
        echo "cat $t"
    fi
    rm test_s21_cat.log test_sys_cat.log
}

vg_checking () {
  t=$(echo $@ | sed "s/VAR/$var/")
  valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --num-callers=20 --track-fds=yes --log-file="vg_info.log" ../s21_cat $t > vg_out.log
  VG_RES="$(grep LEAK -c vg_info.log)"
  if [ "$VG_RES" == "0" ]
    then
        echo "NO LEAKS"
    else
      (( LEAK_COUNTER++ ))
        echo "THERE ARE LEAKS"
        exit 1
  fi
  rm vg_info.log vg_out.log
}


for var1 in b e n s t
do
  for i in "${multy_testing[@]}"
  do
    var="-$var1"
    testing $i
#    vg_checking $i
  done
done

for var1 in b e n s t
do
  for var2 in b e n s t
  do
    for i in "${multy_testing[@]}"
      do
        var="-$var1$var2"
        testing $i
#        vg_checking $i
      done
  done
done

for var1 in b e n s t
do
  for var2 in b e n s t
  do
    for var3 in b e n s t
    do
      for i in "${multy_testing[@]}"
      do
        var="-$var1 -$var2 -$var3"
        testing $i
#        vg_checking $i
      done   
    done
  done
done

for var1 in b e n s t
do
  for var2 in b e n s t
   do
    for var3 in b e n s t
    do
      for var4 in b e n s t
      do
        for i in "${multy_testing[@]}"
        do
          var="-$var1$var2$var3$var4"
          testing $i
#          vg_checking $i
        done
      done
    done
  done
done

for i in "${unique_testing[@]}"
do
    var="-"
    testing $i
#    vg_checking $i
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
#echo "LEAKS: $LEAK_COUNTER"
