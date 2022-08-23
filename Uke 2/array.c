#include <stdio.h>

char lag_navn(){
    //char name[10];
    char *name = malloc(10);
    if(name == NULL){
        printf('feil i malloc\n');
        return Null;
    }

    strncpy(buf, "IN2140", 10);
    return name;
}

/*
char lag_navn(){
    char name[10];

    strncpy(buf, "IN2140", 10);
    return name;
}
*/
void fyll_inn(charr *buf,int size){
    strncpy(buf, "IN2140", size)
}

int main(int argc, char const *argv[])
{
    char name[10];
    fyll_inn(name, 10)
    printf("name: %s\n", name);
    /*
    char *p = name;

    printf("name: %p, p: %p\n", name, p)
    *p = 'i';
    *(p + 1) = 'n'
    *(p + 2) = 0;

    name[0] = "I";
    name[1] = "N";
    name[2] = "2";
    name[3] = "1";
    name[4] = "4";
    name[5] = "0";
    name[6] = "\0";
    */
    char *name = lag_navn();
    printf("name: %s\n", name);

    return 0;
}

