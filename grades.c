
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linked-list.h>

/*======================Structs and Constants================================*/
/* We use this struct to keep track of all the student's courses and grades
 * This struct is to make things easier to implement and used localy */
static struct course{
	char *course_name;
	int course_grade;
};

struct grades{
	char *name;
	int ID;
	struct *course;

	};

/*======================SFunctions Declarations==============================*/
static int clone_grade(void *element, void **output);
static void destroy_grades(void *element);
static int clone_course(void *element, void **output);
static void destroy_course(void *element);

/* We want to use linked lists with both structs above.
 * Therfore, we will first implement needed function for linked-list.
 * Since we use them localy, they are static. */

//For struct grades
/**  
    * @brief: This function will clone element of struct grades to output.
    * @param element: The element we wish to copy.
    * @param output: A pointer to the copy element to.
    * @return: 0 if succeeded, -1 if failed.
*/
static int clone_grade(void *element, void **output){
	//cast element to relevant struct
	struct grades *clone;
	clone = (struct grades*) element;	
	
	//Make new room in the memory
	*output = (struct grades*)malloc(sizeof(clone));
	if(*output == NULL){
		fprintf(stderr,"Malloc failed\n");
        return -1;
	}

	//Cloning part
	//Notice that we need to allocate memory for tha name too.
	*output->name = (char*)malloc(sizeof(char) * strlen(clone->*name));
	if(*output->name == NULL){
		fprintf(stderr,"Malloc failed\n");
        free(*output);	//Output was already allocated
        return -1;
	}
	strcpy(*output->*name,clone->*name); 
	*output->ID = clone->ID;
	*output->course = clone->course;
}


/**  
    * @brief: This function will free all mallocs and destroy element.
    * @param element: The element we wish to destroy.
*/
static void destroy_grades(void *element){
	//Check if element exists
	if (element == NULL){
		return;
	}

	struct grades *clone;
	clone = (struct grades*) element;

	free(clone->name);
	free(clone);

}

//For struct course
/**  
    * @brief: This function will clone element of struct course to output.
    * @param element: The element we wish to copy.
    * @param output: A pointer to the copy element to.
    * @return: 0 if succeeded, -1 if failed.
*/
static int clone_course(void *element, void **output){
	//cast element to relevant struct
	struct course *clone;
	clone = (struct course*) element;	
	
	//Make new room in the memory
	*output = (struct course*)malloc(sizeof(clone));
	if(*output == NULL){
		fprintf(stderr,"Malloc failed\n");
        return -1;
	}

	//Cloning part
	//Notice that we need to allocate memory for tha name too.
	name_length = strlen(clone->*course_name);
	*output->course_name = (char*)malloc(sizeof(char) * name_length);
	if(*output->course_name == NULL){
		fprintf(stderr,"Malloc failed\n");
        free(*output);	//Output was already allocated
        return -1;
	}
	strcpy(*output->*course_name,clone->*course_name); 
	*output->course_grade = clone->course_grade;
}


/**  
    * @brief: This function will free all mallocs and destroy element.
    * @param element: The element we wish to destroy.
*/
static void destroy_grades(void *element){
	//Check if element exists
	if (element == NULL){
		return;
	}

	struct course *clone;
	clone = (struct course*) element;

	free(clone->course_name);
	free(clone);

}



