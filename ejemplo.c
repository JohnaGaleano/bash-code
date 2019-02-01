/*
    ejemplo.c

    Ejemplo de uso de la funcion separaItems
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <wait.h>
#include <stdbool.h>
#include "parser.h"
#include <time.h>
#define TAM 201

int main()
{
  for (;;)
  {
    char **items;
    int num, background;
    char expresion[TAM];

    printf("udea-shell> ");
    fgets(expresion, TAM, stdin);

    num = separaItems(expresion, &items, &background);
    if (num > 0)
    {
      if (strcmp(items[0], "udea-pwd") == 0)
      {
        char *cwd;
        char buff[PATH_MAX + 1];
        cwd = getcwd(buff, PATH_MAX + 1);
        if (cwd != NULL)
        {
          printf("El nombre del directorio de trabajo actual es: %s.\n", cwd);
        }
      }
      else if (strcmp(items[0], "udea-cd") == 0)
      {

      }
      else if (strcmp(items[0], "udea-echo") == 0)
      {
        char **argv = items;
        int i = 1;
        while (argv[i] != NULL)
        {
          printf("%s", argv[i]);
          i++;
        }
        printf("\n");
      }
      else if (strcmp(items[0], "udea-clr") == 0)
      {
        int result = system("reset");
        if (result == -1)
        {
          printf("Ocurrio un error al ejecutar el comando.\n");
        }
      }
      else if (strcmp(items[0], "udea-time") == 0)
      {
        time_t clockTime;
        time(&clockTime);
        printf("La hora y fecha actual del sistema es: %s", asctime(localtime(&clockTime)));
        //char y = tm.ctm_year;
      }
      else if (strcmp(items[0], "udea-exit") == 0)
      {
        exit(0);
      }
      else
      {
        int status;
        pid_t child_pid;
        /* Creacion del proceso hijo. */
        if ((child_pid = fork()) < 0)
        {
          perror("No es posible ejecutar el programa");
          exit(-1);
        }
        else if (child_pid == 0)
        {
          // Codigo ejercicio 12
          execlp(items[0], items[0], 0);
          printf("El comando '%s' es inválido\n", items[0]);
        }
        if (!background)
        {
          // Este es el padre... Llamado a wait para esperar a que el hijo termine
          wait(&status);
        }
      }
      liberaItems(items);
    }
  }
  return 0;
}
