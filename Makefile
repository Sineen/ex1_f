CC = gcc
CPPFLAGS = -DNDEBUG
CFLAGS = -Wall -Wextra -Wvla -g -std=c99
LDFLAGS = -g
LDLIBS = -lm

COMPILE.c = $(CC) $(CPPFLAGS) $(CFLAGS) -c
LINK.c = $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
LINK.o = $(CC) $(LDFLAGS)

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DPNS = $(SRCS:.c=.d)
.DEFAULT_GOAL = ex1
include $(DPNS)

VAL = valgrind
VFLAGS = --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes

# executable
ex1: $(OBJS)
	$(LINK.o) $^ $(LDLIBS) -o $@

# compiling source files
$(OBJS): %.o: %.c
	$(COMPILE.c) $< -o $@

# generating dependencies
$(DPNS): %.d: %.c
	$(CC) -MM $< > $@.$$$$; \
	sed 's/\($*\.o\):/\1 $@:/g' $@.$$$$ > $@; \
	rm -f $@.$$$$

# testing memory use in valgrind
val: valdbg1e3 valdbg1e9
valdbg%: ex1
	$(VAL) $(VFLAGS) --log-file="$@" $< input$*.txt output$*.txt

# creating tar for upload
tar: ex1.tar
ex1.tar:
	tar -cvf $@ ex1.c output1e3.txt output1e9.txt graphs.pdf

clean:
	rm -rf *.d *.o *~ ex1 valdbg*

.PHONY: all clean tar val
