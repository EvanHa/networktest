TARGET_CLIENT = client

cc = gcc

default: client

client:
	$(cc) -o $(TARGET_CLIENT) echo_client.c

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)
