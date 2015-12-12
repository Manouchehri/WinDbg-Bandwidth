CFLAGS  = /nologo /Wall /WX /O2
LDFLAGS = /nologo /DLL
EXPORTS = /EXPORT:bandwidth /EXPORT:WinDbgExtensionDllInit \
          /EXPORT:ExtensionApiVersion

all: objs/bandwidth.obj
	@link $(LDFLAGS) $(EXPORTS) /OUT:bandwidth.dll objs\bandwidth.obj

objs/bandwidth.obj: srcs/bandwidth.c srcs/*.h
	@cl $(CFLAGS) /Foobjs\bandwidth.obj /c srcs\bandwidth.c

clean:
	@del bandwidth.dll bandwidth.exp bandwidth.lib objs\*.obj

