#The root Makefile
jobs:=2

debug:
	scons -Q -j$(jobs) buildmode=debug
release:
	scons -Q -j$(jobs) buildmode=release
#example
relO2:
	scons -Q -j$(jobs) buildmode=relO2
all:
	scons -Q -j$(jobs) buildmode=debug
	scons -Q -j$(jobs) buildmode=release
	
#cleaning
cleandebug:
	scons -Q --clean buildmode=debug
cleanrealease:	
	scons -Q --clean buildmode=release
clean:
	scons -Q --clean buildmode=debug
	scons -Q --clean buildmode=release

#getting status
statusdebug:
	scons -Q --tree=status buildmode=debug
statusrelease:	
	scons -Q --tree=status buildmode=release
