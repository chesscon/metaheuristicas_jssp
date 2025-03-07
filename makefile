CC=gcc
CFLAGS=-lm
MODE_COMPILATION=-O2
#DEBUG: MODE_COMPILATION=-g

DATA_DIR=instances
OUTPUT_DIR=output
INST_TEST=$(DATA_DIR)/ft06.txt
PREFIX_EXE=./
#DEBUG: PREFIX_EXE='valgrind -s ./'

EXE_PROBLEM=$(OUTPUT_DIR)/leer_ejemplar

#### LECTURA DE EJEMPLARES

prueba_lectura: ${OUTPUT_DIR} $(INST_TEST) ${EXE_PROBLEM}
	${PREFIX_EXE}${EXE_PROBLEM} $(INST_TEST)

${EXE_PROBLEM}: src/test/test_problem.c src/problem/*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)

${OUTPUT_DIR}:
	mkdir -p ${OUTPUT_DIR}


EXE_RANDOM_SOL=$(OUTPUT_DIR)/random_sol

#### GENERACION DE SOLUCIONES

prueba_random_sol: ${OUTPUT_DIR} $(INST_TEST) $(EXE_RANDOM_SOL)
	${PREFIX_EXE}${EXE_RANDOM_SOL} $(INST_TEST)


${EXE_RANDOM_SOL}: src/test/test_solution.c src/problem/* src/solution/permutations_machs_sol.*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)
	

#### EVALUACION DE SOLUCIONES
EXE_EVAL_SOL=$(OUTPUT_DIR)/eval_sol
prueba_eval_sol: ${OUTPUT_DIR} $(INST_TEST) $(EXE_EVAL_SOL)
	${PREFIX_EXE}${EXE_EVAL_SOL} $(INST_TEST)


${EXE_EVAL_SOL}: src/test/test_evaluate_sol.c src/problem/job_shop_instance.c src/problem/*.h src/solution/*.c src/solution/*.h src/utils.h src/utils.c
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)


#### REPARACION DE SOLUCIONES
EXE_REP_SOL=$(OUTPUT_DIR)/repara_sol
prueba_repara_sol: ${OUTPUT_DIR} $(INST_TEST) $(EXE_REP_SOL)
	${PREFIX_EXE}${EXE_REP_SOL} $(INST_TEST)


${EXE_REP_SOL}: src/test/test_repair_sol.c src/problem/job_shop_instance.c src/problem/*.h src/solution/*.c src/solution/*.h src/utils.h src/utils.c src/recombination/*.*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)


#### VECINDADES
EXE_N1=$(OUTPUT_DIR)/generate_n1

prueba_n1: ${OUTPUT_DIR} $(INST_TEST) $(EXE_N1)
	${PREFIX_EXE}${EXE_N1} $(INST_TEST)

${EXE_N1}: src/test/test_n1.c src/problem/job_shop_instance.c src/problem/*.h src/solution/*.c src/solution/*.h src/utils.h src/utils.c src/neighborhood/*.c src/neighborhood/*.h
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)

#### ************ BUSQUEDAS DE TRAYECTORIA *******************

#### LOCAL SEARCH
EXE_LS=$(OUTPUT_DIR)/local_search

prueba_local_search: ${OUTPUT_DIR} $(INST_TEST) $(EXE_LS)
	${PREFIX_EXE}${EXE_LS} $(INST_TEST)

${EXE_LS}: src/test/test_local_search.c src/problem/*.* src/solution/*.* src/utils.* src/neighborhood/*.* src/trajectory_search/*.*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)

#### TABU SEARCJ
EXE_TS=$(OUTPUT_DIR)/tabu_search
TIPO_VECINDAD=1
MAX_ITERS=
TAM_TABU_LIST=
MIN_TABU_TENURE=

prueba_tabu_search: ${OUTPUT_DIR} $(INST_TEST) $(EXE_TS)
	${PREFIX_EXE}${EXE_TS} $(INST_TEST) $(TIPO_VECINDAD) $(MAX_ITERS) $(TAM_TABU_LIST) $(MIN_TABU_TENURE)

${EXE_TS}: src/test/test_tabu_search.c src/problem/*.* src/solution/*.* src/utils.* src/neighborhood/*.* src/trajectory_search/*.*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)

#### ************ BUSQUEDAS POBLACIONALES *******************

#### ALG GENETICO
EXE_AG=$(OUTPUT_DIR)/alg_genetico

prueba_alg_genetico: ${OUTPUT_DIR} $(INST_TEST) $(EXE_AG)
	${PREFIX_EXE}${EXE_AG} $(INST_TEST)

${EXE_AG}: src/test/test_genetic_algorithm.c src/problem/*.* src/solution/*.* src/utils.* src/neighborhood/*.* src/trajectory_search/*.* src/population/*.* src/recombination/*.* src/population_search/*.*
	$(CC) $(MODE_COMPILATION) -o $@ $^ $(CFLAGS)