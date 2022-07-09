#!/bin/bash

if [ $# -le 3 ] 
then 
    echo "Usage: ./experiments.sh n iters cores avg_iters";
    exit;
fi

n=$1;
iters=$2;
n_cores=$3
avg_iters=$4;

echo "n_cores;seq_time;threads_time;ff_time;omp_time" > "results_$n.csv";


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
    omp_avg=0;

    for i in $(seq $avg_iters)
    do

        times=$(./main $n $cores $iters 2 | cut -d " " -f2);
        threads_time=$(echo $times | cut -d " " -f1);
        ff_time=$(echo $times | cut -d " " -f2);
        omp_time=$(echo $times | cut -d " " -f3);

        threads_avg=$(echo $threads_time+$threads_avg | bc -l); 
        ff_avg=$(echo $ff_time+$ff_avg | bc -l); 
        omp_avg=$(echo $omp_time+$omp_avg | bc -l); 

    done
    threads_avg=$(echo "scale=2;$threads_avg/$avg_iters" | bc -l);
    ff_avg=$(echo "scale=2;$ff_avg/$avg_iters" | bc -l);
    omp_avg=$(echo "scale=2;$omp_avg/$avg_iters" | bc -l);

    echo "$cores cores executed";
    echo "$cores;$seq_avg;$threads_avg;$ff_avg;$omp_avg" >> "results_$n.csv";
done