CFLAGS = -g3 -Wall -std=c99 -pedantic
APP = pcienema

$(APP): main.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM) $(APP)
