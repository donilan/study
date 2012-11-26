#!/bin/sh
for t in `seq 1 10000`
do
    for x in `seq 1 10`
    do 
	for y in `seq 1 8`
	do
	    curl "http://127.0.0.1:8080/TestLock/ImitateTicket?trains=$x&order=$y&num=-1"&
	    curl "http://127.0.0.1:8080/TestLock/ImitateTicket?trains=$x&order=$y&num=1"&

	done 
    done 
    echo $t
done 