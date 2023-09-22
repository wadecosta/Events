#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "user.h"
#include "event.h"

#define EVENT_LENGTH_MAX 50
#define INPUT_LENGTH_MAX 50


int main(int argc, char **argv) {
	
	printf("Welcome to Rebity!\n");
	
	struct user u1 = user_load_offine();

	int event_length_size = 0;
	struct event event_array[EVENT_LENGTH_MAX];

	char** event_list = event_list_get();
 	
	/* for(int i=0; event_list[i] != NULL; i++) {
		printf("Got here\n");
		printf("%s\n", event_list[i]);
	} */

	for (int i = 0; event_list[i] != NULL; i++) {
	
		char json_folder_location[50] = "../data/";
		strcat(json_folder_location, event_list[i]);

		struct event e = event_load(json_folder_location);
		//e.id = i;
		event_array[i] = e;

		event_length_size++;

		/* Free my homie */
        	free(event_list[i]);
    	}
    	free(event_list);

	char input[INPUT_LENGTH_MAX];
	while(1) {
		printf("> ");

		if(fgets(input, INPUT_LENGTH_MAX, stdin) == NULL) {
			break;
		}

		if(strncmp(input, "quit\n", 5) == 0) {
			break;
		}
		else if(strncmp(input, "create\n", 7) == 0) {
			int day, month, year;
			char description[100];

			printf("Please enter the following to create a new event.\n");
			printf("\nDay : ");
			scanf("%d", &day);

			printf("\nMonth : ");
			scanf("%d", &month);

			printf("\nYear : ");
			scanf("%d", &year);

			printf("\nDescription : ");

			/* Eat the trailing charactor */
			getchar();
			if(fgets(description, sizeof(description), stdin) != NULL) {
				puts(description);
			}

			/* Remove the trailing \n */
			description[strcspn(description, "\n")] = 0;

			
			struct event e = event_create(day, month, year, description);
			event_array[event_length_size] = e;
			event_length_size++;
		}
		else if(strncmp(input, "delete\n", 7) == 0) {
			printf("Enter id to delete: ");
			int id;
			scanf("%d", &id);
			
			if(id > event_length_size-1) {
				printf("Out of bounds.\n");
			}
			else {
				printf("%s\n", event_array[id].file_name);
				event_delete(event_array[id].file_name);
			}

			getchar();
			
		}
		else if(strncmp(input, "print\n", 6) == 0) {

			qsort(event_array, event_length_size, sizeof(struct event), event_comparator);

			printf("size: %d\n", event_length_size);
			
			for(int i = 0; i < event_length_size; i++) {
					printf("._______________.\n");
					printf("|-> Id		: %d\n", i);
                			printf("|-> Description : %s\n", event_array[i].description);
                			printf("|-> Date        : %d-%d-%d\n", event_array[i].year, event_array[i].month, event_array[i].day);
					printf("|-> File Name   : %s\n", event_array[i].file_name);
					printf("|-> Is Active   : %d\n", event_array[i].is_active);
					printf("._______________.\n");
			}
		}
		else if(strncmp(input, "help\n", 5) == 0) {
			printf("Help Menu:\n");
			printf("Type:\tCreate to create a new event.\n");
			printf("Type:\tDelete to delete an event.\n");
			printf("Type:\tPrint to print out all events.\n");
			printf("Type:\tHelp to bring up this menu.\n");
			printf("Type:\tquit to quit this program.\n");
		}
		else {
			input[strcspn(input, "\n")] = 0;
			printf("%s - is not a valid input\n", input);
		}
	}

	return 0;
}
