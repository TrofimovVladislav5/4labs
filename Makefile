all:menu

menu:menu.o max.o min.o diff.o sum.o output.o input_arr.o input_command.o
	gcc menu.o max.o min.o diff.o sum.o output.o input_arr.o input_command.o -o menu
menu.o:menu.c
	gcc -c menu.c
max.o:max.c
	gcc -c max.c
min.o:min.c 
	gcc -c min.c
diff.o:diff.c 
	gcc -c diff.c
sum.o:sum.c
	gcc -c sum.c
output.o:output.c 
	gcc -c output.c 
input_arr.o:input_arr.c 
	gcc -c input_arr.c
input_command.o:input_command.c
	gcc -c input_command.c
clean:
	rm -f *.o menu