# Documentantion  {#documentation}

The documentation of the Hermes MOD was done using [Doxygen](http://www.doxygen.nl/). Doxygen is the de facto tool for generating documentation for many programming languages and this page we describe the shortcomings of the currents documentations and how to create the documentation using the doxygen.

# Creating the documentation  

For the generation of the documentation, doxygen must be installed. Analytical instructions can be found in the [official site of Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/install.html). After the installation, the user only has to run hermes.doxyfile using doxygen.

In ubuntu 14.04 this is achieved by executing the following in a terminal:

	cd hermes 
	
which changes the current directory to the Hermes MOD source. Generally someone will have to use the path were his source code is. Afterwards we call the doxygen by using: 

	doxygen hermes.doxyfile
	
and the documentation is generated in the doc/bin/ folder. The html folder contains the files needed by a web server and the only need to be move to a public web server and the latex file contains the file needed for a pdf to be produced. The easier way of doing it is by calling the make command and the refman.pdf is created containg the documentation.

# Known shortcoming of doxygen

- Doxygen does not support yet the sql language (12/06/2015). For this reason it was preferred the documentation of the sql commands to happen in the doc file. All the documentations that happened in different files MUST BE KEPT in sync and the files must mention the others files.

- [Doxygen does not support the ordering of pages in a custom (or any) fashion](
http://stackoverflow.com/questions/18001897/how-do-you-order-doxygen-custom-pages). For the time being the ordering is declared in the hermes.doxyfile
