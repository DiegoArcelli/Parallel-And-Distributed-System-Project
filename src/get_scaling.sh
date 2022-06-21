if [ $# -le 1 ]
then
    echo "Usage: ./scaling.sh cores iterations";
    exit;
fi

cores=$1;
iters=$2;
avg_iters=1;


echo "n;seq_time;threads_time;ff_time;omp_time" > "scaling.csv";

for n in 10 20 50 100 500 1000 5000
do
    seq_avg=0;
    threads_avg=0;
    ff_avg=0;
    omp_avg=0;
    for i in $(seq $avg_iters)
    do
        # getting times
        times=$(./main $n $cores $iters 0 | cut -d " " -f2);
        seq_time=$(echo $times | cut -d " " -f1);
        threads_time=$(echo $times | cut -d " " -f2);
        ff_time=$(echo $times | cut -d " " -f3);
        omp_time=$(echo $times | cut -d " " -f4);

        seq_avg=$(echo $seq_avg+$seq_time | bc -l);
        threads_avg=$(echo $threads_time+$threads_avg | bc -l); 
        ff_avg=$(echo $ff_time+$ff_avg | bc -l); 
        omp_avg=$(echo $omp_time+$omp_avg | bc -l); 
    done
    seq_avg=$(echo "scale=2;$seq_avg/5" | bc -l);
    threads_avg=$(echo "scale=2;$threads_avg/5" | bc -l);
    ff_avg=$(echo "scale=2;$ff_avg/5" | bc -l);
    omp_avg=$(echo "scale=2;$omp_avg/5" | bc -l);

    echo "$n x $n matrix executed";
    echo "$n;$seq_avg;$threads_avg;$ff_avg;$omp_avg" >> "scaling.csv";
done