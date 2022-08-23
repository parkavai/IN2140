#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Trengs for systemkallene for nettverksoperasjon
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>

static float loss_probability = 0.0f;

void set_loss_probability( float x )
{
    loss_probability = x;
}

void check_if_sent(){
    srand48(1);
    for(int i = 0;i < 10; ++i){
        float rnd = drand48();
        fprintf(stderr, "Random er satt %f\n", rnd);
    }
}

int main(int argc, char const *argv[])
{
    float loss_prop = atof(argv[1]);
    if(loss_prop > 100 || loss_prop < 0){
        printf("Loss propability must be set between 0 and 100");
        exit(EXIT_FAILURE);
    }
    printf("Argumentet er konvertert til %f\n", loss_prop/100);
    set_loss_probability(loss_prop/100);
    printf("Loss_propability: %f\n", loss_probability);
    srand48(time(NULL));
    for(int i = 0;i < 10; ++i){
        float rnd = drand48();
        fprintf(stderr, "Random er satt %f\n", rnd);
        if(rnd < loss_probability){
            printf("Pakke tapt\n");
        }
        else{
            printf("Pakken gik ikke tapt\n");
        }
    }
    return 0;
}
