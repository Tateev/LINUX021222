#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void do_command(char* argv[])
{
    int child = fork();

    if (child == 0)
    {
        if (execvp(argv[0], argv) < 0) {
            perror("exec failed");
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
    else if (child < 0) {
        perror("fork failed");
        exit(1);
    }

    int exit_status;
    waitpid(child, &exit_status, 0);

    if (WIFEXITED(exit_status))
    {
        std::cout << "Child exited normally with status code " << WEXITSTATUS(exit_status) << "\n";
    }
    else if (WIFSIGNALED(exit_status))
    {
        std::cout << "Child process was removed.\n";
    }
}

int main()
{
    char* program[4];
    program[0] = "ls";
    program[1] = "-a";
    program[2] = "-l";
    program[3] = nullptr;
    do_command(program);
    program[0] = "cat";
    program[1] = "/etc/lsb-release";
    program[2] = nullptr;
    do_command(program);
}