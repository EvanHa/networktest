TOOLCHAIN_PATH = "/usr/local/arm/arm-gcc-3.4.4/gcc-3.4.4-glibc-2.3.3/arm-linux-gnu/bin"
PREFIX = "arm-linux-gnu-"


TARGET_SERVER = server
TARGET_CLIENT = client

cc = $(TOOLCHAIN_PATH)/$(PREFIX)gcc

default: server client

server:
	$(cc) -o $(TARGET_SERVER) ls_server.c

client:
	$(cc) -o $(TARGET_CLIENT) ls_client.c

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)
