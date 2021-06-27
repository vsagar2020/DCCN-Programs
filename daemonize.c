#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

static void daemon_init()
{
    pid_t pid;
    pid = fork();

    if (pid < 0)    exit(EXIT_FAILURE);
    if (pid > 0)    exit(EXIT_SUCCESS);
    if (setsid() < 0)   exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0)    exit(EXIT_FAILURE);
    if (pid > 0)    exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--)  close (x);
    
    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

int main()
{
    daemon_init();

    while (1)
    {
        syslog (LOG_NOTICE, "First daemon started.");
        sleep (20);
        break;
    }
    
    syslog (LOG_NOTICE, "First daemon terminated.");
    closelog();

    return EXIT_SUCCESS;
}