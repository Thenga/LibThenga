dirName		:=exampleThengaBuild #The project name is the subdirectory name
jobs		:=2                  #build jobs
prettyflg	:=1                  #custom compile/link messages 

#build
debug:
	cd ..;scons -Q -j$(jobs) printpretty=$(prettyflg) dir=$(dirName) buildmode=debug
release:
	cd ..;scons -Q -j$(jobs) printpretty=$(prettyflg) dir=$(dirName) buildmode=release

#cleaning
cleandebug:
	scons -Q --clean dir=$(dirName) buildmode=debug
cleanrealease:	
	scons -Q --clean dir=$(dirName) buildmode=release
clean:
	scons -Q --clean dir=$(dirName) buildmode=debug
	scons -Q --clean dir=$(dirName) buildmode=release	