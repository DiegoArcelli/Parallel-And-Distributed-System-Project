#!/bin/bash

if [ $# -le 2 ] 
then 
    echo "Usage: ./experiments.sh n iters cores";
    exit;
fi

n=$1;
iters=$2;
n_cores=$3
avg_iters=1;

echo "n_cores;seq_time;ff_time" > "ff_results.csv";


# getting sequential time
seq_avg=0;
for i in $(seq $avg_iters)
do 
    seq_time=$(./main $n 0 $iters 1 | cut -d " " -f2);
    seq_avg=$(echo $seq_avg+$seq_time | bc -l);
done
seq_avg=$(echo "scale=2;$seq_avg/$avg_iters" | bc -l);

# getting parallel times
for cores in $(seq $n_cores)
do
    threads_avg=0;
    ff_avg=0;
    for i in $(seq $avg_iters)
    do

        ff_time=$(./main $n $cores $iters 4 | cut -d " " -f2);
        # echo $times;
        # threads_time=$(echo $times | cut -d " " -f1);
        # threads_avg=$(echo $threads_time+$threads_avg | bc -l); 
        ff_avg=$(echo $ff_time+$ff_avg | bc -l); 
    done
    ff_avg=$(echo "scale=2;$ff_avg/$avg_iters" | bc -l);
    echo "$cores cores executed";
    echo "$cores;$seq_avg;$ff_avg" >> "ff_results.csv";
done
