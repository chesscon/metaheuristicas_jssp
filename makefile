CC=gcc
CFLAGS=-lm
MODE_COMPILATION=-O2

SRC_DIR=src
DATA_DIR=instances
OUTPUT_DIR=output

EXE_PROBLEM=$(OUTPUT_DIR)/leer_ejemplar
INST_TEST=$(DATA_DIR)/abz7.txt

prueba_lectura: $(INST_TEST) ${EXE_PROBLEM}
	./${EXE_PROBLEM} $(INST_TEST)

${OUTPUT_DIR}:
	mkdir -p ${OUTPUT_DIR}

${EXE_PROBLEM}: src/problem/*.c src/problem/*.h
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)


EXE_RANDOM_SOL=output/random_sol

prueba_random_sol: $(INST_TEST) $(EXE_RANDOM_SOL)
	./${EXE_RANDOM_SOL} $(INST_TEST)


${EXE_RANDOM_SOL}: src/problem/job_shop_instance.c src/problem/*.h src/solution/*.c src/solution/*.h
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)