#include <unistd.h>	// fork, execvp, setuid
#include <stdlib.h>	// exit
#include <sys/wait.h>	// wait

#include <cstdio>
#include <string>

namespace Result{
	const int HELP		= -1;
	const int VERIFY	= -2;
	const int CHANGE	= -3;
	const int SYS		= -100;
}

namespace{
	int help(const char *s){
		printf("Usage:\n");
		printf("\t%s [host] [user] [old_password] [new_password]\n", s);

		return Result::HELP;
	}

	inline void error(int const code, const char *message){
		printf("Error: %s\n", message);
		exit(code);
	}

	void printPack(){
	}

	template <typename... Args>
	void printPack(const char *s, Args... args){
		printf("\t%s\n", s);
		printPack(args...);
	}

	template<typename ...Args>
	int myexec(bool const debug, const char *prog, Args... args){
		if (debug)
			printPack(prog, args...);

		int x = fork();

		switch(x){
		case -1:
			error(Result::SYS, "Can not fork()\n");
			break;

		case 0:
			// child
			if (execl(prog, prog, args..., nullptr) != 0)
				error(Result::SYS, "Can not exec()\n");
			break;

		default:
			// parent
			int stat;
			wait(&stat);
			if (WIFEXITED(stat))
				return WEXITSTATUS(stat);
			else
				error(Result::SYS, "Child did not exit correctly\n");

		}

		// in fact it will never reach this point
		return 0;
	}

	template<typename ...Args>
	void myexec_(int const error, bool const debug, const char *prog, Args... args){
		int const code = myexec(debug, prog, args...);

		if (debug)
			printf("Return code: %d\n", code);

		if (code)
			exit(error);
	}
} //namespace



int main(int argc, char **argv){
	if (argc != 4 + 1)
		return help(argv[0]);

	const char *host	= argv[1];
	const char *user	= argv[2];
	const char *old_pass	= argv[3];
	const char *new_pass	= argv[4];

	auto email = std::string() + user + '@' + host;
	auto file  = std::string() + "/VMAIL/AUTH/" + host;

	bool const debug = false;

	myexec_(
		Result::VERIFY,
		debug,
		"/usr/bin/doveadm",
		"auth",
		"test",
		email.c_str(),
		old_pass
	);

	myexec_(
		Result::CHANGE,
		debug,
		"/usr/local/bin/htpasswd",
		"-sb",
		file.c_str(),
		user,
		new_pass
	);
}


