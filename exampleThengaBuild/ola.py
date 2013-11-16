#start with Importing our bulild environment ola
Import ('ola')

'''
  example 1: build a simple executable
  cola => Clone the ola to local ola, this is the first step and its is important 
'''
cola = ola.Clone()
binName = 'exampleThengaBuild.1'
files = ['ex1.cpp']
#note build methods are called on ola and we need to passin the local ola which can
#have our customizations, in this example we have not added/removed so ola = cola here
ola.bld.buildExecutable(cola, binName, files) 

#  example 2: build should pick up the include file from include automatically
cola = ola.Clone()
binName = 'exampleThengaBuild.2'
files = ['ex2.cpp']
ola.bld.buildExecutable(cola, binName, files) 

#  example 2.1: same as ex2, but source is in subdirectory
cola = ola.Clone()
binName = 'exampleThengaBuild.2.1'
files = ['othersrc/ex2.cpp']
ola.bld.buildExecutable(cola, binName, files) 

# example 3: threads
cola = ola.Clone()
binName = 'exampleThengaBuild.3'
files = ['ex3_thread.cpp']
ola.bld.appendCCFlags(cola, ['-std=c++0x'])
ola.bld.appendLibs(cola, ['pthread'])
ola.bld.buildExecutable(cola, binName, files) 

# example 4: build a static lib
cola = ola.Clone()
binName = 'exampleThengaBuild.4'
files = ['ex4_staticlib.cpp']
ola.bld.buildStaticLib(cola, binName, files) 

# example 5: use the static lib
cola = ola.Clone()
binName = 'exampleThengaBuild.5'
files = ['ex5_usestaticlib.cpp']
ola.bld.appendLibs(cola, ['exampleThengaBuild.4']) #use libexampleThengaBuild.4.a
ola.bld.buildExecutable(cola, binName, files) 
