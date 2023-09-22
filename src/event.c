#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <json-c/json.h>

#include "event.h"

#define MAX_NAME_SIZE 30

struct event event_load(char *file_name) {
	struct event e;

	json_object *n_root = json_object_from_file(file_name);

        struct json_object *date_obj;
        if (json_object_object_get_ex(n_root, "date", &date_obj)) {
                struct json_object *month_obj;
                struct json_object *day_obj;
                struct json_object *year_obj;
		struct json_object *doy_obj;
                if (json_object_object_get_ex(date_obj, "month", &month_obj)) {
                        int month = json_object_get_int(month_obj);
                        e.month = month;
                }
                if(json_object_object_get_ex(date_obj, "day", &day_obj)) {
                        int day = json_object_get_int(day_obj);
                        e.day = day;
                }
                if(json_object_object_get_ex(date_obj, "year", &year_obj)) {
                        int year = json_object_get_int(year_obj);
                        e.year = year;
                }
		if(json_object_object_get_ex(date_obj, "doy", &doy_obj)) {
			int doy = json_object_get_int(doy_obj);
			e.doy = doy;
		}


                else {
                        fprintf(stderr, "Failed to retrieve 'month' field\n");
                }
        }
        else {
                fprintf(stderr, "No existing data exist. Please create an event.\n");
		
		int day, month, year;
		char description[100];

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
                e = event_create(day, month, year, description);

		/* Set is_active variable to on */

		return e;
        }

        char *description_text;
        json_object *description = json_object_object_get(n_root, "description");

        description_text = strdup(json_object_get_string(description));
        strcpy(e.description, description_text);

        free(description_text);

	strcpy(e.file_name, file_name);


        json_object_put(n_root);

	e.is_active = 1;
	
	return e;
}

struct event event_create(int day, int month, int year, char *description) {
	struct event e;
	int doy = dayOfYear(month, day, year);

	struct json_object *root = json_object_new_object();
	struct json_object *date = json_object_new_object();


	/* Populate the date object */
	json_object_object_add(date, "day", json_object_new_int(day));
	json_object_object_add(date, "month", json_object_new_int(month));
	json_object_object_add(date, "year", json_object_new_int(year));
	json_object_object_add(date, "doy", json_object_new_int(doy));


	/* Add date and description to the root object */
	json_object_object_add(root, "date", date);
	json_object_object_add(root, "description", json_object_new_string(description));

	/* Serialize the JSON string to a string */
	const char *json_str = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY);

	char file_name[MAX_NAME_SIZE] = "../data/";
	char *generated_name = event_generate_name();
	if (generated_name != NULL) {
		strcat(file_name, generated_name);
        	free(generated_name); // Don't forget to free the allocated memory
    	} 
	else {
        	printf("Memory allocation error\n");
    	}

	/* Add .JSON to file_name */
	strcat(file_name, ".json");
	printf("%s\n", file_name);

	/* Write the JSON string to a file */
	FILE *file = fopen(file_name, "w");
	if(file) {
		fputs(json_str, file);
		fclose(file);
	}
	else {
		fprintf(stderr, "Error opening file for writing\n");
	}

	json_object_put(root);


	e.day = day;
	e.month = month;
	e.year = year;
	e.doy = doy;
	
	strcpy(e.description, description);
	strcpy(e.file_name, file_name);

	e.is_active = 1;

	return e;
}

int event_delete(char *file_name) {
	
	char *p;

	if(asprintf(&p, "rm %s", file_name) != -1) {
		system(p);
		free(p);
	}

	return 0;
}

char** event_list_get() {
	char** list = malloc(10 * sizeof(char*));

	DIR *dr;
   	struct dirent *en;
   	dr = opendir("../data/."); //open all or present directory
   	if (dr) {
		int count = 0;
      		while ((en = readdir(dr)) != NULL && count < 10) {
         		//printf("%s\n", en->d_name); //print all directory name

			list[count] = malloc((strlen(en->d_name) + 1) * sizeof(char));

			if(strlen(en->d_name) > 3) {
				strcpy(list[count], en->d_name);
				count++;
			}
      		}

      		closedir(dr); //close all directory
   	}

  	return list;
}

int event_comparator(const void *v1, const void *v2) {
	
	/* Ascending order (closer dates first */
	/*const struct event *p1 = (struct event *)v1;
	const struct event *p2 = (struct event *)v2;
	
	if(p1->year > p2->year) {
		return +1;
	}
	else if(p1->year == p2->year) {
		if(p1->doy > p2->doy) {
			return +1;
		}
	}
	else {
		return 0;
	}*/

	const struct event *p1 = (const struct event *)v1;
	const struct event *p2 = (const struct event *)v2;

	if(p1->year > p2->year) {
		return 1;
	}
	else if(p1->year < p2->year) {
		return -1;
	}
	else {
		if(p1->doy > p2->doy) {
			return 1;
		}
		else if(p1->doy < p2->doy) {
			return -1;
		}
		else {
			return 0;
		}
			
	}
}

char* event_generate_name() {
	srand(time(NULL));
	char* str = (char*)malloc(17);
	if(str == NULL) {
		return NULL;
	}

	int i, index;
	for (i = 0; i < 16; i++) {
   		index = rand() % 62;
   		str[i] = random_char(index);
	}

	//str[i] = '\0';//We do not forget '\0' for the string termination :D

	return str;
}

char random_char(int index) {
	char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	return charset[index];
}

/* Dealing with dates */
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int dayOfYear(int month, int day, int year) {
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isLeapYear(year)) {
        daysInMonth[2] = 29;
    }

    int dayNumber = 0;
    for (int i = 1; i < month; i++) {
        dayNumber += daysInMonth[i];
    }

    dayNumber += day;

    return dayNumber;
}
