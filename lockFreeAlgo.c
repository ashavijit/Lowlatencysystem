#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_MESSAGES 1000000

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

_Atomic(Node *) head = NULL;
_Atomic(Node *) tail = NULL;
/*
_Atomic means that the variable is accessed atomically, that means that no other thread can access it while it is being accessed by another thread.
*/

void enqueue(int value)
{
    Node *newNode = malloc(sizeof(Node));
    if (!newNode)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = NULL;

    while (1)
    { 
        Node *last = atomic_load(&tail); // atomic_load() is used to load the value of the atomic variable
        Node *next = atomic_load(&last->next); // atomic_store() is used to store the value of the atomic variable

        if (last == atomic_load(&tail))
        {
            if (next == NULL)
            {
                if (atomic_compare_exchange_strong(&last->next, &next, newNode))
                {
                    atomic_compare_exchange_strong(&tail, &last, newNode);
                    return;
                }
            }
            else
            {
                atomic_compare_exchange_strong(&tail, &last, next);
            }
        }
    }
}

/*                  ___ENQUEUE___
    1. Create a new node and set its data value to the value passed as an argument.
    2. Set the next pointer of the new node to NULL.
    3. Set the next pointer of the last node to the new node.
    4. Set the tail pointer to the new node.


                   ___DEQUEUE___
    1. Get the first node from the head pointer.
    2. Get the last node from the tail pointer.
    3. Get the next node from the first node.
    4. If the first node is the same as the head node, then:
        a. If the first node is the same as the last node, then:
            i. If the next node is NULL, then return -1.
            ii. Else, set the tail pointer to the next node.
        b. Else, set the value of the first node to the next node.
    5. Else, repeat from step 1.

    

*/

int dequeue()
{
    while (1)
    {
        Node *first = atomic_load(&head);
        Node *last = atomic_load(&tail);
        Node *next = atomic_load(&first->next);

        if (first == atomic_load(&head))
        {
            if (first == last)
            {
                if (next == NULL)
                {
                    return -1; // Queue is empty
                }
                atomic_compare_exchange_strong(&tail, &last, next);
            }
            else
            {
                int value = next->data;
                if (atomic_compare_exchange_strong(&head, &first, next))
                {
                    free(first);
                    return value;
                }
            }
        }
    }
}

void *messageProcessor(void *arg)
{
    int thread_id = *((int *)arg);
    for (int i = 0; i < NUM_MESSAGES / NUM_THREADS; i++)
    {
        enqueue(thread_id * NUM_MESSAGES / NUM_THREADS + i);
        int message = dequeue();
        printf("Thread %d processed message: %d\n", thread_id, message);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, messageProcessor, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
