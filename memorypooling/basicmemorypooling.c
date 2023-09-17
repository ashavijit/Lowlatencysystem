#include <stdio.h>
#include <stdlib.h>

#define POOL_SIZE 1000

typedef struct Object
{
    int data;
} Object;

typedef struct MemoryPool
{
    Object *pool;
    int nextIndex;
} MemoryPool;

void initializeMemoryPool(MemoryPool *pool)
{
    pool->pool = (Object *)malloc(sizeof(Object) * POOL_SIZE);
    pool->nextIndex = 0;
}

Object *allocateObject(MemoryPool *pool)
{
    if (pool->nextIndex < POOL_SIZE)
    {
        return &(pool->pool[pool->nextIndex++]);
    }
    else
    {
        return NULL; // Pool is exhausted
    }
}

void deallocateObject(Object *obj)
{
    // no explicit deallocation needed in this example
}

int main()
{
    MemoryPool pool;
    initializeMemoryPool(&pool);

    Object *obj1 = allocateObject(&pool);
    if (obj1 != NULL)
    {
        obj1->data = 42;
        printf("Allocated object with data: %d\n", obj1->data);
        // Object can be reused without freeing
    }
    else
    {
        printf("Pool is exhausted.\n");
    }

    free(pool.pool);

    return 0;
}
