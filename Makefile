
SRC += Stm.cpp
SRC += StmTest.cpp
CFLAG += -g -Wall
DFLAG += -DSTM_DEBUG_ON


all:
	g++ $(CFLAG) $(DFLAG) $(SRC) -o Stm
