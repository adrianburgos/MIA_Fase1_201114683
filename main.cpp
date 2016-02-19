#include <QCoreApplication>
#include "stdlib.h"
#include "stdio.h"

typedef struct s
{
    char c[1024];
}s;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    s s1;
    char *entrada = (char*) malloc(sizeof(char*));
    scanf("%s",entrada);
    FILE* f = fopen("/home/adrian/SimuladorDisco/dico1.dsk", "wb");
    int size = 30;
    for(int i = 0; i<size; i++)
    {
        fwrite(&s1,1024,1,f);
    }
    fclose(f);
    return a.exec();
}
