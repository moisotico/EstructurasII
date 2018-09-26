
# SRRIP Cache Simulator

This project simulates a cache memory for given parameters, using a SRRIP replacement, the Static Re-Reference Interval Prediction with Frequency Priority (RRIP-FP) from the ISCA'2010 paper [1].

## Contents (pending...)



### Compile and run program:

```
make build
gunzip -c mcf.trace.gz | cache -t < # > -a < # > -l < # >
```
Note: the program is still not finished

With # being:
 1. Size of the cache in KB (-t)
 2. Size of the line in bytes (-l)
 3. Associativity (-a)


### Remove program:
```
make uninstall
```

#### References
[1] Jaleel A, Theobald K B, Steely Jr S C, et al. High performance cache replacement using re-reference interval prediction (RRIP)[C]//ACM SIGARCH Computer Architecture News. ACM, 2010, 38(3): 60-71.
