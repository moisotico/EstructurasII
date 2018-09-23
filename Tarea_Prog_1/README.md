
# Cache Simulator

This project simulates a cache memory for given parameters, using a SRRIP replacement, the Static Re-Reference Interval Prediction with Frequency Priority (RRIP-FP) from the ISCA'2010 paper [1].

## Contents (pending...)


## Installing boost libraries
to run this program we require the boost c++ libraries. If we use a Debian or Ubuntu distro, we run the following program:
```
sudo apt-get install libboost-all-dev 
```
You can also search with:

```
aptitude search boost
```
to find the packages you need and install them using the apt-get command.

### Compile and run program:
```
make 
gunzip -c mcf.trace.gz | cache -t < # > -a < # > -l < # >
```

With # being:
 1. Size of the cache in KB (-t)
 2. Size of the line in bytes (-l)
 3. Associativity (-a)


#### Remove program:
```
make uninstall
```

#### References
[1] Jaleel A, Theobald K B, Steely Jr S C, et al. High performance cache replacement using re-reference interval prediction (RRIP)[C]//ACM SIGARCH Computer Architecture News. ACM, 2010, 38(3): 60-71.
