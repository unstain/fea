TITLE:   FEA Solver Program
CLASS:   AERO 306
SECTION: 501
AUTHOR:  Adelin Destain
CREATED: 03/23/16

 == COMPILING ==

To compile the program with the makefile, enter the following
commands in a terminal set to the root directory of the source
code. The default C compiler is 'cc', but can be changed by specifying
the system C compiler on the command line as shown below. The
compiler used should be able to comply with c99 standards.

	make [CC=<your C compiler>]

If the GNU make program is not available on your system, read 
the following instructions. To compile the program from source 
without GNU make, run the following commands
from the ./src directory.

	cc -c main.c matrix.c beam.c parse.c 
	cc main.o matrix.o beam.o parse.o -o ../feasolver

Where cc can be substituted for any standard c compiler compliant
with the c99 standard.

 == EXECUTING ==

To run the resulting binary, execute the following command.

	./feasolver [PROBLEM_FILE]

Output is normally printed to stdout. However, this output can
be redirected with the redirect operator, e.g.

	./feasolver [PROBLEM_FILE] > outfile.txt

or the output file name can be set with the -o argument, e.g.

	./feasolver -o outfile.txt [PROBLEM_FILE]

An example problem file is included in the project's root directory,
titled 'example.fep'. It is formatted to match the problem described in the
document '3_beamFEACodeDesign.doc', which can be found under the 'doc/'
directory.

The example problem file can be solved with the following command:

	./feasolver example.fep
