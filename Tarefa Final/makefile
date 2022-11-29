CC = gcc
CFLAGS = -Wall
SERVER = servidor
CLIENT = cliente
UTILS = netfunctions
CHAT = chat

all: $(SERVER) $(CLIENT) clean

$(SERVER): $(SERVER).o $(UTILS).o $(CHAT).o
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).o $(UTILS).o $(CHAT).o

$(SERVER).o: $(SERVER).c $(UTILS).h $(CHAT).h
	$(CC) $(CFLAGS) -c $(SERVER).c

$(CLIENT): $(UTILS).o $(CLIENT).o $(CHAT).o
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).o $(UTILS).o $(CHAT).o

$(CLIENT).o: $(CLIENT).c $(UTILS).h $(CHAT).h
	$(CC) $(CFLAGS) -c $(CLIENT).c

$(UTILS).o: $(UTILS).c $(UTILS).h
	$(CC) $(CFLAGS) -c $(UTILS).c

$(CHAT).o: $(CHAT).c $(CHAT).h
	$(CC) $(CFLAGS) -c $(CHAT).c

clean:
	$(RM) *.o *~