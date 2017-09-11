#!/bin/bash

N=${1:-30}
NPROCESSOS=${2:-20}
METODO=${3:-3}

for i in `seq 1 ${N}`; do 
    ./gera_trace.sh ${NPROCESSOS} ${i}
    ./ep1 ${METODO} trace_${i}_n${NPROCESSOS}.txt out_${i}_n${NPROCESSOS}.txt
done

#rm -f trace_*_n${NPROCESSOS}.txt
mv trace_*_n${NPROCESSOS}.txt results/prior/.
mv out_*_n${NPROCESSOS}.txt results/prior/.
