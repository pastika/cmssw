all : singlePi RelValMacro

singlePi : SinglePi.C
	g++ `root-config --cflags --glibs` SinglePi.C -o SinglePi.exe

RelValMacro : RelValMacro.C
	g++ `root-config --cflags --glibs` RelValMacro.C -o RelValMacro.exe