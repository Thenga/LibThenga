dirName:=exampleThengaBuild #The project name is the subdirectory name
jobs:=2                     #build jobs
prettyflg:=1                #custom compile/link messages 

#build
debug:
	cd ..;scons -Q -j$(jobs) printpretty=$(prettyflg) dir=$(dirName) buildmode=debug
release:
	cd ..;scons -Q -j$(jobs) printpretty=$(prettyflg) dir=$(dirName) buildmode=release

#cleaning
cleandebug:
	cd ..; scons -Q --clean dir=$(dirName) buildmode=debug
cleanrealease:	
	cd ..; scons -Q --clean dir=$(dirName) buildmode=release
clean:
	cd ..; scons -Q --clean dir=$(dirName) buildmode=debug
	cd ..; scons -Q --clean dir=$(dirName) buildmode=release	
