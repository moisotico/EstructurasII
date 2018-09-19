
# Cache Simulator 

This project simulates a cache memory for given parameters, using a SRIP replacement policy.

## Contents (pending...)

To compile:
```
make
gunzip -c mcf.trace.gz | cache -t < # > -a < # > -l < # >
```

With # being:
 1. Size of the cache in KB (-t)
 2. Size of the line in bytes (-l)
 3. Associativity (-a) 
