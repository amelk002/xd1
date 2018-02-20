OUT = rshell
CC = g++
CFLAGS = -Wall -Werror -pedantic -ansi
ODIR = bin
SDIR = src
INC = -Iinc

_OBJS = main.o Separator.o Base.o CMD.o And.o \
    	Or.o Test.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all: $(ODIR)/rshell

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(ODIR)
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

$(ODIR)/rshell: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/$(OUT)
