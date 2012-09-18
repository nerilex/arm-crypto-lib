ALGO_NAME := LIB_CLI_STD

LIBRARYS += ALGO_NAME

$(ALGO_NAME)_DIR := test_src/
$(ALGO_NAME)_OBJ := cli.o hexdigit_tab.o dbz_strings.o string-extras.o uart_i.o \
                    sysclock.o hw_gptm.o dump.o startup.o circularbytebuffer.o \
                    main-test-common.o
$(ALGO_NAME)_BIN := libcli.a


