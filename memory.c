#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

static unsigned char *memory;
static BStree bst;

//	Input: 'size': an unsigned int
//	Effect: Initializes an unsigned char array of size elements
//		and initializes a binary search tree
void mem_ini(unsigned int size) {
	memory = (unsigned char *) malloc(size);
	bst = bstree_ini(size/5); // size/5 is large enough
	bstree_insert(bst, 0, size);
}

//	Input: 'size': an unsigned int
//	Output: A void pointer to the first byte allocated or NULL
//	Effect: Allocates size bytes and returns a pointer to the first
//		byte allocated or NULL if there is not enough memory
void *simu_malloc(unsigned int size) {
	void *p;
	int *key, *data, memory_index;
	
	// find a space with enough memory
	key = bstree_data_search(bst, size+4);

	// if key is null then there is not enough memory to allocate
	if (key == NULL){
		printf("----------MEMORY FULL----------\n");
		return NULL;
	}
	
	// find the amount of data free at the key
	data = bstree_search(bst, *key);

	// remove the memory that is now used
	bstree_delete(bst, *key); 
	memory_index = *key;
	bstree_insert(bst, *key+size+4, (*data)-size-4);

	// store the size in the first 4 bytes
	memory[memory_index] = size;

	// return the location of the start of allocated memory
	p = &memory[memory_index + 4];
	return p;
}

//	Input: 'ptr': a void pointer
//	Effect: Put the allocated memory pointed to by ptr back 
//		to be free memory
void simu_free(void *ptr) {
	unsigned int size;
	int key, *extend;

	if (ptr == NULL)
		return;

	size = *((unsigned int*) (ptr-4));
	
	key = (unsigned char *) ptr - 4 - memory;

	// Find the successor and predecessor to the key
	Key **suc, **pre;
	suc = malloc(sizeof(* suc));
	pre = malloc(sizeof(* pre));
	bstree_pre_suc(bst, key, pre, suc);

	// If the key has a successor in the tree and the successor is directly after the key in memory
	// then merge the two nodes together
	if (*suc != NULL && key + 4 + (int) size == **suc){
		extend = bstree_search(bst, **suc);
		bstree_delete(bst, **suc);
		size += *((unsigned int*) extend);	
	}

	// If the key has a predecessor in the tree and the predecessor is directly before the key
	// in memory, then merge the two nodes together
	if (*pre != NULL){
		extend = bstree_search(bst, **pre);
		if (**pre + *extend == key){
			bstree_delete(bst, **pre);
			size += *((unsigned int*) extend);
			key = **pre;
		}
	}

	free(suc);
	free(pre);

	// Insert the memory into the tree
	bstree_insert(bst, key, size+4);
}

// 	Effect: Prints all the free memory blocks
void mem_print(void) {
	bstree_traversal(bst);
}

//	Effect: Frees the memory used for the array and binary search tree
void mem_free(void) {
	free(memory);
	bstree_free(bst);
}
