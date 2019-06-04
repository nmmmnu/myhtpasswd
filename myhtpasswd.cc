// make strdup() visible...
//#define _POSIX_C_SOURCE 200809L

#include <unistd.h>	// fork, execvp, setuid
#include <stdlib.h>	// exit
#include <sys/wait.h>	// wait

#include <cstdio>
#include <string>

struct Result{
	static constexpr int HELP	= -1;
	static constexpr int VERIFY	= -2;
	static constexpr int CHANGE	= -3;
	static constexpr int SYS	= -100;
};

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

	template<typename ...Args>
	int myexec(bool const debug, const char *prog, Args... args){
		if (fork() == 0){
			// child
			if (execl(prog, prog, args..., nullptr) != 0)
				error(Result::SYS, "Can not exec()\n");

		}else{
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

} //namespace



namespace config{
	std::string email(const char *host, const char *user){
		std::string s;
		s += user;
		s += '@';
		s += host;

		return s;
	}

	std::string file(const char *host){
		std::string s;
		s += "/VMAIL/AUTH/";
		s += host;

		return s;
	}
} //namespace config



int main(int argc, char **argv){
	if (argc != 4 + 1)
		return help(argv[0]);

	const char *host	= argv[1];
	const char *user	= argv[2];
	const char *old_pass	= argv[3];
	const char *new_pass	= argv[4];

	auto email = config::email(host, user);
	auto file  = config::file(host);

	myexec(
		Result::VERIFY,
		"/usr/bin/doveadm",
		"auth",
		"test",
		email.c_str(),
		old_pass
	);

	myexec(
		Result::CHANGE,
		"/usr/local/bin/htpasswd",
		"-sb",
		file.c_str(),
		user,
		new_pass
	);
}


