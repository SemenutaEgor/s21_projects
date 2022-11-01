#!/bin/bash
#cat_test.sh

while read flags_num; do
    echo $flags_num
    res=`echo $flags_num | ./s21_cat`
    echo "$res"
    if [ "$res" != "cat" ]; then
        echo TEST "$res" FAILED: expected cat, got "$res"
    fi
done <<END
    0
END

