# Makefile for serpent
ALGO_NAME := SERPENT_BITSLICE

# comment out the following line for removement of serpent from the build process
BLOCK_CIPHERS += $(ALGO_NAME)

$(ALGO_NAME)_DIR      := serpent/
$(ALGO_NAME)_INCDIR   := bcal/ memxor/
$(ALGO_NAME)_OBJ      := serpent.o serpent-sboxes-bitslice.o memxor.o
$(ALGO_NAME)_TEST_BIN := main-serpent-test.o bcal_serpent.o $(CLI_STD) $(BCAL_STD)
$(ALGO_NAME)_NESSIE_TEST      := "nessie"
$(ALGO_NAME)_PERFORMANCE_TEST := "performance"

