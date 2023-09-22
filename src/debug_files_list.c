#include <stdio.h>
#include <dirent.h>

int main(void) {
   DIR *dr;
   struct dirent *en;
   dr = opendir("../data/."); //open all or present directory
   if (dr) {
      while ((en = readdir(dr)) != NULL) {
         printf("%s\n", en->d_name); //print all directory name
	 
	 /* if((en->d_name[-2] == 'n')) {
	 	printf("YOLO\n");
	} */
      }
      closedir(dr); //close all directory
   }
   return(0);
}
