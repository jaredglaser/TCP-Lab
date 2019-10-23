CC= /usr/bin/gcc

all:	tcpclient tcpserver

tcpclient: tcpclient.c;
	${CC} -g tcpclient.c -o tcpclient

tcpserver: tcpserver.c;
	${CC} -g tcpserver.c -o tcpserver

clean:
	rm tcpclient tcpserver
