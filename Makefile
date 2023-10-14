all:menu

menu:menu.o max.o min.o diff.o sum.o output.o input_arr.o input_command.o
	gcc -std=gnu99 menu.o max.o min.o diff.o sum.o output.o input_arr.o input_command.o -o menu
menu.o:menu.c
	gcc -std=gnu99 -c menu.c
max.o:max.c
	gcc -std=gnu99 -c max.c
min.o:min.c 
	gcc -std=gnu99 -c min.c
diff.o:diff.c 
	gcc -std=gnu99 -c diff.c
sum.o:sum.c
	gcc -std=gnu99 -c sum.c
output.o:output.c 
	gcc -std=gnu99 -c output.c 
input_arr.o:input_arr.c 
	gcc -std=gnu99 -c input_arr.c
input_command.o:input_command.c
	gcc -std=gnu99 -c input_command.c
clean:
	rm -f *.o menu
