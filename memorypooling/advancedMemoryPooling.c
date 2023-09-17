#include <stdio.h>
#include <stdlib.h>

#define POOL_SIZE 1000

typedef struct Object{
    size_t size;
    char data[];
}Object;

typedef struct MemoryPool{
    char* pool;
    size_t nextIdx;
}MemoryPool;

void initMemoryPool(MemoryPool* pool,size_t poolSz){
    pool->pool = (char*)malloc(poolSz);
    pool->nextIdx = 0;
}
// Allocate an object from the memory pool with a specific size
Object *allocateObject(MemoryPool *pool, size_t objSize){
    if(pool->nextIdx + objSize < POOL_SIZE){
        Object* obj = (Object*)&(pool->pool[pool->nextIdx]);
        obj->size = objSize;
        pool->nextIdx += objSize;
        return obj;
    }
    else{
        return NULL;
    }
}

// Deallocate an object from the memory pool
void deallocateObject(Object *obj){
    // no explicit deallocation needed in this example
}

int main()
{
    MemoryPool pool;
    size_t poolSize = POOL_SIZE * sizeof(char);
    initMemoryPool(&pool, poolSize);

    Object *obj1 = allocateObject(&pool, sizeof(Object) + 10); // Object with 10 bytes of data
    Object *obj2 = allocateObject(&pool, sizeof(Object) + 20); // Object with 20 bytes of data

    if (obj1 != NULL)
    {
        obj1->data[0] = 'H';
        obj1->data[1] = 'e';
        obj1->data[2] = 'l';
        obj1->data[3] = 'l';
        obj1->data[4] = 'o';
        printf("Object 1 Data: %s\n", obj1->data);
    }
    else
    {
        printf("Object 1 allocation failed. Pool exhausted.\n");
    }

    if (obj2 != NULL)
    {
        obj2->data[0] = 'W';
        obj2->data[1] = 'o';
        obj2->data[2] = 'r';
        obj2->data[3] = 'l';
        obj2->data[4] = 'd';
        printf("Object 2 Data: %s\n", obj2->data);
    }
    else
    {
        printf("Object 2 allocation failed. Pool exhausted.\n");
    }


    free(pool.pool); // lets free the pool

    return 0;
}