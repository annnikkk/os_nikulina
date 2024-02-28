команды:
1) Переводим hello_from_static_lib.c в переносимый объектный файл:
gcc -c hello_from_static_lib.c -o hello_from_ststic_lib.o
2) Переводим hello.c в hello.o тем же способом
3) Создаем статическую библиотеку:
ar crs libhello.a hello_from_static_lib.o
4) Линкуем со статической библиотекой:
gcc hello.o -L./ -lhello -o hello.out
5) Запускаем:
./hello.out
