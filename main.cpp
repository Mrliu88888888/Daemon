#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

#define _STD_OUT_FILENAME_ "/tmp/process-out.log"
#define _STD_ERR_FILENAME_ "/tmp/process-err.log"



void runAsDaemon()
{
    int pid = fork();
    if (pid < 0)
    {
        // fork fail
        exit(-1);
    }
    else if (pid > 0)
    {
        // parent process
        exit(0);
    }
    else
    {
        // child process
        setsid();

        pid = fork();
        if (pid < 0)
        {
            // fork fail
            exit(-2);
        }
        else if (pid > 0)
        {
            // parent process
            exit(0);
        }
        else
        {
            // child process
            /*
            * /dev/stdin ：0
            * /dev/stdout：1
            * /dev/stderr：2
            */
            int fd = 0;
            for (fd = 0; fd < 3; ++fd)
            {
                close(fd);
            }

            fd = open("/dev/null", O_RDONLY); dup2(fd, 0);
            fd = open(_STD_OUT_FILENAME_, O_RDWR | O_CREAT, 0666); dup2(fd, 1);
            fd = open(_STD_ERR_FILENAME_, O_RDWR | O_CREAT, 0666); dup2(fd, 2);

            umask(0);  //reset file desc
        }
    }
}

int main(int argc, char* argv[])
{
    runAsDaemon();
    cout << "out" << endl;
    cout << "out" << endl;
    cout << "out" << endl;
    cerr << "err" << endl;
    cerr << "err" << endl;
    cerr << "err" << endl;

    while (true)
    {
        std::this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
