COMPILER = mpicc
FLAGS = -O3
NP = 4
EXT = 1

all: clean compile

clean:
	rm -f mpi_solved? ssort Output/output*
	rm -f Tex/report.f* Tex/report.aux Tex/report.log Tex/report.s*

compile:
	$(COMPILER) $(FLAGS) mpi_solved1.c -o mpi_solved1
	$(COMPILER) $(FLAGS) mpi_solved2.c -o mpi_solved2
	$(COMPILER) $(FLAGS) mpi_solved3.c -o mpi_solved3
	$(COMPILER) $(FLAGS) mpi_solved4.c -o mpi_solved4
	$(COMPILER) $(FLAGS) mpi_solved5.c -o mpi_solved5
	$(COMPILER) $(FLAGS) mpi_solved6.c -o mpi_solved6
	$(COMPILER) $(FLAGS) mpi_solved7.c -o mpi_solved7
	$(COMPILER) $(FLAGS) ssort.c -o ssort


runssort:
	mpirun -np $(NP) ./ssort $(N) $(S)


runsolved:
	mpirun -np $(NP) ./mpi_solved$(EXT)