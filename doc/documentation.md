# Documentantion  {#documentation}

The documentation of the Hermes MOD was done using [Doxygen](http://www.doxygen.nl/). Doxygen is the de facto tool for generating documentation for many programming languages and this page describes the shortcomings of the current documentation and how to create the documentation using the doxygen.

# Creating the documentation  

For the generation of the documentation, doxygen must be installed. Analytical instructions can be found in the [official site of Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/install.html). After the installation, the user only has to run hermes.doxyfile using doxygen.

In ubuntu 14.04 this is achieved by executing the following in a terminal:

	sudo apt-get install doxygen graphviz
	cd hermes 
	
which installs doxygen and changes the current directory to the Hermes MOD source. Generally someone will have to use the path were his source code is. Afterwards we call the doxygen by using:

	doxygen hermes.doxyfile
	
and the documentation is generated in the doc/bin/ folder. The html folder contains the files needed by a web server and the only that remains to be done is move them to a public web server. The latex folder contains the files needed for a pdf to be produced. The easier way of doing it is by calling the make command and the refman.pdf is created containing the documentation.

	cd dob/bin/latex
	make

# Known shortcomings of doxygen

- Doxygen does not support yet the sql language (12/06/2015). For this reason a [flex filter from MADLib](https://github.com/madlib/madlib/wiki/Documentation-Guide-%28Doxygen%29) was used. To create the executable file of the filter, used in the documentation of the sql, [flex](http://flex.sourceforge.net/) and [bison](http://www.gnu.org/software/bison/) are needed. To compile the parser, which is located in doc/SQL2C++ folder, the following commands must be executed (in ubuntu):
		
		sudo apt-get install flex bison
		cd doc/SQL2C++/bin
		flex -L -outfile='sql.scanner.cc' ../sql.ll
		mv utfile\=sql.scanner.cc sql.scanner.cc
 		bison -y -d ../sql.yy --output=sql.parser.cc
 		g++ sql.parser.cc sql.scanner.cc -lfl -o SQL2C++
 	
 The executable file must be placed in doc/SQL2C++/bin folder or change the configurations of the hermes.doxyfile. In other operating systems another compile must be used or [install gcc on the windows](https://gcc.gnu.org/install/binaries.html).
 An executable for linux is already provided with the repository and the end user should not produce a new one. 
 From time to time, someone should check for updates in the filter or if the newer editions of the doxygen support sql. 

- [Doxygen does not support the ordering of pages in a custom (or any) fashion](
http://stackoverflow.com/questions/18001897/how-do-you-order-doxygen-custom-pages). For the time being the ordering is declared in the hermes.doxyfile
