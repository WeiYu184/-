#include <stdio.h>


/* Put Course Structure Definition Here */
typedef struct course 
{
	char c_name[5]; // course name
	int c_ID;
	char c_section[10];  //course ID, course section
}course;


/* Put Student Structure Definition Here */
typedef struct student
{
	char first_name[100], last_name[100], SSN[15];
	int num_of_c;     // number of courses attending
	course array_of_c[10];  // array of courses
	struct student *snext;  // next
}student;


void ReadInputFile ( char *filename );
void AddToList ( struct student *addStudent );
void PrintToFile ( char *filename );
void ProcessDeleteFile ( char *filename );
void DeleteStudent ( char *SSN );
void DeleteList ( );