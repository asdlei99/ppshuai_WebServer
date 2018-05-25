
WebServerps.dll: dlldata.obj WebServer_p.obj WebServer_i.obj
	link /dll /out:WebServerps.dll /def:WebServerps.def /entry:DllMain dlldata.obj WebServer_p.obj WebServer_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del WebServerps.dll
	@del WebServerps.lib
	@del WebServerps.exp
	@del dlldata.obj
	@del WebServer_p.obj
	@del WebServer_i.obj
