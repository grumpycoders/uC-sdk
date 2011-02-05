#include <stddef.h>
#include <stdint.h>
#include "malloc.h"

void * sbrk(ptrdiff_t incr);

typedef struct _heap_t {
    void * ptr;
    size_t size;
    struct _heap_t * prev, * next;
} heap_t;

static void * heap_base = NULL;
static heap_t * head = NULL, * tail = NULL;

static heap_t * find_fit(heap_t * head, size_t size) {
    heap_t * prev = head;
    uintptr_t prev_top, next_bot;
    
    while (prev) {
        if (prev->next) {
            prev_top = (uintptr_t) prev->ptr + prev->size;
            next_bot = (uintptr_t) prev->next - prev_top;
            if (next_bot >= size)
                return prev;
        }
        prev = prev->next;
    }
    
    return prev;
}

void * malloc(size_t size) {
    void * ptr = NULL, * heap_ptr;
    heap_t * new, * prev;
    
    size = (size + sizeof(heap_t) + 7) & ~7;
    
    // Nothing's initialized yet ? Let's just initialize the bottom of our heap,
    // flag it as allocated.
    if (!head) {
        if (!heap_base)
            heap_base = sbrk(0);
        heap_ptr = sbrk(size);
        
        if (!heap_ptr)
            return NULL;
        
        ptr = (void *) ((uintptr_t) heap_ptr + sizeof(heap_t));
        head = (heap_t *) heap_ptr;
        head->ptr = ptr;
        head->size = size - sizeof(heap_t);
        head->prev = NULL;
        head->next = NULL;
        tail = head;
        return ptr;
    }
    
    // We *may* have the bottom of our heap that has shifted, because of a free.
    // So let's check first if we have free space there, because I'm nervous
    // about having an incomplete data structure.
    if (((uintptr_t) heap_base + size) < (uintptr_t) head) {
        new = (heap_t *) heap_base;
        ptr = (void *) ((uintptr_t) new + sizeof(heap_t));
        
        new->ptr = ptr;
        new->size = size - sizeof(heap_t);
        new->prev = NULL;
        new->next = head;
        head->prev = new;
        head = new;
        return ptr;
    }
    
    // No luck at the beginning of the heap, let's walk the heap to find a fit.
    prev = find_fit(head, size);
    if (prev) {
        new = (heap_t *) ((uintptr_t) prev->ptr + prev->size);
        ptr = (void *) ((uintptr_t) new + sizeof(heap_t));
        
        new->ptr = ptr;
        new->size = size - sizeof(heap_t);
        new->prev = prev;
        new->next = prev->next;
        new->next->prev = new;
        prev->next = new;
        return ptr;
    }
    
    // Time to extend the size of the heap.
    heap_ptr = sbrk(size);
    if (!heap_ptr)
        return NULL;
    
    ptr = (void *) ((uintptr_t) heap_ptr + sizeof(heap_t));
    new = (heap_t *) heap_ptr;
    new->ptr = ptr;
    new->size = size - sizeof(heap_t);
    new->prev = tail;
    new->next = NULL;
    tail->next = new;
    tail = new;
    return ptr;
}

void * realloc(void * ptr, size_t size) {
    heap_t * prev;
    void * new = NULL;
    
    if (!size && ptr) {
        free(ptr);
        return NULL;
    }
    
    if (!ptr)
        return malloc(size);
    
    size = (size + sizeof(heap_t) + 7) & ~7;
    
    prev = (heap_t *) ((uintptr_t) ptr - sizeof(heap_t));
    
    // New memory block shorter ?
    if (prev->size >= size) {
        prev->size = size;
        if (!prev->next)
            sbrk(ptr + size - sbrk(0));
        
        return ptr;
    }
    
    // New memory block larger
    // Is it the last one ?
    if (!prev->next) {
        new = sbrk(size - prev->size);
        if (!new)
            return NULL;
        
        prev->size = size;
        return ptr;
    }
    
    // Do we have free memory after it ?
    if ((prev->next->ptr - ptr) > size) {
        prev->size = size;
        return ptr;
    }
    
    // No luck.
    new = malloc(size);
    if (!new)
        return NULL;
    
    memcpy(new, ptr, prev->size);
    free(ptr);
    return new;
}

void free(void * ptr) {
    heap_t * cur;
    void * top;
    size_t size;
    
    if (!ptr || !head)
        return;
    
    if (ptr == head->ptr) {
        size = head->size + (size_t) (head->ptr - (void *) head);
        head = head->next;

        if (head) {
            head->prev = NULL;
        } else {
            tail = NULL;
            sbrk(-size);
        }
        
        return;
    }
    
    cur = head;
    for (cur = head; ptr != cur->ptr; cur = cur->next)
        if (!cur->next)
            return;
    
    if (cur->next) {
        cur->next->prev = cur->prev;
    } else {
        tail = cur->prev;
        top = sbrk(0);
        size = (uintptr_t) top - (uintptr_t) cur->prev->ptr + cur->prev->size;
        sbrk(-size);
    }
    
    cur->prev->next = cur->next;
}

__attribute__((weak)) void * __builtin_new(size_t size) { return malloc(size); }
__attribute__((weak)) void __builtin_delete(void * ptr) { free(ptr); }
