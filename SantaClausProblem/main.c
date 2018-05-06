#include <stdio.h>  //uvodjenje biblioteke stdio
#include <stdlib.h>   //uvodjenje biblioteke stdlib
#include <pthread.h>  //uvodjenje biblioteke pthread
#include <semaphore.h>  //uvodjenje biblioteke semaphore


int reindeers=0;  /*kreiranje i inicijalizacija promenjive
reindeers na nulu*/
int elves=0;   /*kreiranje i inicijalizacija promenjive
elves na nulu*/

//kreiranje semafora santaSem, reindeerSem,elfTex,mutex
sem_t santaSem, reindeerSem, elfTex, mutex;

//kreiranje nizova niti reindeer,elf,santaT od 200 clanova
pthread_t reindeer[200],elf[200],santaT[200];

/*kreiranje funkcije santaF koja prima i vraca univerzalni
pokazivac*/
void* santaF(void* atr ){
    int i;//kreiranje promenjive i

    sem_wait(&santaSem); //zakljucavanje santaSem
    sem_wait(&mutex); //zakljucavanje mutexa

    /*ako je broj irvasa jedna ili veci od 9 onda se na
    konzoli stampa "Deda mraz pomaze irvasima\n" zatim se
    devet puta poziva reindeerSem i broj irvasa se smanjuje
    za 9*/
    if(reindeers>=9){
        printf("Deda mraz pomaze irvasima\n");
        for(i=0;i<9;i++)sem_post(&reindeerSem);
        reindeers-=9;

    /*Ako je broj patuljaka jednak ili veci od 3 na konzoli
    se stampa "Deda mraz pomaze patuljcima\n" i broj
    patuljaka se smanjuje za 3*/
    }else if(elves>=3){
        printf("Deda mraz pomaze patuljcima\n");
        elves-=3;
    }

    //otkljucavanje mutexa
    sem_post(&mutex);
}

//funkcije reindeerF koja prima i vraca univerzalni pokazivac
void* reindeerF(void* atr){

  //zakljucavanje mutexa
    sem_wait(&mutex);

    //povecavanje broja irvasa za 1
        reindeers++;

        //ako je broj irvasa jednak 9 onda se poziva santaSem
         if(reindeers==9) {
             sem_post(&santaSem);
           }

    //otkljucavanje mutexa
    sem_post(&mutex);

    //stampanje na konzoli rednog broja irvasa
    printf("Dosao je irvas broj %d\n",reindeers);

}

//funkcija elfF koja prima i vraca univerzalni pokazivac
void* elfF(void* atr){

   //zakljucavanje elfTex
    sem_wait(&elfTex);
    //zakljucavanje mutexa
    sem_wait(&mutex);

    //povecanje elves za jedan
    elves++;

    /* ako je broj irvasa jednak tri otkljucavanje se
    semafor santeSem, u suprutnom se otkljucava semafor
    elfTex*/
    if(elves==3){
         sem_post(&santaSem);
    }else {
        sem_post(&elfTex);
    }

    //otkljucavanje mutexa
    sem_post(&mutex);

    //stampanje rednog broja patuljka na konzoli
    printf("Dosao je patuljak broj %d\n",elves);

    //otkljucavanje mutexa
    sem_wait(&mutex);

    /*ako je broj patuljak jednak 0 otkljucavanje se
    semafor elfTex*/
    if(elves==0){
        sem_post(&elfTex);
    }

    //otkljucavanje mutexa
    sem_post(&mutex);
}

/*genericka funkcija elfGF koja prima i vraca
univerzalni pokazivac*/
void* elfGF(void* atr){
    int i; //kreiranje promenjive i

    /*kreiranje devet niti patuljaka
    sa razmakom od 2 sekunde*/
    for(i=0;i<9;i++){
        pthread_create(elf+i,NULL,elfF,NULL);
        sleep(2);
    }
}

/*genericka funkcija reindeerGF koja prima i vraca
univerzalni pokazivac*/
void* reindeerGF(void* atr){
    int i; //kreiranje promenjive i

    /*kreiranje 27 niti irvasa
    sa razmakom od 1 sekunde*/
    for(i=0;i<27;i++){
        pthread_create(reindeer+i,NULL,reindeerF,NULL);
        sleep(1);
    }
}

/*genericka funkcija santaGF koja prima i vraca
univerzalni pokazivac*/
void* santaGF(void* atr){
    int i; //kreiranje promenjive i

      /*kreiranje 6 niti deda mraza
    sa razmakom od 2 sekunde*/
    for(i=0;i<6;i++){
        pthread_create(santaT+i,NULL,santaF,NULL);
        sleep(2);
    }
}

int main()
{
    int i; //kreiranje celobrojne promenjive i

    //kreiranje niti santa, reindeerG i elfG
    pthread_t santa,reindeerG,elfG;

    //inicijalizacija semafor santaSem
    sem_init(&santaSem,0644,0);
    //inicijalizacija semafora reindeerSem
    sem_init(&reindeerSem,0644,0);
    //kreiranje semafora mutex
    sem_init(&mutex,0644,1);
    //kreiranje semafora elfTex
    sem_init(&elfTex,0644,1);

    //kreiranje niti santa
    pthread_create(&santa,NULL,santaGF,NULL);
    //kreiranje niti reindeerG
    pthread_create(&reindeerG,NULL,reindeerGF,NULL);
    //kreiranje niti elfG
    pthread_create(&elfG,NULL,elfGF,NULL);



   //cekanje da se nit elfG zavrsi
    pthread_join(elfG,NULL);
    //cekanje da se reindeerG zavrsi
    pthread_join(reindeerG,NULL);
    //cekanje da se nit santa zavrsi
    pthread_join(santa,NULL);

    //cekanje da se nit elf sa indeksom i zavrsi
    for(i=0;i<9;i++)pthread_join(elf[i],NULL);
    //cekanje da se nit reindeer sa indeksom i zavrsi
    for(i=0;i<27;i++)pthread_join(reindeer[i],NULL);
    //cekanje da se nit santaT sa indeksom i zavrsi
    for(i=0;i<5;i++)pthread_join(santaT[i],NULL);

    //unistavanje semafora santaSem
    sem_destroy(&santaSem);
    //unistavanje semafora reindeerSem
    sem_destroy(&reindeerSem);
    //unistavanje semafora mutex
    sem_destroy(&mutex);
    //unistavanje semafora elfTex
    sem_destroy(&elfTex);
    printf("Pokloni su uspesno dostavljeni!");
    return 0;
}
