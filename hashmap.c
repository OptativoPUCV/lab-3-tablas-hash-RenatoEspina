#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if (map == NULL) return;
    if (map->size >= map->capacity * 0.7) enlarge(map);
    long originalIndex = hash(key, map->capacity);
    long index = originalIndex;
    Pair *pair = createPair(key, value);
    while (map->buckets[index] != NULL) {
        if (is_equal(map->buckets[index]->key, key)) {
            map->buckets[index]->value = value;
            free(pair);
            return;
        }
        index = (index + 1) % map->capacity;
        if (index == originalIndex) {
            free(pair);
            return;
        }
    }
    map->buckets[index] = pair;
    map->size++;
}
    

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    if (capacity<1) return NULL;
    HashMap* map=(HashMap*)malloc(sizeof(HashMap));
    map->buckets=(Pair**)malloc(sizeof(Pair*)*capacity);
    map->size=0;
    map->capacity=capacity;
    map->current=-1;
    for(int i=0;i<capacity;i++){
        map->buckets[i]=NULL;
    }
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    if(map==NULL) return;
    long index=hash(key,map->capacity);
    if(map->buckets[index]==NULL) return;
    while(map->buckets[index]!=NULL){
        if(is_equal(map->buckets[index]->key,key)){
            map->buckets[index]->key=NULL;
            map->size--;
        }
        index=(index+1)%map->capacity;
        if(index==hash(key,map->capacity)) break;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    if(map==NULL) return NULL;
    long index=hash(key,map->capacity);
    if(map->buckets[index]==NULL) return NULL;
    while(map->buckets[index]!=NULL){
        if(is_equal(map->buckets[index]->key,key)){
            map->current=index;
            return map->buckets[index];
        }
        index=(index+1)%map->capacity;
        if(index==hash(key,map->capacity)) break;
    }
    return NULL;;
}

Pair * firstMap(HashMap * map) {
    if(map==NULL) return NULL;
    for(int i=0;i<map->capacity;i++){
        if(map->buckets[i]!=NULL){
            map->current=i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if(map==NULL) return NULL;
    for(int i=map->current;i<map->capacity;i++){
        if(map->buckets[i]!=NULL){
            map->current=i;
            return map->buckets[i];
        }
    }
    return NULL;
}
