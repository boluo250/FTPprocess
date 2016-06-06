## debug flag  
DBG_ENABLE   = 0

## debug for debug info, when use gdb to debug  
ifeq (1, ${DBG_ENABLE})   
	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1  
endif  

SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SOURCES))
ELF:=server
CC:=gcc
RM:=rm -rf
LDFLAGS:= -lcrypt
$(ELF):$(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)
	$(RM) $(OBJS)
.PHONY:clean
clean:
	$(RM) $(OBJS) $(ELF)
