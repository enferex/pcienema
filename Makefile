CFLAGS = -g3 -std=c99 -pedantic
APP = pcienema

$(APP): main.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM) $(APP)
