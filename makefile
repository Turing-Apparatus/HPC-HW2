HW=2
hw:  hw$(HW)
hw2: clean compile

clean:
	rm -f mpi_solved? ssort

compile:
	mpicc mpi_solved1.c -o mpi_solved1
	mpicc mpi_solved2.c -o mpi_solved2
	mpicc mpi_solved3.c -o mpi_solved3
	mpicc mpi_solved4.c -o mpi_solved4
	mpicc mpi_solved5.c -o mpi_solved5
	mpicc mpi_solved6.c -o mpi_solved6
	mpicc mpi_solved7.c -o mpi_solved7
	# mpicc ssort.c -o ssort

run:
	mpirun -np 4 ./mpi_solved$(n)

