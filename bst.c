#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

//	Input: ’size’: size of an array
//	Output: a pointer of type BStree,
//				i.e. a pointer to an allocated memory of BStree_struct type
//	Effect: dynamically allocate memory of type BStree_struct
//			allocate memory for a Node array of size+1 for member tree_nodes
//			allocate memory for an unsigned int array of size+1 for member is_free
//			set entry i of is_free to i
//			set member size to ’size’
//			set member top to 1 
//			set member root to 0 
BStree bstree_ini(int size) {
	BStree bst;
	int i;

	bst = (BStree) malloc( sizeof(BStree_struct) );
	bst->tree_nodes = (Node *) malloc( (size+1)*sizeof(Node) );
	bst->free_nodes = (unsigned int *) malloc( (size+1)*sizeof(unsigned int) );
	bst->size = size;
	for (i=0; i<=size; i++) {
		bst->free_nodes[i] = i;
	}
	bst->top = 1;
	bst->root = 0;
	return bst;
}

//	Input: ’bst’: a binary search tree
//					’key’: a Key
//					’data’: a Data
//	Effect: get a free tree_nodes index from free_nodes[top]
//	        ’key’ with ’data’ is assigned into the free tree_nodes
//					left and right are assigned to 0
static int new_node(BStree bst, Key key, Data data) {
	int index;
	if (bst->top > bst->size) {
		return 0;
	}
	index = bst->free_nodes[bst->top];
	bst->top++;
	bst->tree_nodes[index].key = key;
	bst->tree_nodes[index].data = data;
	bst->tree_nodes[index].left = 0;
	bst->tree_nodes[index].right = 0;
	return index;
}

//	Input: ’bst’: a binary search tree
//					’key’:  a Key
//					’data’: a Data
//					'index': pointer to current tree node index
//	Effect: ’key’ with ’data’ is inserted into ’bst’
//					if ’key’ is already in ’bst’, do nothing
static void bst_insert(BStree bst, int *index, Key key, Data data) {
	if (*index == 0 ) {
		*index = new_node(bst, key, data);
		return;
	}
	else {
		int i=*index;
		int comp = key_comp( bst->tree_nodes[i].key, key );
		if ( comp < 0 ) {
			bst_insert(bst, &bst->tree_nodes[i].right, key, data);
		}
		else if ( comp > 0 ) {
			bst_insert(bst, &bst->tree_nodes[i].left, key, data);
		}
		else
			return;
	}
}

//	Input: ’bst’: a binary search tree
//					’key’:  a Key
//					’data’: a Data
//	Effect: ’key’ with ’data’ is inserted into ’bst’
//					if ’key’ is already in ’bst’, do nothing
void bstree_insert(BStree bst, Key key, Data data) {
	bst_insert(bst, &bst->root, key, data);
}

//	Input: ’bst’: a binary search tree
//					’key’:  a Key
//					'index': pointer to current tree node index
//	Effect: delete the tree node who's key is ’key’ from ’bst’
//					if ’key’ is not ’bst’, do nothing
static void bst_delete(BStree bst, int *index, Key key) {
	int ind = *index;
  if (ind == 0) {
    return;
  }
  int comp = key_comp(key, bst->tree_nodes[ind].key);
  if ( comp < 0 )
    bst_delete(bst, &bst->tree_nodes[ind].left, key);
  else if ( comp > 0 )
    bst_delete(bst, &bst->tree_nodes[ind].right, key);
  else { //comp==0
    if (bst->tree_nodes[ind].left == 0) {
      *index = bst->tree_nodes[ind].right;
    }
    else if (bst->tree_nodes[ind].right == 0) {
      *index = bst->tree_nodes[ind].left;
    }
    else {
      int *index1=&bst->tree_nodes[ind].right;
			int ind1 = *index1;
			while ( bst->tree_nodes[ind1].left != 0 ) {
        index1 = &bst->tree_nodes[ind1].left;
        ind1 = *index1;
      }
			*index1 = bst->tree_nodes[ind1].right;
      *index = ind1;
      bst->tree_nodes[ind1].left = bst->tree_nodes[ind].left;
      bst->tree_nodes[ind1].right = bst->tree_nodes[ind].right;
    }
    bst->top--;
		bst->free_nodes[bst->top] = ind;
  }
}

//	Input: ’bst’: a binary search tree
//					’key’:  a Key
//	Effect: delete the tree node who's key is ’key’ from ’bst’
//					if ’key’ is not ’bst’, do nothing
void bstree_delete(BStree bst, Key key) {
	bst_delete(bst, &bst->root, key);
}

// Input: ’bst’: a binary search tree
//        'index': index of current tree node
// Effect: print all the nodes in bst using in order traversal
static void bst_traversal(BStree bst, int index) {
	if ( index == 0 ) {
		return;
	}
	else {
		bst_traversal(bst, bst->tree_nodes[index].left);
		print_node( bst->tree_nodes[index] );
		bst_traversal(bst, bst->tree_nodes[index].right);
	}
}

// Input: ’bst’: a binary search tree
// Effect: print all the nodes in bst using in order traversal
void bstree_traversal(BStree bst) {
  bst_traversal(bst, bst->root);
}

// Input: ’bst’: a binary search tree
// Effect: free all dynamic allocated memory for bst
void bstree_free(BStree bst) {
	free( bst->tree_nodes );
	free( bst->free_nodes );
	free( bst );
}

//	Input: 'bst': a binary search tree
//	       'key':  a Key
//	       'index': pointer to current tree node index
//	Output: Address of key's associated data or NULL
//	Effect: Searches bst for 'key' and returns the address of 
//		the key's associated data. If the key is not in bst
//		returns NULL
static Data *bst_search(BStree bst, int index, Key key) {
	// If index is 0, then key is not in the tree
	if (index == 0){
		return NULL;	
	}
	else {
		// Traverse the tree recursively and search for the key
		int comp = key_comp( bst->tree_nodes[index].key, key );

		// Inputted key is larger than key at current node
		// Therefore, call again with right subtree
		if ( comp < 0 ) {
			bst_search(bst, bst->tree_nodes[index].right, key);
		}
		// Inputted key is smaller than key at current node
		// Therefore, call again with left subtree
		else if ( comp > 0 ) {
			bst_search(bst, bst->tree_nodes[index].left, key);
		}
		// Key at current node is the inputted key
		else
			return &bst->tree_nodes[index].data;
	}
}

//	Input: 'bst': a binary search tree
//	       'key':  a Key
//	Output: Address of key's associated data or NULL
//	Effect: Searches bst for 'key' and returns the address of 
//		the key's associated data. If the key is not in bst
//		returns NULL
Data *bstree_search(BStree bst, Key key) {
	return bst_search(bst, bst->root, key);
}

//	Input: 'bst': a binary search tree
//	       'key':  a Key
//	       'index': pointer to current tree node index  
//	Output: Address of the key of the first node where the data of 
//		the node is greater than or equals to 'data'
//		Returns null if there is no such node
//	Effect: Searches bst for a node with data greater than 
//		or equal to 'data' and returns the associated key
//		If there is no node with enough data, returns NULL
static Key *bst_data_search(BStree bst, int index, Data data) {
	// If index is 0, then there is not a large enough data location in the tree
	if ( index == 0 ) {
		return NULL;
	}
	else {
		Key *key;

		// Perform in order traversal
		// Check left subtree
		key = bst_data_search(bst, bst->tree_nodes[index].left, data);
		// If key is not null then there is a value in the left subtree that is large enough
		// Thus return a pointer to the key
		if (key != NULL)		
			return key;
		// Check current node to see if it has enough data
		if (bst->tree_nodes[index].data >= data)
			return &bst->tree_nodes[index].key;

		// Check right subtree
		key = bst_data_search(bst, bst->tree_nodes[index].right, data);
		// If key is not null then there is a value in the right subtree that is large enough
		// Thus return a pointer to the key
		if (key != NULL)		
			return key;

		return NULL;
	}
}

//	Input: 'bst': a binary search tree
//	       'key':  a Key
//	Output: Address of the key of the first node where the data of 
//		the node is greater than or equals to 'data'
//		Returns null if there is no such node
//	Effect: Searches bst for a node with data greater than 
//		or equal to 'data' and returns the associated key
//		If there is no node with enough data, returns NULL
Key *bstree_data_search(BStree bst, Data data) {
	return bst_data_search(bst, bst->root, data);
}

//	Input: 'bst': a binary search tree
//	       'key': a Key
//	       'pre': a pointer to a pointer to a key
//	       'suc': a pointer to a pointer to a key
//	       'index': pointer to current tree node index 
//	Effect: Sets pre and suc to be pointers to the keys of predecessor and successor of key
//		where key is not in the tree.
//		If key does not have a predecessor or successor, the respective pointer is set to NULL
void bst_pre_suc(BStree bst, Key key, Key **pre, Key **suc, int index){
	// If index is 0 then tree is empty 
	if ( index == 0 ) {
		return;
	}
	else{
		int comp = key_comp( bst->tree_nodes[index].key, key );

		if ( comp < 0 && bst->tree_nodes[index].right != 0) {
			// If value at current node is greater than current predecessor 
			if (*pre != NULL && bst->tree_nodes[index].key > **pre)
				*pre = &bst->tree_nodes[index].key;
			// Go to right subtree
			bst_pre_suc(bst, key, pre, suc, bst->tree_nodes[index].right);
		}	
		else if ( comp > 0 && bst->tree_nodes[index].left != 0) {
			// If value at current node is less than current successor 
			if (*suc != NULL && bst->tree_nodes[index].key < **suc)
				*suc = &bst->tree_nodes[index].key;
			// Go to left subtree
			bst_pre_suc(bst, key, pre, suc, bst->tree_nodes[index].left);
		}
		else{
			// This means the right subtree is empty and key is larger than value at the current node
			// Therefore, currrent node is the predecessor	
			if (*pre != NULL && comp < 0)
				*pre = &bst->tree_nodes[index].key;
			if (*suc != NULL && comp > 0)
				*suc = &bst->tree_nodes[index].key;
		}
	}
}

//	Input: 'bst': a binary search tree
//	       'key': a Key
//	       'pre': a pointer to a pointer to a key
//	       'suc': a pointer to a pointer to a key
//	Effect: Sets pre and suc to be pointers to the keys of predecessor and successor of key
//		where key is not in the tree.
//		If key does not have a predecessor or successor, the respective pointer is set to NULL
void bstree_pre_suc(BStree bst, Key key, Key **pre, Key **suc){
	int index = bst->root;
	// If index is 0 then tree is empty 
	if (index == 0){
		return;
	}
	Key max, min;
	// First see if key has a successor and predecessor
	// Find the max value
	while (	bst->tree_nodes[index].right != 0)
		index = bst->tree_nodes[index].right;
	max = bst->tree_nodes[index].key;
	
	index = bst->root;
	// Find the min value
	while (	bst->tree_nodes[index].left != 0)
		index = bst->tree_nodes[index].left;
	min = bst->tree_nodes[index].key;

	index = bst->root;
	// If the key is larger than the max then the successor is not in the tree
	if (key > max)
		*suc = NULL;
	else
		*suc = &bst->tree_nodes[index].key;

	// If the key is smaller than the min then the predecessor is not in the tree
	if (key < min)
		*pre = NULL;
	else
		*pre = &bst->tree_nodes[index].key;
	
	return bst_pre_suc(bst, key, pre, suc, bst->root);
}



