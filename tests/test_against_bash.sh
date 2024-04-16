#!/bin/bash
RUN_DIR=temp

< tests/cmds bash > $RUN_DIR/bash.out 2> $RUN_DIR/bash.err
< tests/cmds ./minishell > $RUN_DIR/minishell.out 2> $RUN_DIR/minishell.err
echo "stdout diff:"
diff $RUN_DIR/bash.out $RUN_DIR/minishell.out
echo "stderr diff:"
diff $RUN_DIR/bash.err $RUN_DIR/minishell.err