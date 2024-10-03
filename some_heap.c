
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "some_heap.h"

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity) {
    heap_t *heap = (heap_t*)malloc(sizeof(heap_t));
    heap -> data = (heap_node_t*)malloc(capacity * sizeof(heap_node_t));
    heap -> size = 0;
    heap -> capacity = capacity;

    return heap;
}

void heap_free(heap_t *heap) {
    if(heap == NULL) {
        return;
    }

    if(heap -> data != NULL) {
        free(heap -> data);
    }
    free(heap);
}

unsigned int heap_size(heap_t *heap) { 
    return heap->size; 
}

unsigned int heap_parent(unsigned int index) {
    if(index == 0) {
        return 0;
    }
    return (index - 1) / 2;
}

unsigned int heap_left_child(unsigned int index) {
    return 2 * index + 1;
}

unsigned int heap_right_child(unsigned int index) {
    return 2 * index + 2;
}

unsigned int heap_level(unsigned int index) {
    int level = 0;

    index++;
    while (index > 1) {
        index /= 2;
        level++;
    }
    return level;
}

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_node_t temp_data = heap -> data[index1];

    if(heap == NULL || index1 < 0 || index2 < 0 || index1 >= heap -> size || index2 >= heap ->size) {
        return;
    }

    heap -> data[index1] = heap -> data[index2];
    heap -> data[index2] = temp_data;
}

void heap_bubble_up(heap_t *heap, int index) {
    int parent_index = heap_parent(index);

    if(heap == NULL || index <= 0 || index >= heap -> size) {
        return;
    }

    while(index > 0 && heap -> data[index].key < heap -> data[parent_index].key) {
        heap_swap(heap, index, parent_index);
        index = parent_index;
        parent_index = heap_parent(index);
    }
}

void heap_bubble_down(heap_t *heap, int index) {
    int left_child = heap_left_child(index), right_child = heap_right_child(index), smallest_value = index;

    if(left_child < heap -> size && heap -> data[right_child].key < heap -> data[smallest_value].key) {
        smallest_value = left_child;
    }

    if(right_child < heap -> size && heap -> data[right_child].key < heap -> data[smallest_value].key) {
        smallest_value = right_child;
    }

    if(smallest_value != index) {
        heap_swap(heap, index, smallest_value);
        heap_bubble_down(heap, smallest_value);
    }

}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity) {
        return;
    }

    heap -> data[heap_size(heap)].key = key; 
    heap -> data[heap_size(heap)].value = data;
    heap -> size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return NULL;
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}
