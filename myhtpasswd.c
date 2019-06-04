// make strdup() visible...
//#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>	// fork, execvp
#include <stdlib.h>	// exit
#include <sys/wait.h>	// wait
#include <stdbool.h>



#define	HTPASSWD	"/home/nmmm/Development/myhtpasswd/myhtpasswd.help"
#define HTPASSWD_VERIFY	"VERIFY"
#define HTPASSWD_CHANGE	"CHANGE"



const int SETUID	= -1;	// negative means no need to setuid()

const int RESULT_HELP	= -1;
const int RESULT_VERIFY	= -2;
const int RESULT_CHANGE	= -3;
const int RESULT_SYS	= -100;

static int help(const char *s){
	printf("Usage:\n");
	printf("\t%s [host] [user] [old_password] [new_password]\n", s);

	return RESULT_HELP;
}

inline static void error(int const code, const char *message){
	printf("Error: %s\n", message);
	exit(code);
}

static int myexec(char **args, bool const debug){
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

static void myexec_(int const error, char *a1, char *a2, char *a3, char *a4, bool const debug){
	// this works only with defines or non const values...
	char *args[] = {
		HTPASSWD,
		a1,
		a2,
		a3,
		a4,
		NULL
	};

	int const code = myexec(args, false);

	if (debug)
		printf("Return code: %d\n", code);

	if (code)
		exit(error);
}

inline static void change(char *host, char *user, char *old_pass, char *new_pass){
	bool const debug = false;
	myexec_(RESULT_VERIFY, HTPASSWD_VERIFY, host, user, old_pass, debug);
	myexec_(RESULT_CHANGE, HTPASSWD_CHANGE, host, user, new_pass, debug);
}

static void mysetuid(){
	if (SETUID < 0){
		printf("No need to setuid()\n");
	}else{
		printf("Trying setuid()\n");
		if (setuid(SETUID)){
			printf("Can not setuid()\n");
			exit(RESULT_SYS);
		}
	}
}

int main(int argc, char **argv){
	if (argc != 4 + 1)
		return help(argv[0]);

	mysetuid();

	change(argv[1], argv[2], argv[3], argv[4]);

	return 0;
}


