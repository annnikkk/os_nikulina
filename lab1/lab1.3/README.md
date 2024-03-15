команды:
1) Компиляция исходников в позиционно независимые оъектные файлы:
gcc -c hello_from_dynamic_lib.c -fPIC -o hello_from_dynamic_lib.o
2) Создание динамической библиотеки из переносимых объектных файлов:
gcc -shared hello_from_dynamic_lib.o -o libhello.so
3) Создание объектного файла из hello.c:
gcc -c hello.c -o hello.o
4) Создание исполняемого файла:
gcc hello.o -L./ -lhello -o hello.out
5) Запуск исполняемого файла с подключением динамической библиотеки:
export LD_LIBRARY_PATH=./
./hello.out
