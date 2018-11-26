
	#rule to compile Problema1, 2 and 3
build:
	mpicc ./Problema2/MultMatVec.c o ./bin/Mult
	gcc -pthread pi_pthreads.c -o ./bin/P3_pi -lm

# make "proc = n" run_Problema2
run_Problema2 :
	mpiexec -n $(proc) ./bin/Mult

# make "num_Dec = n" "num_Threads = m" run_Problema3
run_Problema3:
	./bin/P3_pi $(num_Dec) $(num_Threads)

cleanup:
	rm ./bin/*
