all: run

run: playbits playmatrix playlist
	./playbits ./playmatrix ./playlist
playbits: a5_bits.c a5.h
	gcc a5_bits.c a5.h -o playbits -g
playmatrix: a5_matrix.c
	gcc a5_matrix.c -o playmatrix -g
playlist:  a5_list.c
	gcc a5_list.c -o playlist -g
memcheck: play
	valgrind --leak-check=full ./play
