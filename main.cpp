#include <QCoreApplication>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Bloque
{
    char car[1024];
}Bloque;

typedef struct Disco
{
    char *path;
    char *unit;
    int size;
}Disco;

typedef struct Particion
{
    char estado[1];
    char tipo[1];
    char ajuste[2];
    int byteInicio;
    char nombre[16];
    int size;
}Particion;

typedef struct Mbr
{
    int size;
    char fechaCreacion[128];
    int diskSignature;
    Particion particion1;
    Particion particion2;
    Particion particion3;
    Particion particion4;
}Mbr;

typedef struct parSimple
{
    char cad[50];
    int cant; //cantidad de espacios que me desplace
}parSimple;

typedef struct Formato
{
    int size;
    char *path;
    char *unit;
    char *tipo;
    char *ajuste;
    char *borrar;
    char *nombre;
    int agregar;
}Formato;

typedef struct Ebr
{
    char estado[1];
    char ajuste[2];
    int byteInicio;
    int size;
    int siguiente;
    char nombre[16];
}Ebr;

//obtiene el MBR del disco especificado en la ruta
Mbr* getMbr(char *ruta)
{
    FILE *archivo = fopen(ruta, "r+b");
    Mbr *mbr = (Mbr *) malloc(sizeof(Mbr));
    //se coloca en la primera posicion del archivo
    fseek(archivo, 0, SEEK_SET);
    //se obtiene el Mbr del disco en el que se va a realizar formato
    fread(mbr, sizeof(Mbr), 1, archivo);
    fclose(archivo);
    return mbr;
}

//obtiene el EBR del disco especificado en la ruta
Ebr* getEbr(char *ruta)
{
    Mbr *mbr = getMbr(ruta);
    FILE *archivo = fopen(ruta, "r+b");
    Ebr *ebr = (Ebr *) malloc(sizeof(Ebr));
    //se busca la particion extendida dentro del disco
    if(strcmp(mbr->particion1.estado, "a") == 0 && strcmp(mbr->particion1.tipo, "e") == 0)
        fseek(archivo, mbr->particion1.byteInicio, SEEK_SET);
    if(strcmp(mbr->particion2.estado, "a") == 0 && strcmp(mbr->particion2.tipo, "e") == 0)
        fseek(archivo, mbr->particion2.byteInicio, SEEK_SET);
    if(strcmp(mbr->particion3.estado, "a") == 0 && strcmp(mbr->particion3.tipo, "e") == 0)
        fseek(archivo, mbr->particion3.byteInicio, SEEK_SET);
    if(strcmp(mbr->particion4.estado, "a") == 0 && strcmp(mbr->particion4.tipo, "e") == 0)
        fseek(archivo, mbr->particion4.byteInicio, SEEK_SET);
    fread(ebr, sizeof(Ebr), 1, archivo);
    fclose(archivo);
    return ebr;
}

//establece el MBR del disco especificado en la ruta
void setMbr(char *ruta, Mbr *mbr)
{
    FILE *archivo;
    archivo = fopen(ruta, "r+b");
    fseek(archivo, 0, SEEK_SET);
    fwrite(mbr, sizeof(Mbr), 1, archivo);
    fclose(archivo);
}

//establece el EBR del disco especificado en la ruta
void setEbr(char *ruta, Ebr *ebr)
{
    FILE *archivo;
    archivo = fopen(ruta, "r+b");
    fseek(archivo, ebr->byteInicio, SEEK_SET);
    fwrite(ebr, sizeof(Ebr), 1, archivo);
    fclose(archivo);
}

//escribe los datos del MBR del disco especificado en la ruta
void mostrarMbr(char *ruta, int particion)
{
    printf("ruta mostrarMbr: .%s.\n", ruta);
    Mbr *mbr = (Mbr *) malloc(sizeof(Mbr));
    mbr = getMbr(ruta);
    printf("------------------------~~~~~~------------------------\n");

    switch(particion)
    {
    case 0:
        printf("MBR tam: %d\n", mbr->size);
        printf("MBR fecha: %s\n", mbr->fechaCreacion);
        printf("MBR diskSignature: %d\n", mbr->diskSignature);
        break;
    case 1:
        printf("MBR particion %i estado: %s\n", particion, mbr->particion1.estado);
        printf("MBR particion %i tipo: %s\n", particion, mbr->particion1.tipo);
        printf("MBR particion %i ajuste: %s\n", particion, mbr->particion1.ajuste);
        printf("MBR particion %i byteInicio: %d\n", particion, mbr->particion1.byteInicio);
        printf("MBR particion %i tam: %d\n", particion, mbr->particion1.size);
        printf("MBR particion %i nombre: %s\n", particion, mbr->particion1.nombre);
        break;
    case 2:
        printf("MBR particion %i estado: %s\n", particion, mbr->particion2.estado);
        printf("MBR particion %i tipo: %s\n", particion, mbr->particion2.tipo);
        printf("MBR particion %i ajuste: %s\n", particion, mbr->particion2.ajuste);
        printf("MBR particion %i byteInicio: %d\n", particion, mbr->particion2.byteInicio);
        printf("MBR particion %i tam: %d\n", particion, mbr->particion2.size);
        printf("MBR particion %i nombre: %s\n", particion, mbr->particion2.nombre);
        break;
    case 3:
        printf("MBR particion %i estado: %s\n", particion, mbr->particion3.estado);
        printf("MBR particion %i tipo: %s\n", particion, mbr->particion3.tipo);
        printf("MBR particion %i ajuste: %s\n", particion, mbr->particion3.ajuste);
        printf("MBR particion %i byteInicio: %d\n", particion, mbr->particion3.byteInicio);
        printf("MBR particion %i tam: %d\n", particion, mbr->particion3.size);
        printf("MBR particion %i nombre: %s\n", particion, mbr->particion3.nombre);
        break;
    case 4:
        printf("MBR particion %i estado: %s\n", particion, mbr->particion4.estado);
        printf("MBR particion %i tipo: %s\n", particion, mbr->particion4.tipo);
        printf("MBR particion %i ajuste: %s\n", particion, mbr->particion4.ajuste);
        printf("MBR particion %i byteInicio: %d\n", particion, mbr->particion4.byteInicio);
        printf("MBR particion %i tam: %d\n", particion, mbr->particion4.size);
        printf("MBR particion %i nombre: %s\n", particion, mbr->particion4.nombre);
        break;
    }
    printf("------------------------~~~~~~------------------------\n");
}
//escribe los datos del EBR del disco especificado
void mostrarEbr(char *ruta)
{
    Mbr *mbr = getMbr(ruta);
    FILE *archivo = fopen(ruta, "r+b");
    if(strcmp(mbr->particion1.tipo, "e") == 0)
    {
        fseek(archivo, mbr->particion1.byteInicio, SEEK_SET);
        Ebr *ebr = (Ebr *) malloc(sizeof(Ebr));
        fread(ebr, sizeof(Ebr), 1, archivo);
        fclose(archivo);
        printf("----Particion 1 EBR ----\n");
        printf("EBR estado: .%s.\n", ebr->estado);
        printf("EBR ajuste: .%s.\n", ebr->ajuste);
        printf("EBR byteInicio: .%d.\n", ebr->byteInicio);
        printf("EBR size: .%d.\n", ebr->size);
        printf("EBR siguiente: .%d.\n", ebr->siguiente);
        printf("EBR nombre: .%s.\n", ebr->nombre);
        printf("=============================\n");
    }
    if(strcmp(mbr->particion2.tipo, "e") == 0)
    {
        fseek(archivo, mbr->particion2.byteInicio, SEEK_SET);
        Ebr *ebr = (Ebr *) malloc(sizeof(Ebr));
        fread(ebr, sizeof(Ebr), 1, archivo);
        fclose(archivo);
        printf("----Particion 2 EBR ----\n");
        printf("EBR estado: .%s.\n", ebr->estado);
        printf("EBR ajuste: .%s.\n", ebr->ajuste);
        printf("EBR byteInicio: .%d.\n", ebr->byteInicio);
        printf("EBR size: .%d.\n", ebr->size);
        printf("EBR siguiente: .%d.\n", ebr->siguiente);
        printf("EBR nombre: .%s.\n", ebr->nombre);
        printf("=============================\n");
    }
    if(strcmp(mbr->particion3.tipo, "e") == 0)
    {
        fseek(archivo, mbr->particion3.byteInicio, SEEK_SET);
        Ebr *ebr = (Ebr *) malloc(sizeof(Ebr));
        fread(ebr, sizeof(Ebr), 1, archivo);
        fclose(archivo);
        printf("----Particion 3 EBR ----\n");
        printf("EBR estado: .%s.\n", ebr->estado);
        printf("EBR ajuste: .%s.\n", ebr->ajuste);
        printf("EBR byteInicio: .%d.\n", ebr->byteInicio);
        printf("EBR size: .%d.\n", ebr->size);
        printf("EBR siguiente: .%d.\n", ebr->siguiente);
        printf("EBR nombre: .%s.\n", ebr->nombre);
        printf("=============================\n");
    }
    if(strcmp(mbr->particion4.tipo, "e") == 0)
    {
        fseek(archivo, mbr->particion4.byteInicio, SEEK_SET);
        Ebr *ebr = (Ebr *) malloc(sizeof(Ebr));
        fread(ebr, sizeof(Ebr), 1, archivo);
        fclose(archivo);
        printf("----Particion 4 EBR ----\n");
        printf("EBR estado: .%s.\n", ebr->estado);
        printf("EBR ajuste: .%s.\n", ebr->ajuste);
        printf("EBR byteInicio: .%d.\n", ebr->byteInicio);
        printf("EBR size: .%d.\n", ebr->size);
        printf("EBR siguiente: .%d.\n", ebr->siguiente);
        printf("EBR nombre: .%s.\n", ebr->nombre);
        printf("=============================\n");
    }
}

//devolvera la primera partcion activa a partir de la particion de inicio + 1
int buscarParticion(Mbr *mbr, int inicio)
{
    switch(inicio)
    {
    case 1:
        if(strcmp(mbr->particion2.estado,"a") == 0)
            return 2;
        else
            if(strcmp(mbr->particion3.estado,"a") == 0)
                return 3;
            else
                if(strcmp(mbr->particion4.estado,"a") == 0)
                    return 4;
        break;
    case 2:
        if(strcmp(mbr->particion3.estado,"a") == 0)
            return 3;
        else
            if(strcmp(mbr->particion4.estado,"a") == 0)
                return 4;
        break;
    case 3:
        if(strcmp(mbr->particion4.estado,"a") == 0)
            return 4;
        break;
    }
    return -1;
}

//devolvera 0 si es posible insertar particiones
int cantidadParticiones(char *ruta)
{
    int cantParticiones = 0;
    Mbr *mbr = getMbr(ruta);
    if(strcmp(mbr->particion1.estado, "a") == 0)
        cantParticiones++;
    if(strcmp(mbr->particion2.estado, "a") == 0)
        cantParticiones++;
    if(strcmp(mbr->particion3.estado, "a") == 0)
        cantParticiones++;
    if(strcmp(mbr->particion4.estado, "a") == 0)
        cantParticiones++;
    return cantParticiones;
}

//devolvera true si existe una particion extendida dentro del disco deseado
bool hayExtendida(char *ruta)
{
    Mbr *mbr = getMbr(ruta);
    if(strcmp(mbr->particion1.estado, "a") == 0 && strcmp(mbr->particion1.tipo, "e") == 0)
        return true;
    if(strcmp(mbr->particion2.estado, "a") == 0 && strcmp(mbr->particion2.tipo, "e") == 0)
        return true;
    if(strcmp(mbr->particion3.estado, "a") == 0 && strcmp(mbr->particion3.tipo, "e") == 0)
        return true;
    if(strcmp(mbr->particion4.estado, "a") == 0 && strcmp(mbr->particion4.tipo, "e") == 0)
        return true;
    return false;
}

//lee hasta encontrar el limite establecido
parSimple *leerHasta(char* str, char limite)
{
    parSimple *retorno = (parSimple *) malloc(sizeof(parSimple));
    //se limpia la cadena de retorno
    for(int i = 0; i<=50; i++)
        retorno->cad[i] = '\0';
    int cont = 0;
    bool esPath = false;
    while(*str != limite && str != NULL && *str != '\n' && *str != '\0')
    {
        if(*str == '/')
            esPath = true;
        if(!esPath)
            *str = tolower(*str);
        retorno->cad[cont] = *str;
        //avanzo de posicon en el puntero
        str++;
        cont++;
    }
    retorno->cant = cont;
    return retorno;
}

Disco* parMkdisk(char parametros[100])
{
    Disco *tempDisco = (Disco*) malloc(sizeof(Disco));
    //parametros por defecto
    tempDisco->unit = "m";
    tempDisco->size = 0;
    char*par = parametros;
    //recorre toda la cadena
    while(*par != '\n' && *par != '\0' && par != NULL)
    {
        //avanza hasta encontrar un guion '-'
        while(*par != '-')
            par++;
        parSimple* parNuevo = leerHasta(par,' ');
        char* cadena = parNuevo->cad;
        cadena = strtok(cadena, "=");
        if(strcmp(cadena,"-size")==0)
        {
            cadena = strtok(NULL, "=");
            tempDisco->size = atoi(cadena);
        }else
            if(strcmp(cadena,"-path")==0)
            {
                cadena = strtok(NULL, "=");
                tempDisco->path = cadena;
            }else
                if(strcmp(cadena,"-unit")==0)
                {
                    cadena = strtok(NULL, "=");
                    tempDisco->unit = cadena;
                }
        //se corre la cantidad de espacios necesarios para
        //eliminar el parametro que se ha leido
        for(int i = 0; i<parNuevo->cant; i++)
            par++;
    }
return tempDisco;
}

char* parRmdisk(char parametros[100])
{
    char * retorno = parametros;
    //avanza hasta encontrar un guion '-'
    while(*retorno != '-')
        retorno++;
    retorno = leerHasta(retorno, ' ')->cad;
    retorno = strtok(retorno, "=");
    if(strcmp(retorno, "-path") == 0)
    {
        //avanza hacia el path
        retorno = strtok(NULL, "=");
    }

    return retorno;
}

Formato* parFdisk(char parametros[100])
{
    Formato *formato = (Formato *) malloc(sizeof(Formato));
    //parametros por defecto
    formato->size = 0;
    formato->tipo = "p";
    formato->ajuste = "wf";
    formato->unit = "k";
    formato->borrar = "";
    formato->agregar = 0;
    char *par = parametros;
    while(*par != '\n' && *par != '\0' && par != NULL)
    {
        //avanza hasta encontrar un guion '-'
        while(*par != '-')
            par++;
        //recibe solo el parametro deseado
        parSimple *parNuevo = leerHasta(par, ' ');
        printf("parametro nuevo: .%s.\n", parNuevo->cad);
        char *cadena = strtok(parNuevo->cad, "=");
        if(strcmp(cadena, "-size") == 0)
        {
            cadena = strtok(NULL, "=");
            formato->size = atoi(cadena);
        }
        else
            if(strcmp(cadena, "-path") == 0)
            {
                cadena = strtok(NULL, "=");;
                formato->path = cadena;
            }
            else
                if(strcmp(cadena, "-type") == 0)
                {
                    cadena = strtok(NULL, "=");
                    formato->tipo = cadena;
                }
                else
                    if(strcmp(cadena, "-fit") == 0)
                    {
                        cadena = strtok(NULL, "=");
                        formato->ajuste = cadena;
                    }
                    else
                        if(strcmp(cadena, "-delete") == 0)
                        {
                            cadena = strtok(NULL, "=");
                            formato->borrar = cadena;
                        }
                        else
                            if(strcmp(cadena, "-name") == 0)
                            {
                                cadena = strtok(NULL, "=");
                                formato->nombre = cadena;
                            }
                            else
                                if(strcmp(cadena, "-add") == 0)
                                {
                                    cadena = strtok(NULL, "=");;
                                    formato->agregar = atoi(cadena);
                                }
                                else
                                    if(strcmp(cadena, "-unit") == 0)
                                    {
                                        cadena = strtok(NULL, "=");;
                                        formato->unit = cadena;
                                    }
        //se corre la cantidad de espacios necesarios para
        //eliminar el parametro que se ha leido
        for(int i = 0; i<parNuevo->cant; i++)
            par++;
    }
    return formato;
}

void mkdisk(char parametros[100])
{
    bool hayError = false;
    Bloque *bloque = (Bloque *) malloc(sizeof(Bloque));
    //se obtienen los parametros del disco a crear (size, path, unit)
    Disco *nuevoDisco = parMkdisk(parametros);
    //printf("size: %d\n", nuevoDisco->size);
    //printf("path: %s\n", nuevoDisco->path);
    //printf("unit: %s\n", nuevoDisco->unit);
    if(strcmp(nuevoDisco->unit, "m") == 0)
    {
        nuevoDisco->size = nuevoDisco->size * 1024;
        hayError = false;
    }
    else
        if(strcmp(nuevoDisco->unit, "k") == 0)
            hayError = false;
        else
        {
            hayError = true;
            printf("El parametro -UNIT NO ES CORRECTO\n");
        }
    if(nuevoDisco->size <= 0)
    {
        hayError = true;
        printf("El parametro -SIZE NO ES CORRECTO\n");
    }
    FILE *ar = fopen(nuevoDisco->path, "rb");
    if(ar != NULL)
    {
        hayError = true;
        printf("El disco que se desea CREAR YA EXISTE\n");
        fclose(ar);
    }
    //si no hay error se crea el disco
    if(!hayError)
    {
        FILE *archivo = fopen(nuevoDisco->path, "w+b");
        //printf("sizeof(Bloque) %d\n", sizeof(Bloque));
        for(int i = 0; i< nuevoDisco->size; i++)
            fwrite(bloque, sizeof(Bloque), 1, archivo);
        fclose(archivo);

        Mbr *nuevoMbr = (Mbr *) malloc(sizeof(Mbr));
        Particion particionTemp;
        strcmp(particionTemp.ajuste,"wf");
        particionTemp.byteInicio = 0;
        strcmp(particionTemp.estado,"i");
        strcpy(particionTemp.nombre, "-");
        particionTemp.size = 0;
        strcmp(particionTemp.tipo, "p");
        nuevoMbr->size = nuevoDisco->size * 1024;
        //para obtener la fecha y la hora
        time_t tiempo = time(0);
        struct tm *ptrTiempo = localtime(&tiempo);
        strftime(nuevoMbr->fechaCreacion, 128, "%d/%m/%y %H:%M:%S", ptrTiempo);
        nuevoMbr->diskSignature = rand() % 9999;
        nuevoMbr->particion1 = particionTemp;
        nuevoMbr->particion2 = particionTemp;
        nuevoMbr->particion3 = particionTemp;
        nuevoMbr->particion4 = particionTemp;

        printf("fecha: %s\n", nuevoMbr->fechaCreacion);
        printf("diskSignature: %d\n",nuevoMbr->diskSignature);
        printf("sizeof(Mbr): %d\n",sizeof(Mbr));
        printf("sizeof(Particion): %d\n",sizeof(Particion));

        setMbr(nuevoDisco->path, nuevoMbr);
        mostrarMbr(nuevoDisco->path, 0);
        printf("El disco se CREO CON EXITO\n");
    }
}

void rmdisk(char parametros[100])
{
    bool hayError = false;
    //se obtiene los parametros para el archivo a remover (path)
    char *archivoRemover = parRmdisk(parametros);
    FILE *ar = fopen(archivoRemover, "r+b");
    if(ar == NULL)
    {
        hayError = true;
        printf("El disco que se desea ELIMINAR NO EXISITE\n");
    }else
        fclose(ar);
    if(!hayError)
    {
        remove(archivoRemover);
        printf("El disco se ELIMINO CON EXITO\n");
    }
    hayError = false;
}

int fdisk(char parametros[100])
{
    bool flag = false;
    Particion nuevaParticion;
    Formato *nuevoFormato = parFdisk(parametros);
    //se reconocio que se desea crear una particion primaria
    //se obtiene el disco en que se desea realizar formato
    FILE *ar = fopen(nuevoFormato->path, "rb");
    if(ar == NULL)
    {
        printf("El disco al que se le desea dar formato NO EXISTE");
        return 0;
    }
    fclose(ar);
    //se establece el size y add
    if(strcmp(nuevoFormato->unit, "m") == 0)
    {
        nuevoFormato->size = nuevoFormato->size * 1024 * 1024;
        nuevoFormato->agregar = nuevoFormato->agregar * 1024 * 1024;
    }
    else
        if(strcmp(nuevoFormato->unit, "k") == 0)
        {
            nuevoFormato->size = nuevoFormato->size * 1024;
            nuevoFormato->agregar = nuevoFormato->agregar * 1024;
        }
        else
            if(strcmp(nuevoFormato->unit, "b") == 0)
            {
                nuevoFormato->size = nuevoFormato->size;
                nuevoFormato->agregar = nuevoFormato->agregar;
            }
            else
            {
                printf("El parametro -UNIT NO ES CORRECTO\n");
                return 0;
            }

    if(cantidadParticiones(nuevoFormato->path) >= 4)
    {
        printf("La cantidad de particiones ya esta en su limite\n");
        return 0;
    }

    if(hayExtendida(nuevoFormato->path) && strcmp(nuevoFormato->tipo, "e") == 0)
    {
        printf("Ya existe una particion extendida dentro del disco\n");
        return 0;
    }
    //se obtienen los datos del MBR del disco que se desea
    Mbr *mbr = getMbr(nuevoFormato->path);
    //Se borrar la particion dependiendo del parametro
    if(strcmp(nuevoFormato->borrar, "") != 0)
    {
        //se eliminara de manera rapida
        if(strcmp(nuevoFormato->borrar, "fast") == 0)
        {//si se encuentra la particion en cual se desea eliminar
         //se establece el estado de la particon como inactiva (i)
            if(strcmp(mbr->particion1.nombre, nuevoFormato->nombre) == 0)
                strcmp(mbr->particion1.estado, "i");
            else
                if(strcmp(mbr->particion2.nombre, nuevoFormato->nombre) == 0)
                    strcmp(mbr->particion2.estado, "i");
                else
                    if(strcmp(mbr->particion3.nombre, nuevoFormato->nombre) == 0)
                        strcmp(mbr->particion3.estado, "i");
                    else
                        if(strcmp(mbr->particion4.nombre, nuevoFormato->nombre) == 0)
                            strcmp(mbr->particion4.estado, "i");
                        else
                            printf("La particion \"%s\" no se encuentra dentro del disco\n", nuevoFormato->nombre);
            setMbr(nuevoFormato->path, mbr);
            mostrarMbr(nuevoFormato->path, 1);
            mostrarMbr(nuevoFormato->path, 2);
            mostrarMbr(nuevoFormato->path, 3);
            mostrarMbr(nuevoFormato->path, 4);
        }
        else
            if(strcmp(nuevoFormato->borrar, "full") == 0)
            {
                printf("FULL\n");
            }
            else
            {
                printf("El parametro -DELETE no es correcto\n");
            }
    }
    else
        if(nuevoFormato->agregar != 0)
        {//se comprueba si se desea agregar o eliminar espacio dentro de la particion
            if(nuevoFormato->agregar < 0)
            {
                printf("QUITAR ESPACIO\n");
            }
            else
                printf("AGREGAR ESPACIO\n");
        }
        else
        {//se desea crear particion ya que los parametros -delete y -add no fueron ingresados
            int espacioDisponible = 0;
            strcmp(nuevaParticion.tipo, nuevoFormato->tipo);
            strcmp(nuevaParticion.ajuste, nuevoFormato->ajuste);
            strcmp(nuevaParticion.estado, "a");
            strcpy(nuevaParticion.nombre, nuevoFormato->nombre);
            nuevaParticion.size = nuevoFormato->size;
            if(strcmp(nuevoFormato->tipo,"p") == 0 || strcmp(nuevoFormato->tipo, "e") == 0)
            {
                printf("--Particion primaria--\n");
                //mostrarMbr(nuevoFormato->path, 1);
                if(strcmp(mbr->particion1.estado,"i") == 0)
                {
                    printf("particion 1 inactiva\n");
                    nuevaParticion.byteInicio = sizeof(Mbr);
                    //se busca la primer particion ocupada
                    switch (buscarParticion(mbr, 1))
                    {
                    case 2:
                        espacioDisponible = mbr->particion2.byteInicio - sizeof(Mbr);
                        //la nueva particion si cabe dentro del espacio disponible
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion1 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    case 3:
                        espacioDisponible = mbr->particion3.byteInicio - sizeof(Mbr);
                        //la nueva particion si cabe dentro del espacio disponible
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion1 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    case 4:
                        espacioDisponible = mbr->particion4.byteInicio - sizeof(Mbr);
                        //la nueva particion si cabe dentro del espacio disponible
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion1 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    default:
                        espacioDisponible = mbr->size - sizeof(Mbr);
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion1 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    }

                }
                //la particion 2 es la nueva particion inactiva
                if(strcmp(mbr->particion2.estado, "i") == 0 && !flag)
                {
                    printf("particion 2 inactiva\n");
                    //si la particion 1 esta activa el byteInicio sera establecido
                    if(strcmp(mbr->particion1.estado, "a") == 0)
                        nuevaParticion.byteInicio = mbr->particion1.byteInicio + mbr->particion1.size;
                    else
                        nuevaParticion.byteInicio = sizeof(Mbr);
                    //se busca la primer particion ocupada
                    switch (buscarParticion(mbr, 2))
                    {
                    case 3:
                        if(strcmp(mbr->particion1.estado, "a") == 0)
                            espacioDisponible = mbr->particion3.byteInicio - mbr->particion1.byteInicio - mbr->particion1.size;
                        else
                            espacioDisponible = mbr->particion3.byteInicio - sizeof(Mbr);
                        //la nueva particion si cabe dentro del espacio disponible
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion2 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    case 4:
                        if(strcmp(mbr->particion1.estado, "a") == 0)
                            espacioDisponible = mbr->particion4.byteInicio - mbr->particion1.byteInicio - mbr->particion1.size;
                        else
                            espacioDisponible = mbr->particion4.byteInicio - sizeof(Mbr);
                        //la nueva particion si cabe dentro del espacio disponible
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion2 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    default:
                        if(strcmp(mbr->particion1.estado, "a") == 0)
                            espacioDisponible = mbr->size - mbr->particion1.byteInicio - mbr->particion1.size;
                        else
                            espacioDisponible = mbr->size - sizeof(Mbr);
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion2 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    }
                }
                //la particion 3 es la nueva particion inactiva
                if(strcmp(mbr->particion3.estado, "i") == 0 && !flag)
                {
                    printf("particion 3 inactiva\n");
                    //si la particion 2 esta activa el byteInicio sera establecido
                    if(strcmp(mbr->particion2.estado, "a") == 0)
                        nuevaParticion.byteInicio = mbr->particion2.byteInicio + mbr->particion2.size;
                    else
                        if(strcmp(mbr->particion1.estado, "a") == 0)
                            nuevaParticion.byteInicio = mbr->particion1.byteInicio + mbr->particion1.size;
                        else
                            nuevaParticion.byteInicio = sizeof(Mbr);
                    //se busca la primer particion ocupada
                    switch (buscarParticion(mbr, 3))
                    {
                    case 4:
                        if(strcmp(mbr->particion2.estado, "a") == 0)
                            espacioDisponible = mbr->particion4.byteInicio - mbr->particion2.byteInicio - mbr->particion2.size;
                        else
                            if(strcmp(mbr->particion1.estado, "a") == 0)
                                espacioDisponible = mbr->particion4.byteInicio - mbr->particion1.byteInicio - mbr->particion1.size;
                            else
                                espacioDisponible = mbr->particion4.byteInicio - sizeof(Mbr);
                        //la nueva particion si cabe dentro del espacio disponible
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion3 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    default:
                        if(strcmp(mbr->particion2.estado, "a") == 0)
                            espacioDisponible = mbr->size - mbr->particion2.byteInicio - mbr->particion2.size;
                        else
                            if(strcmp(mbr->particion1.estado, "a") == 0)
                                espacioDisponible = mbr->size - mbr->particion1.byteInicio - mbr->particion1.size;
                            else
                                espacioDisponible = mbr->size - sizeof(Mbr);
                        if(espacioDisponible >= nuevaParticion.size)
                        {
                            mbr->particion3 = nuevaParticion;
                            flag = true;
                        }
                        break;
                    }
                }
                //la particion 4 es la nueva particion inactiva
                if(strcmp(mbr->particion4.estado, "i") == 0 && !flag)
                {
                    printf("particion 4 inactiva\n");
                    //si la particion 3 esta activa el byteInicio sera establecido
                    if(strcmp(mbr->particion3.estado, "a") == 0)
                        nuevaParticion.byteInicio = mbr->particion3.byteInicio + mbr->particion3.size;
                    else
                        if(strcmp(mbr->particion2.estado, "a") == 0)
                            nuevaParticion.byteInicio = mbr->particion2.byteInicio + mbr->particion2.size;
                        else
                            if(strcmp(mbr->particion1.estado, "a") == 0)
                                nuevaParticion.byteInicio = mbr->particion1.byteInicio + mbr->particion1.size;
                            else
                                nuevaParticion.byteInicio = sizeof(Mbr);
                    if(strcmp(mbr->particion3.estado, "a") == 0)
                        espacioDisponible = mbr->size - mbr->particion3.byteInicio - mbr->particion3.size;
                    else
                        if(strcmp(mbr->particion2.estado, "a") == 0)
                            espacioDisponible = mbr->size - mbr->particion2.byteInicio - mbr->particion2.size;
                        else
                            if(strcmp(mbr->particion1.estado, "a") == 0)
                                espacioDisponible = mbr->size - mbr->particion1.byteInicio - mbr->particion1.size;
                            else
                                espacioDisponible = mbr->size - sizeof(Mbr);
                    if(espacioDisponible >= nuevaParticion.size)
                    {
                        mbr->particion4 = nuevaParticion;
                        flag = true;
                    }
                }
                //se logro insertar la nueva particion
                if(flag)
                {
                    //se actualiza el MBR dentro del disco especificado en la ruta
                    setMbr(nuevoFormato->path, mbr);
                    //si la particion ingresada es extendida se debe de crear el EBR
                    if(strcmp(nuevaParticion.tipo, "e") == 0)
                    {
                        printf("PARTICION EXTENDIDA\n");
                        Ebr *ebr = (Ebr *) malloc(sizeof(Ebr));
                        strcmp(ebr->ajuste, "wf");
                        ebr->byteInicio = nuevaParticion.byteInicio;
                        strcmp(ebr->estado, "i");
                        strcpy(ebr->nombre, "-");
                        ebr->siguiente = -1;
                        ebr->size = 0;
                        setEbr(nuevoFormato->path, ebr);
                        mostrarEbr(nuevoFormato->path);
                    }
                    printf("nuevoFormato->path: .%s.\n", nuevoFormato->path);
                    printf("mostrarMbr\n");
                    mostrarMbr(nuevoFormato->path, 0);
                    mostrarMbr(nuevoFormato->path, 1);
                    mostrarMbr(nuevoFormato->path, 2);
                    mostrarMbr(nuevoFormato->path, 3);
                    mostrarMbr(nuevoFormato->path, 4);
                    printf("La particion \"%s\" se CREO con exito\n", nuevaParticion.nombre);
                }
                else
                    printf("La capacidad dentro del disco se han sobrepasado\n");
            }
            else
                if(strcmp(nuevoFormato->tipo, "l") == 0)
                {
                    if(hayExtendida(nuevoFormato->path))
                    {
                        printf("PARTICION LOGICA\n");
                        mostrarEbr(nuevoFormato->path);
                        Ebr *actual = getEbr(nuevoFormato->path);
                    }
                    else
                    {
                        printf("No existe una particion extendida dentro de este disco\n");
                    }
                }
                else
                    printf("El tipo de particion ingresado no es correcto\n");
        }
    printf("formato->size: %d\n", nuevoFormato->size);
    printf("formato->path: %s\n", nuevoFormato->path);
    printf("formato->unit: %s\n", nuevoFormato->unit);
    printf("formato->tipo: %s\n", nuevoFormato->tipo);
    printf("formato->ajuste: %s\n", nuevoFormato->ajuste);
    printf("formato->borrar: %s\n", nuevoFormato->borrar);
    printf("formato->nombre: %s\n", nuevoFormato->nombre);
    printf("formato->agregar: %d\n", nuevoFormato->agregar);
}

int main(int argc, char *argv[])
{
    char comando[10];
    char parametros[100];
    while(1)
    {
        for(int i = 0; i<=100; i++)
            parametros[i] = '\0';
        for(int i = 0; i<=10; i++)
            comando[i] = '\0';
        printf("INGRESE UN COMANDO\n");
        fflush(stdin);
        scanf("%s",&comando);
        scanf("%[^\n]",&parametros);
        for(int i = 0; i <= 10; i++)
            comando[i] = tolower(comando[i]);
        if (strcmp(comando, "salir") == 0)
            return 0;
        if(strcmp(comando,"mkdisk") == 0)
        {
            mkdisk(parametros);
        }
        else
            if(strcmp(comando, "rmdisk") == 0)
            {
                rmdisk(parametros);
            }
            else
                if(strcmp(comando, "fdisk") == 0)
                {
                    fdisk(parametros);
                }//fin fdisk
    }
    return 0;
    QCoreApplication a(argc, argv);
    return a.exec();
}
