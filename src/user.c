#include <json-c/json.h>
#include <string.h>
#include <stdlib.h>

#include "user.h"

struct user user_load_offine() {
	struct user u;

	char *user_name;
        json_object *root = json_object_from_file("../settings.json");

        json_object *name = json_object_object_get(root, "name");

        user_name = strdup(json_object_get_string(name));

        json_object_put(root);

        strcpy(u.name, user_name);

        free(user_name);

	return u;
}
