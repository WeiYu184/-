#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
struct student *head = NULL;  
// Global Linked List initially set to zero struct
int count = 0;
// for count（计算插入了多少个学生的信息）
struct student* tail = NULL;


int main(void)
{

  head = (struct student*)malloc(sizeof(struct student));
  head->snext = NULL;

  // Read the input file and create the linked list
  // ReadInputFile calls helper function AddToList to create list in sorted order 
  ReadInputFile("student.txt");   

  // print the current list to the outputfile output.txt
  PrintToFile("output.txt");

  //Read and process the dropped students file delete.txt
  //It uses helper function DeleteStudent to remove the student from the list
  ProcessDeleteFile("delete.txt");

  //print the updated linked list to the file update.txt
  PrintToFile("update.txt");

  //Delete the Entire Linked List before exiting the program
  DeleteList();

  return 0 ;
}


void ReadInputFile ( char *filename )
{
    struct student tmp;      
                                             // temporary(临时) student structure
	FILE *fp1 = fopen("student.txt", "rt");  // open up the input file for read mode
	if(!fp1)
	{
		printf("ERROR! Cannot open the input file!");
		return;     
	}                                        // if it cannot open print error message
	
	while(!feof(fp1)){
	     fscanf(fp1,"%s\n", tmp.first_name);
		 fscanf(fp1,"%s\n", tmp.last_name);
		 fscanf(fp1,"%s\n", tmp.SSN);
		 fscanf(fp1,"%d", &tmp.num_of_c);
	     if(tmp.first_name[0] == '*')break;  // while you still have data to read
		 else for(int i = 0; i < tmp.num_of_c; i++)
		 {
			 fscanf(fp1, " %s  %d  %s\n",tmp.array_of_c[i].c_name, &tmp.array_of_c[i].c_ID, tmp.array_of_c[i].c_section );
		 }                                   // read one student's data into temporary structure
		 AddToList(&tmp);
	}                                    
	fclose(fp1);
}

void AddToList ( struct student *addStudent )
{
	count++;
	struct student *new_stu = (struct student*)malloc(sizeof(struct student));   
	new_stu->snext = NULL;                   // malloc a new student

	strcpy(new_stu->first_name, addStudent->first_name);
	strcpy(new_stu->last_name, addStudent->last_name);
	new_stu->num_of_c = addStudent->num_of_c;
	strcpy(new_stu->SSN, addStudent->SSN);
	for(int i=0;i<new_stu->num_of_c; i++)
	{
		strcpy(new_stu->array_of_c[i].c_name, addStudent->array_of_c[i].c_name);
		new_stu->array_of_c[i].c_ID = addStudent->array_of_c[i].c_ID;
		strcpy(new_stu->array_of_c[i].c_section, addStudent->array_of_c[i].c_section);
	}                                        // copy tmp to new student

    struct student *p = head, *prev = head;
	if(count == 1)
	{
		head->snext = new_stu; 
		new_stu->snext = NULL;
		return; 
	}                                        // is linked list empty? if so, new student is head
	
	else while( p!=NULL && p->snext != NULL)
	{
		prev = p;
		if(strcmp(p->snext->last_name, new_stu->last_name) == 0 )
			if(strcmp(p->snext->first_name, new_stu->first_name) > 0)break;
		if(strcmp(p->snext->last_name, new_stu->last_name) > 0 )break;
		p = p->snext;
	}

	prev->snext = new_stu; 
	new_stu->snext = p;                   // if not, search through the linked list to find a proper place for it
}

void PrintToFile ( char *filename )
{
	FILE*fp2 = fopen(filename, "wt"); // open up file for write mode

	for(struct student *p = head->snext; p != NULL; p=p->snext )
	{
		fprintf(fp2,"SSN: %s\nName: %s, %s\nRegistered For %d Classes\n",p->SSN, p->last_name, p->first_name, p->num_of_c);
		for(int i=0; i<p->num_of_c; i++)
			fprintf(fp2,"  - %s0%d-%s\n",p->array_of_c[i].c_name, p->array_of_c[i].c_ID, p->array_of_c[i].c_section);
	}// print the linked list student by student to the output file
}

void ProcessDeleteFile ( char *filename )
{
	char deleteSSN[20][64] = {""};  // create string deleteSSN

	FILE* fp3 = fopen(filename, "rt"); // open up the file for read mode

	if(!fp3)
	{
		printf("ERROR! Cannot open the delete file! ");
		return;                        // if it cannot open print error message
	}
    int x = 0;
	do{fscanf(fp3,"%s\n",deleteSSN[x]);x++;}
	while(deleteSSN[x-1][0] != '*');
	                                   // while you still have data to read
                                       // read a string from the file
	
	DeleteStudent(deleteSSN[x-1]);          // call the function DeleteStudent with argument SSN that you read in 
	fclose(fp3);                                    
}
         
void DeleteStudent ( char *SSN )
{
	struct student *cur = (struct student*)malloc(sizeof(struct student));
	struct student *prev = NULL;
	
	for(cur = head, prev = head; cur->snext != NULL && strcmp(cur->SSN, SSN) != 0; prev = cur, cur = cur->snext);
	// search through the linked list to see if SSN exists
	// If so, remove it from the linked list
	// Concerns:
	// - what if deleted student is the head?
	// - what if deleted student is in the middle?
	// - what if deleted student is at the tail?
	if ( cur->snext == NULL && strcmp(cur->SSN ,SSN) == 0)free(cur);
	else 
	{
		prev->snext = cur->snext;
	    free(cur);
	}
}

void DeleteList ( )
{
	struct student *p = head, *cur = (struct student*)malloc(sizeof(struct student));
	while(p != NULL)
	{
		cur = p;
		p = cur->snext;
		free(cur);
	}// start from the head and remove each entry from the list and free up the space via calling the function free
}