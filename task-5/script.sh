#!/bin/bash
COUNTER=2
	 while [  $COUNTER -lt 10 ]; do            
             mpiexec -n $COUNTER ./mpiMult mat1 mat2
	     echo MPI processes: $COUNTER
	     let COUNTER=COUNTER+1 
         done

echo $HOSTNAME 
