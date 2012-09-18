# Makefile for HMAC-MD5
ALGO_NAME := HMAC-MD5

# comment out the following line for removement of HMAC-MD5 from the build process
MACS += $(ALGO_NAME)

$(ALGO_NAME)_DIR      := hmac-md5/
$(ALGO_NAME)_INCDIR   := md5/
$(ALGO_NAME)_OBJ      := hmac-md5.o md5.o md5_sbox.o
$(ALGO_NAME)_TESTBIN  := main-hmac-md5-test.o $(CLI_STD) \
                         nessie_mac_test.o nessie_common.o
$(ALGO_NAME)_NESSIE_TEST      := "nessie"
$(ALGO_NAME)_PERFORMANCE_TEST := "performance"

