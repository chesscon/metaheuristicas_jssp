CC=gcc
CFLAGS=-lm
MODE_COMPILATION=-O2

DATA_DIR=instances
OUTPUT_DIR=output
INST_TEST=$(DATA_DIR)/ft06.txt
PREFIX_EXE=./

EXE_PROBLEM=$(OUTPUT_DIR)/leer_ejemplar

prueba_lectura: ${OUTPUT_DIR} $(INST_TEST) ${EXE_PROBLEM}
	${PREFIX_EXE}${EXE_PROBLEM} $(INST_TEST)

${EXE_PROBLEM}: src/test/test_problem.c src/problem/*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)

${OUTPUT_DIR}:
	mkdir -p ${OUTPUT_DIR}


EXE_RANDOM_SOL=$(OUTPUT_DIR)/random_sol

prueba_random_sol: ${OUTPUT_DIR} $(INST_TEST) $(EXE_RANDOM_SOL)
	${PREFIX_EXE}${EXE_RANDOM_SOL} $(INST_TEST)


${EXE_RANDOM_SOL}: src/test/test_solution.c src/problem/* src/solution/permutations_machs_sol.*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)
	
EXE_EVAL_SOL=$(OUTPUT_DIR)/eval_sol

prueba_eval_sol: ${OUTPUT_DIR} $(INST_TEST) $(EXE_EVAL_SOL)
	${PREFIX_EXE}${EXE_EVAL_SOL} $(INST_TEST)


${EXE_EVAL_SOL}: src/test/test_evaluate_sol.c src/problem/job_shop_instance.c src/problem/*.h src/solution/*.c src/solution/*.h src/utils.h src/utils.c
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)

	
EXE_N1=$(OUTPUT_DIR)/generate_n1

prueba_n1: ${OUTPUT_DIR} $(INST_TEST) $(EXE_N1)
	${PREFIX_EXE}${EXE_N1} $(INST_TEST)


${EXE_N1}: src/test/test_n1.c src/problem/job_shop_instance.c src/problem/*.h src/solution/*.c src/solution/*.h src/utils.h src/utils.c src/neighborhood/*.c src/neighborhood/*.h
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)