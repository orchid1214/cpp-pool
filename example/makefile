# A more generalized makefile by Li X. K., 2012.11.23
_PATH_=E:/Sources/cpp-pool

IDIR=./ ./include ../include $(_PATH_)/include
SDIR=./ ./source ../source $(_PATH_)/source
ODIR=object

vpath %.cpp $(SDIR)
vpath %.h $(IDIR)
vpath %.o $(ODIR)

MAIN?=main
SRCS=$(MAIN).cpp demo.cpp
DEPS=
OBJS=$(patsubst %,$(ODIR)/%,$(SRCS:.cpp=.o))

EXEC?=$(MAIN)

#PRT:
#	@echo $(OBJS),$(DEPS),$(SRCS)

CC=g++
CFLAGS=-Wall -lpthread $(addprefix -I ,$(IDIR))
CPPFLAGS=

all:$(EXEC)

$(EXEC):$(OBJS)
	$(CC) $^ -static-libstdc++ -lpthread -o $@

$(ODIR)/%.o:%.cpp $(DEPS)
	test -d $(ODIR) || mkdir $(ODIR)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean: 
	rm -rf $(OBJS) $(EXEC)
	rmdir $(ODIR)
