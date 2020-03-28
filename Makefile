build: so-cpp.exe

so-cpp.exe: main.c hashmap.c hashmap.h
	cl /MD /W3 main.c hashmap.c -o so-cpp.exe

clean:
	del so-cpp.exe main.obj hashmap.obj
