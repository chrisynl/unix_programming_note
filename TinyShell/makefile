CC = gcc

# 极简
tsh1: tsh1.c execute_cmd_simple.c make_argv.c
	$(CC) -o tsh1 tsh1.c execute_cmd_simple.c make_argv.c

# 带有重定向功能
redi_tsh: tsh1.c execute_cmd_redirect.c parse_redirect.c make_argv.c
	$(CC) -o redi_tsh tsh1.c execute_cmd_redirect.c parse_redirect.c make_argv.c

# 流水线功能
pipeline: tsh1.c execute_redirect.c parse_redirect.c execute_cmd_pipe.c make_argv.c
	$(CC) -o pipeline tsh1.c execute_redirect.c parse_redirect.c execute_cmd_pipe.c make_argv.c

# 带有信号处理
tsh2: tsh2.c execute_redirect.c parse_redirect.c execute_cmd_pipe.c make_argv.c signal_setup.c
	$(CC) -o tsh2 tsh2.c execute_redirect.c parse_redirect.c execute_cmd_pipe.c make_argv.c signal_setup.c

# tsh2的升级处理 
tsh3: tsh3.c execute_redirect.c parse_redirect.c execute_cmd_pipe.c make_argv.c signal_setup.c
	$(CC) -o tsh3 tsh3.c execute_redirect.c parse_redirect.c execute_cmd_pipe.c make_argv.c signal_setup.c

clean:
	find -maxdepth 1 -type f -executable -delete
