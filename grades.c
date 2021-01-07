
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked-list.h"
#include "grades.h"

/*======================Structs and Constants================================*/
/* We use this struct to keep track of all the student's courses and grades
 * This struct is to make things easier to implement and used localy */
struct course{
	char *course_name;
	int course_grade;
	};

struct student{
	char *name;
	int ID;
	struct list *courses;	//Linked-list of courses.
	};

struct grades{
	struct list *students;	//Linked-list of students.
	int num_of_students;
	};

enum{PRINT = 2, FAIL = -1};
/*=======================Functions Declarations==============================*/
int clone_student(void *element, void **output);
void destroy_student(void *element);
int clone_course(void *element, void **output);
void destroy_course(void *element);
struct grades* grades_init();
void grades_destroy(struct grades *grades);
int grades_add_student(struct grades *grades, const char *name, int id);
int grades_add_grade(struct grades *grades, const char *name, int id,
                     int grade);
float grades_calc_avg(struct grades *grades, int id, char **out);
int grades_print_student(struct grades *grades, int id);
static struct student* student_init(char *stu_name, int ID);
static struct course* course_init(char *name, int cour_grade);
static struct student* list_search_student_ID(struct list *students, int ID);
static struct course* search_print_course_name(struct list *courses, char *name
						,int print_or_search);
int grades_print_all(struct grades *grades);
/*int main();*/

/*======================Functions for Linked List============================*/
/* We want to use linked list for grades per student and for a list of students.
 * Therfore, we will first implement needed function for linked-list.
 * Since we use them localy, they are static. */

//For struct student
/**  
    * @brief: This function will clone element of struct studen to output.
    * @param element: The element we wish to copy.
    * @param output: A pointer to the copy element to.
    * @return: 0 if succeeded, -1 if failed.
*/
int clone_student(void *element, void **output){
	//cast element to relevant struct
	struct student *clone;
	clone = (struct student*) element;

	struct student **out;
	out = (struct student**)output;	
	
	//Make new room in the memory
	*out = (struct student*)malloc(sizeof(clone));
	if(*out == NULL){
		fprintf(stderr,"Malloc failed\n");
        return FAIL;
	}

	//Cloning part
	//Notice that we need to allocate memory for tha name too.
	(*out)->name = (char*)malloc(sizeof(char)* strlen(clone->name));
	if((*out)->name == NULL){
		fprintf(stderr,"Malloc failed\n");
        free((*out));	//Output was already allocated
        return FAIL;
	}
	strcpy((*out)->name,clone->name); 
	(*out)->ID = clone->ID;
	(*out)->courses = clone->courses;
	return 0;
}


/**  
    * @brief: This function will free all mallocs and destroy element.
    * @param element: The element we wish to destroy.
*/
void destroy_student(void *element){
	//Check if element exists
	if (element == NULL){
		return;
	}
	struct student *clone;
	clone = (struct student*) element;
	free(clone->name);
	list_destroy(clone->courses);
	free(clone);

}

//For struct course
/**  
    * @brief: This function will clone element of struct course to output.
    * @param element: The element we wish to copy.
    * @param output: A pointer to the copy element to.
    * @return: 0 if succeeded, -1 if failed.
*/
int clone_course(void *element, void **output){
	//cast element to relevant struct
	struct course *clone;
	clone = (struct course*) element;	
	
	struct course **out;
	out = (struct course**)output;
	//Make new room in the memory
	*out = (struct course*)malloc(sizeof(clone));
	if((*out) == NULL){
		fprintf(stderr,"Malloc failed\n");
        return FAIL;
	}

	//Cloning part
	//Notice that we need to allocate memory for tha name too.
	int name_length = strlen(clone->course_name);
	(*out)->course_name = (char*)malloc(sizeof(char) * name_length);
	if((*out)->course_name == NULL){
		fprintf(stderr,"Malloc failed\n");
        free((*out));	//Output was already allocated
        return FAIL;
	}
	strcpy((*out)->course_name,clone->course_name); 
	(*out)->course_grade = clone->course_grade;
	return 0;
}


/**  
    * @brief: This function will free all mallocs and destroy element.
    * @param element: The element we wish to destroy.
*/
void destroy_course(void *element){
	//Check if element exists
	if (element == NULL){
		return;
	}

	struct course *clone;
	clone = (struct course*) element;
	free(clone->course_name);
	free(clone);

}

/*========================Functions for Grades===============================*/
/**
 * @brief Initializes the "grades" data-structure.
 * @returns A pointer to the data-structure, of NULL in case of an error
 */
struct grades* grades_init(){
	struct grades *output;
	output = (struct grades*)malloc(sizeof(*output));
	if(output == NULL){
		fprintf(stderr,"Malloc failed\n");
        return NULL;
	}
	output->students = list_init(clone_student, destroy_student);
	output->num_of_students = 0;
	return output;
}

/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades){
	list_destroy(grades->students);
	free(grades);
	//Type int frees itself.
}

/**
 * @brief: Adds a student with "name" and "id" to "grades"
 * @returns 0 on success and FAIL if failed.
 * @note Failes if "grades" is invalid, or a student with 
 * @the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id){
	/*Check if pointer is NULL or what it contains is NULL(unlikely but just in
	case).*/
	if (grades == NULL){
		return FAIL;
	}
	if (list_search_student_ID(grades->students, id) != NULL){
		return FAIL;
	}
	//Add student
	struct student *add_to_list;
	add_to_list = student_init((char*)name, id);
	if(add_to_list == NULL){
		free(add_to_list);
		return FAIL;
	}

	if(list_push_back(grades->students, add_to_list) != 0){
		free(add_to_list->name);
		free(add_to_list);
		return FAIL;
	}
	free(add_to_list->name);
	free(add_to_list);
	return 0;

}

/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success and FAIL on fail.
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
	//First we will check if it fails
	/*Check if pointer is NULL or what it contains is NULL(unlikely but just in
	case).*/
	if (grades == NULL || 0 > grade || grade > 100){
		return FAIL;
	}
	//Now we will get the requestd student, if exists.
	struct student *student;
	student = list_search_student_ID(grades->students, id);
	if (student == NULL){
		return FAIL;
	}
	//Check if already took course
	if (search_print_course_name(student->courses, (char*)name, 0) != NULL){
		return FAIL;
	}
	//If not exists, create a new course and add it to list.
	struct course *new_course;
	new_course = course_init((char*)name, grade);
	if(new_course == NULL){
		return FAIL;
	}
	if(list_push_back(student->courses, new_course) != 0){
		destroy_course(new_course);
		return FAIL;
	}
	destroy_course(new_course);
	return 0;
}

/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or FAIL on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
float grades_calc_avg(struct grades *grades, int id, char **out){
	if (grades == NULL){
		*out = NULL;
		return FAIL;
	}
	//Now we will get the requestd student, if exists.
	struct student *student;
	student = list_search_student_ID(grades->students, id);
	if (student == NULL){
		*out = NULL;
		return FAIL;
	}

	*out = (char*)malloc(sizeof(char) * strlen(student->name));
	strcpy(*out, student->name);
	//If list size is zero, there is no point to loop over list
	int course_num;
	course_num = list_size(student->courses);
	if(course_num == 0){
		return 0;
	}

	struct node *cursor;
	struct course *element;
	float avg = 0;
	int sum = 0;
	for(cursor = list_begin(student->courses)
		;cursor; cursor = list_next(cursor)){
		element = list_get(cursor);
		//sum all nodes
		sum += element->course_grade;
	}
	avg = (float)(sum) / (float)(course_num);
	return avg;
}

/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id){
	if (grades == NULL){
	return FAIL;
	}

	//Now we will get the requestd student, if exists.
	struct student *student;
	student = list_search_student_ID(grades->students, id);
	if (student == NULL){
		return FAIL;
	}

	printf("%s %d:",student->name,student->ID);
	//If srudent has no courses, return.
	/*Note - If grades and student exists and has courses, 
	search_print_course_name function will not detect other errors so we can use
	it for printing*/ 
	if (list_size(student->courses) == 0){
		printf("\n");
		return 0;
	}
	//We don't care what the name is so we send NULL
	search_print_course_name(student->courses, NULL, PRINT);
	return 0;

}

/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order 
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_all(struct grades *grades){
	if (grades == NULL){
		return FAIL;
		}
	//Check if there are students if not print nothing
	if (list_size(grades->students) == 0){
		return 0;
	}
	struct node *cursor;
	struct student *element;
	for(cursor = list_begin(grades->students);
				cursor; 
				cursor = list_next(cursor)){
		element = list_get(cursor);
		grades_print_student(grades, element->ID);
	}
	return 0;
}


/*======================== HELP FUNCTIONS FOR GRADES =========================*/

/**
 * @brief: This function will search for a student with the same ID in the list.
 * @param students: A pointer to a list of students.
 * @param ID: The ID we wish to search by.
 * @return: A pointer to the student if found, NULL if not found or if failed.
 */
static struct student* list_search_student_ID(struct list *students, int ID){
	if (students == NULL){
		return NULL;
	}

	if(list_size(students) == 0){
		return NULL;
	}
	struct node *cursor;
	struct student *element;
	for(cursor = list_begin(students); cursor; cursor = list_next(cursor)){
		element = list_get(cursor);
		if (element->ID == ID){
			return element;
		}
	}
	//If left for, we did not find, return NULL.
	return NULL;
}

/**
 * @brief: This function will search for a course with the same name in the 
 * @	   list, it can also print all nodes if requierd.
 * @param courses: A pointer to a list of courses.
 * @param name: The name we wish to search by.
 * @param print_or_search: 1 or else if you want to search courses, 0 if you 
 * @					   want to print.
 * @return: A pointer to the course if found, NULL if not found or if failed.
 */
static struct course* search_print_course_name(struct list *courses, char *name
						,int print_or_search){
	if (courses == NULL){
		return NULL;
	}

	if(list_size(courses) == 0){
		return NULL;
	}
	struct node *cursor;
	struct course *element;
	int count = 1; //This counter is use for us to know when is the last course
	int num_of_courses = list_size(courses);
	for(cursor = list_begin(courses); cursor; cursor = list_next(cursor)){
		element = list_get(cursor);
		if(print_or_search == 0){
			//If course has the same name ,found.
			if (strcmp(element->course_name, name) == 0){
			return element;
			}
		} else{
			if (count == num_of_courses){
				printf(" %s %d\n", element->course_name, element->course_grade);
				return NULL;
			}
			printf(" %s %d,", element->course_name, element->course_grade);
			count++;
		}
		
	}
	//If left for, we did not find, return NULL.
	return NULL;
}

/*=====================Functions for Struct Student===========================*/
/**
 * @brief: This Function will create a student with name, ID and empty list of
 * 		   Courses.	Returns a pointer to a student struct.
 * @param stu_name: Pointer to a string containing student name,
 * @param ID: Student ID.
 * @return: Pointer to a struct of type student, or NULL on error.
 */
static struct student* student_init(char *stu_name, int ID){
	struct student *output;
	//Need to allocate space for output
	output = (struct student*)malloc(sizeof(*output));
	if(output == NULL){
		fprintf(stderr,"Malloc failed\n");
        return NULL;
	}
	//Need to allocate space for strign name
	output->name = (char*)malloc(sizeof(char) * strlen(stu_name));
	if(output->name == NULL){
		fprintf(stderr,"Malloc failed\n");
        free(output);
        return NULL;
	}

	strcpy(output->name, stu_name);
	output->ID = ID;
	output->courses = list_init(clone_course, destroy_course);
	return output;
}

/*=====================Functions for Struct Course============================*/

/**
 * @brief: This Function will create a corse with name and a grade.
 * 		   Returns a pointer to a course struct.
 * @param name: Pointer to a string containing course name,
 * @param grade: Grade.
 * @return: Pointer to a struct of type course, or NULL on error.
 */
static struct course* course_init(char *name, int cour_grade){
	struct course *output;
	//Need to allocate space for output
	output = (struct course*)malloc(sizeof(*output));
	if(output == NULL){
		fprintf(stderr,"Malloc failed\n");
        return NULL;
	}
	//Need to allocate space for strign name
	output->course_name = (char*)malloc(sizeof(char) * strlen(name));
	if(output->course_name == NULL){
		fprintf(stderr,"Malloc failed\n");
        free(output);
        return NULL;
	}

	strcpy(output->course_name, name);
	output->course_grade = cour_grade;
	return output;
}

