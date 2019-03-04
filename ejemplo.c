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
  char prompt[50] = "udea-shell> ";

  for (;;)
  {
    char **items;
    int num, background;
    char expresion[TAM];

    printf("%s ", prompt);
    fgets(expresion, TAM, stdin);

    num = separaItems(expresion, &items, &background);
    if (num > 0)
    {
      //Ejecución de comandos internos
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
        if (chdir(items[1]) != 0)
        {
          printf("udea-cd: %s no es un directorio válido \n", items[1]);
        }
      }
      else if (strcmp(items[0], "udea-echo") == 0)
      {
        char **argv = items;
        int i = 1;
        if (argv[1] == 0)
        {
          printf("No hay mensaje");
        }

        while (argv[i] != NULL)
        {
          printf("%s ", argv[i]);
          i++;
        }
        printf("\n");
      }
      else if (strcmp(items[0], "udea-clr") == 0)
      {
        printf("\e[1;1H\e[2J");
      }
      else if (strcmp(items[0], "udea-time") == 0)
      {
        time_t clockTime;
        time(&clockTime);
        char timeStamp[30];
        char day[30];
        printf("La hora y fecha actual del sistema es: ");

        strftime(day, sizeof(day), "%A", localtime(&clockTime));
        if (strcmp(day, "Sunday") == 0)
        {
          printf("Domingo");
        }
        else if (strcmp(day, "Monday") == 0)
        {
          printf("Lunes");
        }
        else if (strcmp(day, "Tuesday") == 0)
        {
          printf("Martes");
        }
        else if (strcmp(day, "Thursday") == 0)
        {
          printf("Jueves");
        }
        else if (strcmp(day, "Friday") == 0)
        {
          printf("Viernes");
        }
        else if (strcmp(day, "Saturday") == 0)
        {
          printf("Sábado");
        }
        strftime(timeStamp, sizeof(timeStamp), " %d/%m/%Y - %I:%M:%S %p", localtime(&clockTime));
        printf("%s. \n", timeStamp);
      }
      else if (strcmp(items[0], "udea-exit") == 0)
      {
        exit(0);
      }

      else if (strcmp(items[0], "udea-help") == 0)
      {
        printf("Comandos internos de udea-shell:\n");
        printf("udea-pwd: Imprime en pantalla el directorio de trabajo actual\n");
        printf("udea-cd: Cambia el directorio de trabajo del udea-shell\n");
        printf("udea-echo: Imprime un mensaje en pantalla\n");
        printf("udea-clr: Limpia el registro del udea-shell\n");
        printf("udea-time: Imprime la hora y fecha actual del sistema en formato:[Día][DD/MM/AAAA][Hora]\n");
        printf("udea-exit: Finaliza la ejecución del udea-shell\n");
      }

      //Ejecución de comandos externos
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
