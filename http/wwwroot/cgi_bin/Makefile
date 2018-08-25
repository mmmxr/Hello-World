.PHONY:clean
.PHONY:all
all:register login download
register:register.c
	gcc -o $@ $^ -I/usr/include/mysql -L/usr/lib/mysql  -L/usr/lib64/mysql -lmysqlclient 
login:login.c
	gcc -o $@ $^ -I/usr/include/mysql -L/usr/lib/mysql  -L/usr/lib64/mysql -lmysqlclient 
download:download.c
	gcc -o $@ $^ -I/usr/include/mysql -L/usr/lib/mysql  -L/usr/lib64/mysql -lmysqlclient 
clean:
	rm -f register login download
