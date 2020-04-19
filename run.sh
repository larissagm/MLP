#!/bin/bash

echo "--MLP Benchmark--"

make

k=9
for instance in instances/*; do
	echo $instance >> ./benchmark/bm.txt

	echo "Running $instance"
	echo "Instance $k of 22" 

	for i in {1..10}; do
		./mlp ${instance} >> ./benchmark/bm.txt
	done

	k=$(($k + 1))
done

echo "-" >> ./benchmark/bm.txt

echo "Benchmark completed."
