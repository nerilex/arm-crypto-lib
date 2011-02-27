# Makefile for Base64-encoding
ALGO_NAME := BASE64

# comment out the following line for removement of base64 from the build process
ENCODINGS += $(ALGO_NAME)

$(ALGO_NAME)_DIR      := base64/
$(ALGO_NAME)_OBJ      := base64_enc.o base64_dec.o
$(ALGO_NAME)_INCDIR   := noekeon/ memxor/
$(ALGO_NAME)_TEST_BIN := main-base64-test.o $(CLI_STD)  \
                         performance_test.o noekeon.o noekeon_prng.o memxor.o
			 
$(ALGO_NAME)_PERFORMANCE_TEST := performance

