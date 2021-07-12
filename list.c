#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "memory.h"

// Output: A pointer of type List to a list node or NULL if there is not enough memory
// Effect: Initializes a list with a dummy node at the beginning and allocates memory for the first node
List list_ini(void) {
	// Allocate memory for dummy node
	List_node *head = (List_node *) simu_malloc( sizeof(List_node) ); 
	// Not enough memeory
	if (head == NULL)
		return NULL;

	head->next = NULL;
	return head;
}

// Input: 'list': a list
//	  'key': a key
// Output: A pointer to the data where the key is located or NULL
// Effect: Searches the inputted list for the inputted key, returning the data at that key's location
//	   or if key is not in the list, returns NULL
Data *list_search(List list, Key key) {
	// Set current node to the one following the head (head is a dummy node)
	List_node *current_node = list->next;
	int *ptr;
	
	// while the current node is not NULL
	while(current_node){
		if (current_node->key == key){
			ptr = &(current_node->data);
			return ptr;
		}
		current_node = current_node->next;
	} 
	return NULL;
}

// Input: 'list': a list
//	  'key': a key
//	  'data': data
// Effect: Adds the key with data to the front of the list if the key is not already in the list.
//	   If key is already in the list, do nothing
void list_add(List list, Key key, Data data) {
	if (list_search(list, key) == NULL){
		List_node *new_node = (List_node *) simu_malloc( sizeof(List_node) );
		if (new_node == NULL)
			return;
		new_node->next = list->next;
		list->next = new_node;
		new_node->key = key;
		new_node->data = data;
	}
}

// Input: 'list': a list
//	  'key': a key
// Effect: Deletes the node in the list with its key equals to key.
// 	   If the node does not exist in the list, do nothing.
void list_delete(List list, Key key) {
	// Ensure node with key is in the list
	if (list_search(list, key) != NULL){
		List_node *current_node = list->next;
		List_node *previous_node = list;

		// Find the node to be removed
		while(current_node->key != key){
			previous_node = current_node;
			current_node = current_node->next;
		}

		// Remove the node
		previous_node->next = current_node->next;

		// Free memory for the node
		simu_free(current_node);
	}
}

// Input: 'list': a list
// Effect: Linearly traverses the list and prints each node's key and data
void list_print(List list) {
	List_node *current_node = list->next;
	while(current_node){
		print_list_node(current_node);
		current_node = current_node->next;
	} 
}

// Input: 'list': a list
// Effect: Frees all the dynamically allocated memory of the list
void list_free(List list) {
	List_node *current_node = list;
	List_node *next_node;
	while(current_node){
		next_node = current_node->next;
		simu_free(current_node);
		current_node = next_node;
	} 

}
