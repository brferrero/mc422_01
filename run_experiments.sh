#!/bin/bash

N=3
NPROCESSOS=10
for i in `seq 1 ${N}`; do 
    ./gera_trace.sh ${NPROCESSOS} ${i}
    ./ep1 1 trace_${i}_n${NPROCESSOS}.txt out_${i}_n${NPROCESSOS}.txt d
done

rm -f trace_*n{$NPROCESSOS}.txt
