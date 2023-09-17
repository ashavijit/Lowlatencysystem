#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// lets define cache structure

typedef struct MyCache{
    char key[50];
    char val[50];
    struct MyCache*next;
} MyCache;

typedef struct Cache {
    MyCache* entries[100]; // fixed length of 100
} Cache;

void initCache(Cache*cache){
    for(int i=0;i<100;i++){
        cache->entries[i] = NULL;
    }
}

// add to cache 

void addToCache(Cache*cache,const char *key ,const char *val){
    unsigned int hash = 0;
    for(int i=0;key[i];i++){
        hash+=key[i];
    }
    int idx = hash % 100;

    MyCache* newEntry = malloc(sizeof(MyCache));
    strcpy(newEntry->key,key);
    strcpy(newEntry->val,val);

    cache->entries[idx] = newEntry;
}

const char* getFromMyCache(Cache* cache , const char* key){
    unsigned int hash = 0;
    for(int i=0;key[i];i++){
        hash+=key[i];
    }
    int idx = hash % 100;

    MyCache*curr = cache->entries[idx];
    while(curr != NULL){
        if(strcmp(curr->key,key) == 0){
            return curr->val;
        }
        curr = curr->next;
    }
    return NULL;
}

int main(){
    Cache cache;
    initCache(&cache);
    addToCache(&cache,"name","sahil");
    addToCache(&cache,"age","20");
    addToCache(&cache,"city","delhi");

    const char* val = getFromMyCache(&cache,"name");
    printf("%s\n",val);

    val = getFromMyCache(&cache,"age");
    printf("%s\n",val);

    val = getFromMyCache(&cache,"city");
    printf("%s\n",val);

    return 0;
}