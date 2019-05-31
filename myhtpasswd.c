// make strdup() visible...
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>	// fork, execvp
#include <stdlib.h>	// exit
#include <sys/wait.h>	// wait

const char *HTPASSWD	= "/usr/local/bin/htpasswd";

const int SETUID	= -1;	// negative means no need to setuid()

const int RESULT_HELP	= -1;
const int RESULT_VERIFY	= -2;
const int RESULT_CHANGE	= -3;
const int RESULT_SYS	= -100;

static int help(const char *s){
	printf("Usage:\n");
	printf("\t%s [password_file] [user] [old_password] [new_password]\n", s);

	return RESULT_HELP;
}

inline static void error(int const code, const char *message){
	printf("Error: %s\n", message);
	exit(code);
}

static int myexec(char **args, int const debug){
	if (debug){
		printf("%s\n", args[0]);

		for(char **s = args; *s; ++s)
			printf("\t%s\n", *s);
	}

	if (fork() == 0){
		// child
		const char *prog = args[0];

		if (execvp(prog, args) != 0)
			error(RESULT_SYS, "Can not exec()\n");

	}else{
		// parent
		int stat;
		wait(&stat);
		if (WIFEXITED(stat))
			return WEXITSTATUS(stat);
		else
			error(RESULT_SYS, "Child did not exit correctly\n");
	}
}

inline static void check_code(int const code, int const result, int const debug){
	if (debug)
		printf("Return code: %d\n", code);

	if (code)
		exit(result);
}

int change(char *file, char *user, char *old_pass, char *new_pass){
	char *args[8];

	args[0] = strdup(HTPASSWD);
	args[1] = "-vb";
	args[2] = file;
	args[3] = user;
	args[4] = old_pass;
	args[5] = NULL;

	check_code(
		myexec(args, 0),
		RESULT_VERIFY,
		0
	);

	free(args[0]);

	args[0] = strdup(HTPASSWD);
	args[1] = "-b";
	args[2] = file;
	args[3] = user;
	args[4] = new_pass;
	args[5] = NULL;

	check_code(
		myexec(args, 0),
		RESULT_CHANGE,
		0
	);

	free(args[0]);

	return 0;
}

int main(int argc, char **argv){
	if (argc != 4 + 1)
		return help(argv[0]);

	if (SETUID < 0){
		printf("No need to setuid()\n");
	}else{
		printf("Trying setuid()\n");
		if (setuid(SETUID)){
			printf("Can not setuid()\n");
			exit(RESULT_SYS);
		}
	}

	return change(argv[1], argv[2], argv[3], argv[4]);
}


