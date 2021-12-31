CC=gcc

make: server.c client.c
	@echo compiling...
	$(CC) server.c -o server
	$(CC) client.c -o client
	gnome-terminal
	clear
	
