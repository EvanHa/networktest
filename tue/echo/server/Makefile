TARGET_SERVER = server

cc = gcc

default: server

server:
	$(cc) -o $(TARGET_SERVER) echo_server.c

client:
	$(cc) -o $(TARGET_CLIENT) echo_client.c

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)
