default: analisador_lexico-clang


analisador_lexico-clang: analisador_lexico.c
	gcc analisador_lexico.c -o analisador_lexico
	./analisador_lexico
	cat saida.txt
