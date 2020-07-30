CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
BIN=sams
OBJ=main.o sams.o admin.o manager.o teacher.o student.o tools.o

all:$(OBJ)
	$(CC) $(OBJ) -o $(BIN)

main.o:main.c sams.h
	$(CC) $(STD) $(FLAG) -c $< 

sams.o:sams.c sams.h admin.h manager.h teacher.h student.h tools.h
	$(CC) $(STD) $(FLAG) -c $< 

admin.o:admin.c admin.h manager.h tools.h
	$(CC) $(STD) $(FLAG) -c $< 

manager.o:manager.c manager.h teacher.h tools.h
	$(CC) $(STD) $(FLAG) -c $< 

teacher.o:teacher.c teacher.h student.h tools.h
	$(CC) $(STD) $(FLAG) -c $< 

student.o:student.c student.h tools.h
	$(CC) $(STD) $(FLAG) -c $< 

tools.o:tools.c tools.h status.h 
	$(CC) $(STD) $(FLAG) -c $< 

clean:
	rm -rf $(BIN) $(OBJ)
