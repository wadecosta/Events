struct event {
	int day;
	int month;
	int year;
	int doy; /* Day of the Year */
	char description[250];
	char file_name[50];
	int is_active;
};

struct event event_load(char *file_name);

struct event event_create(int day, int month, int year, char *description);
int event_delete(char *file_name); /* Takes in file_name string and sets is_active to 1 (off). Returns 0 if successful */

char** event_list_get();
char* event_generate_name();
char random_char(int index);
int event_comparator(const void *v1, const void *v2);



/* DOY stuff */
int isLeapYear(int year);
int dayOfYear(int month, int day, int year);
