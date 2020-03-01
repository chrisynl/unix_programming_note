CC = gcc

# 极简
tsh1: tsh1.c execute_cmd_simple.c make_argv.c
	$(CC) -o tsh1 tsh1.c execute_cmd_simple.c make_argv.c

# 带有重定向功能
redi_tsh: tsh1.c execute_cmd_redirect.c parse_redirect.c make_argv.c
	$(CC) -o redi_tsh tsh1.c execute_cmd_redirect.c parse_redirect.c make_argv.c

clean:
	find -maxdepth 1 -type f -executable -delete
