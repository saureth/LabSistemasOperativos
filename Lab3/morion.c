#define _POSIX_SOURCE
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>


void ImprimirCarpeta();
int micp(const char *to, const char *from);
void MyEcho(const char *msj);
void MyClr();
void MyTime();

int main(void){
    MyClr();
    ImprimirCarpeta();
    MyEcho("\nHola\n");
    MyTime();
    /*
    Ejemplo de uso de micp

    char origen[1024];
    strcpy(origen, cwd);
    strcat(origen, "/morion.c");
    char destino[1024];
    strcpy(destino, cwd);
    strcat(destino, "/caso.c");
    micp(origen, destino);
    
    */

    return 0;
}


void ImprimirCarpeta(){
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "%s", cwd);
    else
        perror("getcwd() error");
}


int MiCp(const char *from, const char *to)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}


void MyEcho(const char *msj)
{
    fprintf(stdout, "%s",msj);
}

void MyClr()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void MyTime(){
    time_t rawtime;
    struct tm * timeinfo;
  
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "%s", asctime (timeinfo) );
  
}