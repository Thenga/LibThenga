dirName:=compress 
jobs:=2                     
prettyflg:=1                 

debug:
	cd ..;scons -Q -j$(jobs) printpretty=$(prettyflg) dir=$(dirName) buildmode=debug
release:
	cd ..;scons -Q -j$(jobs) printpretty=$(prettyflg) dir=$(dirName) buildmode=release
cleandebug:
	cd ..; scons -Q --clean dir=$(dirName) buildmode=debug
cleanrealease:	
	cd ..; scons -Q --clean dir=$(dirName) buildmode=release
clean:
	cd ..; scons -Q --clean dir=$(dirName) buildmode=debug
	cd ..; scons -Q --clean dir=$(dirName) buildmode=release	
