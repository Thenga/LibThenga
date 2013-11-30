#start with Importing our bulild environment ola
Import ('ola')

cola = ola.Clone()
binName = 'testfszd1'
sources = ['testfszd1.cpp']
ola.bld.appendCCFlags(cola, ['-std=c++11'])
ola.bld.buildExecutable(cola, binName, sources) 
