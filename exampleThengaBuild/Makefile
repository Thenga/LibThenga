#The project name is the subdirectory name
dirName:=exampleThengaBuild
jobs:=2

debug:
	cd ..;scons -Q -j$(jobs) dir=$(dirName) buildmode=debug
release:
	cd ..;scons -Q -j$(jobs) dir=$(dirName) buildmode=release


#cleaning
cleandebug:
	scons -Q --clean dir=$(dirName) buildmode=debug
cleanrealease:	
	scons -Q --clean dir=$(dirName) buildmode=release
clean:
	scons -Q --clean dir=$(dirName) buildmode=debug
	scons -Q --clean dir=$(dirName) buildmode=release	