#start with Importing our bulild environment ola
Import ('ola')

cola = ola.Clone()
binName = ''
files = ['']
ola.bld.buildExecutable(cola, binName, files) 
