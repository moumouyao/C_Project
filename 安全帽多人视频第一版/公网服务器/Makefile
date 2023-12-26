main:main.o myagreement.o mydatabase.o mylist.o mysockapi.o
	gcc $^ -o $@ -lpthread -lmysqlclient
%.o:%.c
	gcc -c $<  -o $@ -lpthread -lmysqlclient