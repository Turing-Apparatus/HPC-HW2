HW=2
hw:  hw$(HW)
hw2: compile

clean:
	rm -f mpi_bug?


compile:
	mpicc mpi_bug1.c -o mpi_bug1
	mpicc mpi_bug2.c -o mpi_bug2
	mpicc mpi_bug3.c -o mpi_bug3
	mpicc mpi_bug4.c -o mpi_bug4
	mpicc mpi_bug5.c -o mpi_bug5
	mpicc mpi_bug6.c -o mpi_bug6
	mpicc mpi_bug7.c -o mpi_bug7
	# mpicc ssort.c -o ssort

run1:
	mpirun -np 2 ./mpi_bug1

run2:
	mpirun -np 2 ./mpi_bug2

run3:
	mpirun -np 16 ./mpi_bug3        # np divisible by 4

run4:
	mpirun -np 16 ./mpi_bug4        # np divisible by 4

run5:
	mpirun -np 2 ./mpi_bug5

run6:
	mpirun -np 4 ./mpi_bug6

run7:
	mpirun -np 10 ./mpi_bug7        # np arbitrary

runS:
	mpirun -np 2 ./mpi_bug1

run:
	run1 run2 run3 run4 run5 run6 run7 runS