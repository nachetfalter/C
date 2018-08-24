//Topic: Sorting Students
//Author: Victor Wang
//Date: 20/05/2017
//Description: Sorting Students

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_STUDENT_NAME_LENGTH 128
#define MAX_GRADE_STRING_LENGTH 22
#define MAX_LAB_NAME_LENGTH 32
#define MAX_LINE_LENGTH 4096

struct student {
    int              zid;
    char             name[MAX_STUDENT_NAME_LENGTH + 1];
    char             lab_name[MAX_LAB_NAME_LENGTH + 1];
    char             lab_grades[MAX_GRADE_STRING_LENGTH + 1];
    struct student   *next;
};

struct student *read_students_file(char filename[]);
struct student *read_student(FILE *stream);
double grades2labmark(char grades[]);

int main(int argc, char *argv[]) {

	int dummy_id = 0;

	//These dummy names exist because strtok break the original
	char dummy_name1[MAX_STUDENT_NAME_LENGTH + 1];
	char dummy_name2[MAX_STUDENT_NAME_LENGTH + 1];
	char dummy_name3[MAX_STUDENT_NAME_LENGTH + 1];
	char dummy_name4[MAX_STUDENT_NAME_LENGTH + 1];
	char dummy_name5[MAX_STUDENT_NAME_LENGTH + 1];

	//These are for strcpy strings to their new pointer position
	char dummy_lab_name[MAX_LAB_NAME_LENGTH + 1];
	char dummy_lab_grades[MAX_GRADE_STRING_LENGTH + 1];
    char delimiter[2] = " ";

    //Breaked strings
    char *fname;
    char *lname;
    char *fname2;
    char *lname2;

    //Switch
    int trigger = 0;

    //Node position recording
    struct student*start_node;
    struct student*node1_position;
	struct student*node1_next;
	struct student*node2_position;
	struct student*node2_next;

	//Utiliy
	struct student*dummy;
    struct student*s;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <marks-file>\n", argv[0]);
        return 1;
    }

    struct student *student_list = read_students_file(argv[1]);
    
    

    //Record the start Location of the list
    start_node = student_list;
    dummy = student_list -> next;
    node2_next = dummy;

    while(student_list != NULL){

        //Break the start node strings
    	strcpy(dummy_name1,student_list -> name);
        fname = strtok(dummy_name1, delimiter);
        strcpy(dummy_name2,student_list -> name);
        lname = strrchr(dummy_name2, ' ');
        dummy_id = 0;
    	dummy_id = student_list -> zid;
          	
    	//Record the current location
    	dummy = student_list;
    	node1_position = dummy;
    	dummy = student_list -> next;
    	node1_next = dummy;
    	trigger = 0;

        //My pathetic attempt to spead up the calculaton, but it is still way too slow. Too bad I didn't realize using quick sort would be better.
    	student_list = student_list -> next;
    	
        while(student_list != NULL){

        	//Break string again for comparison
        	strcpy(dummy_name3,student_list -> name);
	        fname2 = strtok(dummy_name3, delimiter);
	        strcpy(dummy_name4,student_list -> name);
	        lname2 = strchr(dummy_name4, ' ');

	        //Record desired target infos
        	if(strcmp(lname, lname2) > 0){
        		dummy_id = student_list -> zid;
        		strcpy(dummy_name1,student_list -> name);
		        fname = strtok(dummy_name1, delimiter);
		        strcpy(dummy_name2,student_list -> name);
		        lname = strchr(dummy_name2, ' ');
        		dummy = student_list;
        		node2_position = dummy;
        		dummy = student_list -> next;
        		node2_next = dummy;
        		trigger = 1;
        	}
        	else if(strcmp(lname,lname2) == 0 && strcmp(fname, fname2) > 0){
        		dummy_id = student_list -> zid;
        		strcpy(dummy_name1,student_list -> name);
		        fname = strtok(dummy_name1, delimiter);
		        strcpy(dummy_name2,student_list -> name);
		        lname = strchr(dummy_name2, ' ');
        		dummy = student_list;
        		node2_position = dummy;
        		dummy = student_list -> next;
        		node2_next = dummy;
        		trigger = 1;
			}
        	else if(strcmp(lname,lname2) == 0 && strcmp(fname, fname2) == 0 && dummy_id > student_list -> zid){
        		dummy_id = student_list -> zid;
        		strcpy(dummy_name1,student_list -> name);
		        fname = strtok(dummy_name1, delimiter);
		        strcpy(dummy_name2,student_list -> name);
		        lname = strchr(dummy_name2, ' ');
        		dummy = student_list;
        		node2_position = dummy;
        		dummy = student_list -> next;
        		node2_next = dummy;  
        		trigger = 1;
        	}

            student_list = student_list -> next;     
        }

        //Switch the pointer location
        if(trigger == 1){
 
        	student_list = node1_position;
        	strcpy(dummy_name5, student_list -> name);
        	strcpy(dummy_lab_name, student_list -> lab_name);
        	strcpy(dummy_lab_grades, student_list -> lab_grades);
        	dummy_id = student_list -> zid; 
        	*student_list = *node2_position;
        	student_list -> next = node1_next;
        	       	
        	student_list = node2_position;       	
        	*student_list = *node1_position;
        	strcpy(student_list -> name, dummy_name5);
        	strcpy(student_list -> lab_name, dummy_lab_name);
        	strcpy(student_list -> lab_grades, dummy_lab_grades);
        	student_list -> zid = dummy_id; 
        	student_list -> next = node2_next;  
        }
        
        //Reset location
        student_list = node1_position;
        student_list = student_list -> next;
    }
    

    
    //Print
    student_list = start_node;
    while(student_list != NULL){
        printf("%d %-30s %-12s %-22s %4.1lf\n",student_list -> zid,student_list -> name, student_list -> lab_name, student_list -> lab_grades, grades2labmark(student_list -> lab_grades));
    	s = student_list -> next;
    	free(student_list);
    	student_list = s;
    }
    
    return 0;
}

// DO NOT CHANGE THE CODE BELOW HERE - DO NOT CHANGE read_students_file

// read_students_file reads a file where line contains information for 1 student
// it creates a linked of student structs containing the information
// it returns a pointer to the head of the list

struct student *read_students_file(char filename[]) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr,"warning file %s could not  be opened for reading\n", filename);
        return NULL;
    }

    struct student *first_student = NULL;
    struct student *last_student = NULL;
    struct student *s;
    while ((s = read_student(fp)) != NULL) {
        if (last_student == NULL) {
            first_student = s;
            last_student = s;
        } else {
            last_student->next = s;
            last_student = s;
        }
    }
    return first_student;
}

// DO NOT CHANGE read_student

// read_student mallocs a student struct
// and reads a line in this format:
//
// 5099703 Tsun Bordignon thu13-sitar A+A+CABAB..A.
//
// stores the values in the struct field
// and returns a pointer to the struct

struct student *read_student(FILE *stream) {
    char line[MAX_LINE_LENGTH];

    struct student *s = malloc(sizeof (struct student));
    assert(s);

    if (fgets(line, MAX_LINE_LENGTH, stream) == NULL) {
        free(s);
        return NULL;
    }

    char *newline_ptr = strchr(line, '\n');
    assert(newline_ptr);
    *newline_ptr = '\0';

    char *space_ptr = strrchr(line, ' ');
    assert(space_ptr);
    strncpy(s->lab_grades, space_ptr + 1, MAX_GRADE_STRING_LENGTH);
    s->lab_grades[MAX_GRADE_STRING_LENGTH] = '\0';
    *space_ptr = '\0';

    space_ptr = strrchr(line, ' ');
    assert(space_ptr);
    strncpy(s->lab_name, space_ptr + 1, MAX_LAB_NAME_LENGTH);
    s->lab_name[MAX_LAB_NAME_LENGTH] = '\0';
    *space_ptr = '\0';

    space_ptr = strchr(line, ' ');
    assert(space_ptr);
    strncpy(s->name, space_ptr + 1, MAX_STUDENT_NAME_LENGTH);
    s->name[MAX_STUDENT_NAME_LENGTH] = '\0';
    *space_ptr = '\0';

    s->zid = atoi(line);
    s->next = NULL;
    return s;
}

double grades2labmark(char grades[]){
	int index = 0;
	double mark = 0;

	while(grades[index] != '\0'){
		if(grades[index + 1] == '+'){
            mark += 1.2;
		}
		else if(grades[index] == 'A'){
			mark += 1;
		}
		else if(grades[index] == 'B'){
			mark += 0.8;
		}
		else if(grades[index] == 'C'){
			mark += 0.5;
		}
		else if(grades[index] == '.'){
			mark += 0;
		}
		index += 1;
	}
    
    if(mark > 10.0) mark = 10.0;
    return mark;
	
}