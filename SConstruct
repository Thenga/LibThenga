'''
  /|\+=+=+=+=+=+=+=+=+=+=+=+=+=+/|\
  /|\  Thenga Build SConstruct  /|\
  /|\+=+=+=+=+=+=+=+=+=+=+=+=+=+=/|\  

  Get started: How to build a new project:
  ---------------------------
  step 0.0: checkout the exampleThengaBuild and do make in the exampleThengaBuild dir or
            LibThenga directroy. Your binaries will be created in LibThenga/bin/<somemode> directory
  step 0.1: Now read the following for more deatils:
  
  A project should have a Makefile (not mandatory) and ola.py (mandatory).
  step 1: Makefile
          The Makefiles in the projects (or subdirs) are just dummy wrappers. Copy Makefile from 
          exampleThengaBuild to your subdir which we call as project and change the dirName with
          the subdirectory name.
  
  step 2: copy ola.py from exampleThengaBuild and modify as per your needs.
  
  ola.py is our sconscript. Read on for more details especially for ola and cola
  ola.py should have these three steps:
    -> Import ola                                    ;ola is our base build environment
    -> make cola by cloning ola                      ;we never modify ola, only cola(s)
    -> call cola executors or modifiers(listed below);executors and modifiers are called through ola and they only act on cola(s) 

  [current cola executors]
  ola.bld.buildExecutable    (Cloned ola, Binary Name, [list of files])
  ola.bld.buildLibrary       (Cloned ola, Binary Name, [list of files])
  ola.bld.buildDynamicLibrary(Cloned ola, Binary Name, [list of files])
  ola.bld.buildStaticLib     (Cloned ola, Binary Name, [list of files])

  [current cola modifiers]
  ola.bld.appendLibs         (Cloned Ola, [list of additional libs eg: pthread])
  ola.bld.appendCCFlags      (Cloned Ola, [list of compile flags eg: '-std=c++0x', '-DMYDEBUG'])
  ola.bld.appendLinkFlags    (Cloned Ola, [list of additional linkflags])
  ola.bld.appendIncludePaths (Cloned Ola, [list of additional include paths (outside the current project) eg: '../talgos/include'])
  ola.bld.appendLibPaths     (Cloned Ola, [list of additional lib paths (which are outside LibThenga)])

  Advanced build configuration
  -----------------
  ThengaBldVars contains the global build settings. You can pretty much reconfigure the entire build
  system from here. 
  e.g: We want to build some new binaries (say with some additional compile flags) and want to compare/run
       them against earlier binaries. For this all we need to do is add a new build mode in BUILD_MODE_N_FLAGS dict:
       The build will produce new binaries in the new directory:
       LibThenga/<project>/TARGET_DIR_PREFIX/BUILD_MODE_N_FLAGS.key
       
      +-<root>=LibThenga
        | 
        +-<project1>
        | |
        | +-src
        | +-include
        +-<project2>
        +-<BUILD_OBJECTS_PREFIX 1>
        +-<BUILD_OBJECTS_PREFIX..>
        +-<TARGET_DIR_PREFIX>
          |
          +-<BUILD_MODE_N_FLAGS.key1>
          | |
          | +-<binaries>
          +-<BUILD_MODE_N_FLAGS.key..n>
            |
            +-<binaries>
            
       
'''

import glob,sys,os,re,subprocess
from SCons.Script.SConscript import SConsEnvironment

class ThengaBldVars:
    MAIN_PROJECTS_LIST_FILE = './thengaprojects'#Contains the list of essential projects that needs to be build first. 
                                                #This file will be picked up when thengabuild(make <target>) is triggered from 
                                                #the <root> directory
                                                
    ARGNAME_MAKE_VAR      = 'buildmode' #name of the argument passed to scons, this can come from Makefile also 
                                        #valid values are the keys of: BUILD_MODE_N_FLAGS
    BUILD_MODE_N_FLAGS    = {                   
                              'debug'   : ['-g', '-fno-inline', '-Wall'], 
                              'release' : ['-O3'],
                              'relO2'   : ['-O2', '-Wall'] 
                            }
    ARGNAME_PROJECT       = 'dir'                      #name of the second argument passed 
    ARGNAME_CUST_MSG      = 'printpretty'
    DEFAULT_BUILD_MODE    = 'debug'                    #default build modes 
    BUILD_OBJECTS_PREFIX  = '../.objects_'             #Location of intermediate objects files.Relative path from a project/subdir
    TARGET_DIR_PREFIX     = '../bin'                   #Final target location for binaries,libs etc. -do-
    DEFAULT_SRC_EXT       = ['*.cpp','*.c','*.cc','*.hpp'] #Default source file extensions    
    DEFAULT_SRC_DIR       = 'src/'                     #Pickup sources from <project>/src, if they are not passed
    DEFAULT_INC_DIR       = ['include', '/usr/include', '/usr/local/include'] #default include paths
    DEFAULT_LIB_DIR       = ['/usr/local/lib', '/usr/lib'] #default lib paths, thenga libs go to bin dir 
    SCONSCRIPT_FILE       = 'ola.py'                   #all subproject should have make.py sconscript file

#Thenga's colorful build messages.
class ThengaCustom:
    def __init__(self):
      if not sys.stdout.isatty(): 
        self.blue = self.green = self.yellow = self.red = self.end = ''
      else:
        self.blue    = '\033[94m'  #where the sky is blue
        self.green   = '\033[92m'  #and the leaves of thenga are still green
        self.yellow  = '\033[93m'  #someone kept a yellow banana on green leaves
        self.red     = '\033[91m'  #with some red flowers
        self.end     = '\033[0m'   #enough

    #pretty bulid messages
    def setThengaBuildMessages(self, ola):
        compile_source_message        = '%s/|\ stage:compile %s    : %s$SOURCE%s' %\
          (self.green,self.yellow,self.blue,self.end)
        compile_shared_source_message = '%s/|\ stage:compile()%s   : %s$SOURCE%s' %\
          (self.green,self.yellow,self.blue,self.end)
        link_program_message          = '%s/|\ stage:link %s       : %s$TARGET%s' %\
          (self.red,self.green,self.yellow,self.end)
        link_library_message          = '%s/|\ stage:link static %s: %s$TARGET%s' %\
          (self.yellow,self.green,self.blue,self.end)
        link_shared_library_message   = '%s/|\ stage:link shared %s: %s$TARGET%s' %\
          (self.green,self.red,self.yellow,self.end)
        ola.AppendUnique(CXXCOMSTR   = compile_source_message) 
        ola.AppendUnique(CCCOMSTR    = compile_source_message)
        ola.AppendUnique(SHCCCOMSTR  = compile_shared_source_message)
        ola.AppendUnique(SHCXXCOMSTR = compile_shared_source_message)
        ola.AppendUnique(ARCOMSTR    = link_library_message)
        ola.AppendUnique(SHLINKCOMSTR= link_shared_library_message)
        ola.AppendUnique(LINKCOMSTR  = link_program_message)

    def printPaccha(self):
        print self.green, '-+='*18 , self.end

    def printTError(self, msg):
        print self.green, '/|\\', self.red, msg, self.end

    def printWhereAreWeNow(self, inproj, inmode):
        print '%s /|\ %s{%s %s %s} buildmode = %s %s %s' %\
        (self.green, self.end, self.red, inproj, self.end, self.yellow, inmode, self.end)
      
    def printSubWorking(self, strType, strName):
      print '\t{%s %s %s %s %s %s}' %\
        (self.blue, strType, self.end, self.yellow, strName, self.end)

#Current build types
class ThengaBuildTypes:
    executable     = 'exe'
    library        = 'lib'
    dynamicLibrary = 'dynamiclib'
    staticLibrary  = 'staticlib'
    
"""             /|\              /|\
                /|\ThengaBuilder /|\
                /|\              /|\ 
"""
class ThengaBuilder:
  buildMode = ThengaBldVars.DEFAULT_BUILD_MODE
  tPrinter = ThengaCustom()

  def __getSconscriptPath(self, project):
    return project + '/' + ThengaBldVars.SCONSCRIPT_FILE

  def __build(self, cola, project, filelist, buildType):
    #The Build and Taget dirs
    buildroot  = ThengaBldVars.BUILD_OBJECTS_PREFIX + ola.bld.buildMode
    builddir   = buildroot + '/' + project
    targetpath = ThengaBldVars.TARGET_DIR_PREFIX + '/' + ola.bld.buildMode + '/' + project
    generatedLibPath = ThengaBldVars.TARGET_DIR_PREFIX + '/' + ola.bld.buildMode + '/'

    if filelist is None: 
      filelist = self.GetFiles(ThengaBldVars.DEFAULT_SRC_EXT)
   
    self.appendIncludePaths(cola, ThengaBldVars.DEFAULT_INC_DIR)
    self.appendLibPaths(cola, ThengaBldVars.DEFAULT_LIB_DIR)
    self.appendLibPaths(cola, [generatedLibPath])
    self.appendCCFlags(cola, ThengaBldVars.BUILD_MODE_N_FLAGS.get(self.buildMode))
    
    cola.VariantDir(builddir, ThengaBldVars.DEFAULT_SRC_DIR, duplicate=0)
    srclst = map(lambda x: builddir + '/' + x, filelist)

    if ThengaBuildTypes.executable == buildType:
        self.tPrinter.printSubWorking('executable:- ', project)
        prog = cola.Program(targetpath, source=srclst)
    elif ThengaBuildTypes.dynamicLibrary == buildType:
        self.tPrinter.printSubWorking('dynamicLibrary:- ', project)
        prog = cola.SharedLibrary(targetpath, source=srclst)
    elif ThengaBuildTypes.staticLibrary == buildType:
        self.tPrinter.printSubWorking('staticLibrary:- ', project)
        prog = cola.StaticLibrary(targetpath, source=srclst)
    elif ThengaBuildTypes.library == buildType:
        self.tPrinter.printSubWorking('library:- ', project)
        prog = cola.Library(targetpath, source=srclst)
    else:
        self.tPrinter.printTError('*** invalid binary type ***')
        
    ola.Alias('all', prog)
  
  #helper func
  def getFiles(self, patternList, relPath=None):
      fullList = []
      if relPath is None:
          path = ThengaBldVars.DEFAULT_SRC_DIR
      else:
          path = relPath
      for p in patternList:
          fullList.extend(map(lambda x: x.replace(path,'') ,glob.glob(path + p)))
      if '.'  in fullList: fullList.remove('.')
      if '..' in fullList: fullList.remove('..')
      return fullList

  def subproject(self, project):
      SConscript(self.__getSconscriptPath(project))

  def buildExecutable(self, cola, project, filelist=None):
      self.__build( cola, project, filelist, ThengaBuildTypes.executable )

  def buildLibrary(self, cola, project, filelist=None):
      self.__build( cola, project, filelist, ThengaBuildTypes.library )

  def buildDynamicLibrary(self, cola, project, filelist=None):
      self.__build( cola, project, filelist, ThengaBuildTypes.dynamicLibrary )

  def buildStaticLib(self, cola, project, filelist=None):
      self.__build( cola, project, filelist, ThengaBuildTypes.staticLibrary )

  def appendLibs(self, cola, libList):
      cola.AppendUnique(LIBS = libList)

  def appendCCFlags(self, cola, flagList):
      cola.AppendUnique(CCFLAGS = flagList)

  def appendLinkFlags(self, cola, flagList):
      cola.AppendUnique(LINKFLAGS = flagList)

  def appendIncludePaths(self, cola, pathList):
      cola.AppendUnique(CPPPATH = pathList)

  def appendLibPaths(self, cola, pathList):
      cola.AppendUnique(LIBPATH = pathList)
      cola.AppendUnique(RPATH = pathList)


#Thenga build preparation
class ThengaBuildMgr:

  def __loadProjectList(self, prjList):
    with open(ThengaBldVars.MAIN_PROJECTS_LIST_FILE, "r") as fPlist:
      for line in fPlist:
        prjList.append(line.rstrip('\r\n'))
    
  def startThengaBulid(self):
    ola.bld = ThengaBuilder()
    ola.bld.buildMode = ARGUMENTS.get(ThengaBldVars.ARGNAME_MAKE_VAR, ThengaBldVars.DEFAULT_BUILD_MODE)
    singleProj = ARGUMENTS.get(ThengaBldVars.ARGNAME_PROJECT, ' ')

    
    tCust = ThengaCustom()
    useCustomeCompileMessages = ARGUMENTS.get(ThengaBldVars.ARGNAME_CUST_MSG, "1")
    if useCustomeCompileMessages == "1":
      tCust.setThengaBuildMessages(ola)

    if not (ThengaBldVars.BUILD_MODE_N_FLAGS.has_key(ola.bld.buildMode)):
      tCust.printTError (' Looks like you forgot to add your alien build mode ' + ola.bld.buildMode + 'to thenga BUILD_MODE_N_FLAGS.' )
      Exit(1)

    projectList = []
    if singleProj is not ' ':
      projectList = [ singleProj ]
    else:
      self.__loadProjectList(projectList)

    Export('ola')   #Export ola to sconscripts
    for proj in projectList:
      tCust.printPaccha()
      tCust.printWhereAreWeNow(proj, ola.bld.buildMode)
      tCust.printPaccha()
      ola.bld.subproject(proj)

#let's start from here
ola = Environment()
ola.SConsignFile()
tBuildMgr = ThengaBuildMgr()
tBuildMgr.startThengaBulid()