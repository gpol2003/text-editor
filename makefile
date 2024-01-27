all: clipboard_manager

main.o: main.c queue.h
	gcc main.c -c -g -Wall -Wextra

queue.o: queue.c queue.h
	gcc queue.c -c -g -Wall -Wextra

clipboard_manager: main.o queue.o
	gcc main.o queue.o -o clipboard_manager.exe -g -Wall -Wextra

clean:
	rm *.o
	rm clipboard_manager
test:
	./clipboard_manager.exe
check:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./clipboard_manager.exe
