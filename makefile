VPATH=.
cc=g++
file=main.o
exe:main
main:$(file)
	g++ -o main $(file)
$(file):

clean:
	rm *.o main
