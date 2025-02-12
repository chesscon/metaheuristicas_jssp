CC=gcc
CFLAGS=-lm

SRC_DIR=src
DATA_DIR=instances

EXE_PROBLEM=output/leer_ejemplar
INST_TEST=$(DATA_DIR)/abz7.txt

prueba_lectura: $(INST_TEST) ${EXE_PROBLEM}
	./${EXE_PROBLEM} $(INST_TEST)

${EXE_PROBLEM}: src/problem/*.c src/problem/*.h
	$(CC) -O2 -o $@ $^ $(CFLAGS)