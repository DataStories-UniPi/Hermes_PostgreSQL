Instructinos for producing the binary file needed by Doxygen

cd bin
flex -L -outfile='sql.scanner.cc' ../sql.ll
mv utfile\=sql.scanner.cc sql.parser.cc
bison -y -d ../sql.yy --output=sql.parser.cc
g++ sql.parser.cc sql.scanner.cc -lfl -o SQL2C++


These files were taken from MADLib (http://madlib.net/) and are under BSD License.
A binary file for linux is also included
