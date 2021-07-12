#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory.h"
#include "list.h"
#include "datatype.h"
#include "bst.h"

int read_input(FILE* fp, List* list);
void add_or_increment(List list, Key key);

int main(void) {
	char choice[2];
	List *list;
	int  choice_num;

	// Prompt uset to read from stdin or file
	printf("0 to read from stdin, or 1 to read from a file, or anything else to exit: ");
	fgets(choice, sizeof(choice), stdin);
	choice_num = choice[0] - '0';
	if (choice_num != 0 && choice_num != 1)
		return 0;

	// Clear stdin	
	while ((getchar()) != '\n');

	// If stdin is chosen
	if (choice_num == 0){
		if (read_input(stdin, list) == -1)
			return -1;
	}
	// If file is chosen
	else{
		if (read_input(NULL, list) == -1)
			return -1;
	}

	// Display results
	printf("Key = integer, Data = occurrences\n");
	list_print(*list);

	// Free allocated memory
	list_free(*list);
	mem_free();
}

// Input: 'list': a list
//	  'key': a key
// Effect: Searches the inputted list for the inputted key. If the key is present in the list,
//	   increment data by 1, otherwise add it to the list with data = 1
void add_or_increment(List list, Key key){
	Data *data;
	data = list_search(list, key);
	if (data == NULL)
		list_add(list, key, 1);
	else
		(*data)++;
}

// Input: 'fp': a pointer to a file
//	  'list': a pointer to a list
// Output: 0 if functioning properly or 1 if there was an error
// Effect: Reads in lines either from a file or stdin and puts them into a list
int read_input(FILE* fp, List* list){
	unsigned int size = 0;
	// Max integer input of 20 digits
	char input[20], size_input[20], file_name[20];
	char *new_line;
	// Boolean value (1 = true, 0 = false)
	int is_num = 1;
	int len, input_num;

	if (fp == NULL) {
		printf("Enter the file name: ");
		fgets(file_name, sizeof(file_name), stdin);

		// Remove trailing new line character on the file name
		if ((new_line = strchr(file_name, '\n'))!= NULL)
			*new_line = '\0';

		fp = fopen(file_name, "r");
		if(fp == NULL){
			printf("Error opening file\n");
			return -1;
		}
	}

	// Get size of dynamic memory from file/stdin
	if (fp == stdin)
		printf("Enter the size of dynamic memory allocation and deallocation system (must be greater than 20): ");
	fgets(size_input, sizeof(size_input), fp);
	len = strlen(size_input);
	for (int i = 0; i < len-1; i++){
		if (size_input[i] != '0'&&size_input[i] != '1'&&size_input[i] != '2'&&size_input[i] != '3'&&size_input[i] != '4'&&size_input[i] != '5'&&size_input[i] != '6'&&size_input[i] != '7'&&size_input[i] != '8'&&size_input[i] != '9')
			is_num = 0;
	}

	// Repeatedly prompt if bad input
	if(is_num == 0){
		printf("Error: Input for dynamic memory was not a positive integer \n");
		return -1;
	}
	size = atoi(size_input);

	// Size must be atleast 20 to initialize the list
	if (size < 20){
		printf("Not enough memory allocated \n");
		return -1;
	}
	
	mem_ini(size);
	*list = list_ini();

	if (fp == stdin)
		printf("Enter an integer (non-integer to exit): ");
	// Repeatedly prompt for more inputs while adding values to list
	while (fgets(input, sizeof(input), fp)){
		len = strlen(input);
		// Check if entered value is an integer
		for (int i = 0; i < len-1; i++){
			if (input[i] != '0'&&input[i] != '1'&&input[i] != '2'&&input[i] != '3'&&input[i] != '4'&&input[i] != '5'&&input[i] != '6'&&input[i] != '7'&&input[i] != '8'&&input[i] != '9'&&input[i]!='-')
				is_num = 0;
			}

		// If value is not an integer then exit
		if (is_num == 0||len == 1){
			break;
		}
		// Convert entered string into an integer
		input_num = atoi(input);

		// Add the inputted number to the list, or if it exists already, increment the data by 1
		add_or_increment(*list, input_num);
		if (fp == stdin)
			printf("Enter an integer (non-integer to exit): ");
	}
	if (fp != stdin)
		fclose(fp);
	return 0;
}
