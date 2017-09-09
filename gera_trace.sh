#!/bin/bash
#
# gera arquivos traces para o simulador de processos\
# ./gera_trace.sh 5
#

NPROCESSOS=${1:-10}
FNAME=${2:-01}

d0=1.0
dt=3.0
df=4.0

i=0
echo ${d0} ${dt} ${df} processo${i} >trace_${FNAME}_n${NPROCESSOS}.txt

for i in `seq 1 ${NPROCESSOS}`; do
    delta=$((1 + RANDOM % 10))
    d0=`echo ${d0} + 0.${delta} | bc` 
    dt=$((0 + RANDOM % 6))
    delta=$((1 + RANDOM % 9))
    df=`echo ${d0} + ${dt}.${delta} +${df} | bc` 
    
    echo ${d0} ${dt}.${delta} $df processo${i} >>trace_${FNAME}_n${NPROCESSOS}.txt
done
