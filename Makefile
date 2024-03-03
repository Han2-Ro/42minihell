CC=cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf

SRC_DIR = src
OBJ_DIR = obj
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

NAME = minishell
LIB = libminishell.a

TEST_DIR = tests
TESTS = $(wildcard $(TEST_DIR)/*.c)
TESTBINS = $(patsubst $(TEST_DIR)/test_%.c, $(TEST_DIR)/bin/test_%, $(TESTS))
TEST_UTILS = $(wildcard $(TEST_DIR)/utils/*.c)

all: $(LIB) $(NAME)

$(NAME): $(OBJS)
	make -C ./libft
	$(CC) $(CFLAGS) $(SRCS) -L./libft -lft -o $(NAME)

$(LIB): $(OBJS)
	make -C ./libft
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(TEST_DIR)/bin:
	mkdir $@

$(TEST_DIR)/bin/%: $(TEST_DIR)/%.c $(LIB) $(TEST_DIR)/bin
	$(CC) $(CFLAGS) $< -o $@ -L. -lminishell  -L./libft -lft

test: CFLAGS += -DLOG_LEVEL=DEBUG
test: fclean $(TESTBINS)
	exit_code=0; \
	for test in $(TESTBINS); do \
		$$test || exit_code=1; \
	done; \
	exit $$exit_code

clean:
	make clean -C ./libft
	$(RM) $(OBJ_DIR)

fclean: clean
	make fclean -C ./libft
	$(RM) $(NAME)

re: fclean all