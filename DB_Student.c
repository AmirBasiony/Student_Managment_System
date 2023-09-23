/*
 * DB_Student.c
 *
 *  Created on: 23/2/2023
 *      Author: Amir
 */
 
#include"stdio.h"
#include"DB_Student.h"
#include"string.h"

FILE *students_file;

FIFO_Buf_status_e DB_Student_init(FIFO_buf_t* FIFO_buf , SStd_Data_t* buf , unsigned int length)
{
	if(buf == NULL)
		return FIFO_NULL;

	FIFO_buf->base = buf;
	FIFO_buf->head = buf;
	FIFO_buf->tail = buf;
	FIFO_buf->length = length;
	FIFO_buf->count = 0;

	return FIFO_no_error;
}
FIFO_Buf_status_e add_student_manually(FIFO_buf_t* FIFO_buf )
{
	int i;
	SStd_Data_t new_student;
	printf("---------------------------\n");
	printf("Add the Student Details: \n");
	printf("---------------------------\n");
	printf("Enter the Roll Number: ");
	scanf("%d",&new_student.roll);
	if(search_student_by_roll(FIFO_buf,new_student.roll) != NULL)
	{
		printf("\n[ERROR]: Roll Number %d is already taken\n",new_student.roll);
		return FIFO_error;
	}
	// entering the rest of data
	printf("Enter the first name of the student: ");
	scanf("%s",new_student.fname);
	printf("Enter the last name of the student: ");
	scanf("%s",new_student.lname);
	printf("Enter the GPA tou obtained: ");
	scanf("%f",&new_student.GPA);
	
	printf("Enter the course ID for each course: \n");
	for(i = 0; i<COURSES_NUMBER ; i++)
	{
		printf("Course numer %d: ",(i+1));
		scanf("%d",&new_student.cid[i]);
	}
	
	if(FIFO_enqueue(FIFO_buf,new_student)== FIFO_no_error)
	{
		printf("\n[INFO]: Student Details is added successfully\n");
	}
	else
	{
		printf("\n[ERROR]: Adding students manually failed\n");
		return FIFO_error;
	}
	Print_Students_Number(FIFO_buf);
	return FIFO_no_error;
}
FIFO_Buf_status_e Add_Student_From_Text_File(FIFO_buf_t* FIFO_buf)
{
	int i;
	SStd_Data_t new_student;
	
	printf("---------------------------\n");
	students_file = fopen("Students.txt","r");
	
	// check if the file  is NULL (not exist)
	if(students_file == NULL)
	{
		printf("\n [ERROR] student.txt file not found. \n");
		printf("\n [ERROR] adding students from file failed. \n");
		return FIFO_NULL;
	}
	
	while(!feof(students_file))
	{
		fscanf(students_file,"%d",&new_student.roll);
		
		//Check if roll number is exist or not.
		if(search_student_by_roll(FIFO_buf,new_student.roll) != NULL)
		{
			printf("\n[ERROR]: Roll Number %d is already taken\n",new_student.roll);
			
			// ignoe the rest of the line
			fscanf(students_file, "%*[^\n]");
			
		}
		else
		{	
			// entering the rest of data
			fscanf(students_file,"%s",new_student.fname);
			fscanf(students_file,"%s",new_student.lname);
			fscanf(students_file,"%f",&new_student.GPA);
			for(i = 0; i<COURSES_NUMBER ; i++)
			{
				fscanf(students_file,"%d",&new_student.cid[i]);
			}
			if(FIFO_enqueue(FIFO_buf,new_student)== FIFO_no_error)
			{
				printf("\n[INFO]: Student number %d Details is added successfully\n",new_student.roll);
			}
			else
			{
				printf("\n[ERROR]: Adding student number %d failed\n",new_student.roll);
				return FIFO_error;
			}
		}
	}
	printf("--------------------------------------------------\n");
	printf("[INFO] Students details are saved successfully\n");

	// Closing the file
	fclose(students_file);
	
	Print_Students_Number(FIFO_buf);
	return FIFO_no_error;
}
FIFO_Buf_status_e FIFO_IS_Buf_Full(FIFO_buf_t* FIFO_buf)
{
	//check if FIFO is valid or not
	if(!FIFO_buf->base || !FIFO_buf->head || !FIFO_buf->tail)
		return FIFO_NULL;

	if(FIFO_buf->count >= FIFO_buf->length)
	{
		printf("==========FIFO is Full==========\n");
		return FIFO_FULL;
	}
	return FIFO_no_error;
}
FIFO_Buf_status_e FIFO_enqueue(FIFO_buf_t* FIFO_buf , SStd_Data_t ENQ_data)
{
	//check if FIFO is valid or not
	if(!FIFO_buf->base || !FIFO_buf->head || !FIFO_buf->tail)
		return FIFO_NULL;

	//check if FIFO is Full
	if(FIFO_IS_Buf_Full(FIFO_buf) != FIFO_no_error)
	{
		printf("[ERROR]: Student DataBase is FULL\n");
		return FIFO_IS_Buf_Full(FIFO_buf);
	}
	else
	{
		// Enqueue an element
		*(FIFO_buf->head) = ENQ_data;
		FIFO_buf->count++;


		if(FIFO_buf->head == (FIFO_buf->base + (FIFO_buf->length * sizeof(SStd_Data_t))))
			FIFO_buf->head = FIFO_buf->base;
		else
			FIFO_buf->head++;

		return FIFO_no_error;
	}
}
FIFO_Buf_status_e view_student(FIFO_buf_t* FIFO_buf )
{
	struct Sinfo* temp = FIFO_buf->tail;
	//check if FIFO is valid or not
	if(!FIFO_buf->base || !FIFO_buf->head || !FIFO_buf->tail)
	{
		printf("-------------------------------\n");
		printf("[ERROR]: Students DataBase is NULL\n");
		printf("-------------------------------\n");
		return FIFO_NULL ;
	}

	//check if FIFO is empty or not
	if(FIFO_buf->count == 0)
	{
		printf("-------------------------------\n");
		printf("[ERROR]: Students DataBase is Empty\n");
		printf("-------------------------------\n");
		return FIFO_empty;
	}
	
	//print FIFO elements
	for(int i=0 ; i< FIFO_buf->count ; i++)
	{
		printf("-------------------------------\n");
		printf("\tStudent first name: %s\n",(temp->fname));
		printf("\tStudent last name: %s\n",(temp->lname));
		printf("\tStudent Roll Number: %d\n",(temp->roll));
		printf("\tStudent GPA: %.2f\n",(temp->GPA));
		for(int j=0;j<5;j++)
		{
			printf("\tThe course ID are: %d\n",(temp->cid[j]));
		}
		temp++;
	}
	return FIFO_no_error;
}
FIFO_Buf_status_e delete_student_by_roll(FIFO_buf_t* FIFO_buf)
{
	int i,check,Deleted_Roll,student_number;
	SStd_Data_t* student = FIFO_buf->tail;
	
	printf("---------------------------\n");
	//check if FIFO is valid or not
	if(!FIFO_buf->base || !FIFO_buf->head || !FIFO_buf->tail)
	{
		printf("[ERROR]: Student DataBase  not found");
		return FIFO_NULL;
	}
	//check if FIFO is empty
	if(FIFO_buf->count == 0)
	{
		printf("[ERROR]: Student DataBase is Empty");
		return FIFO_empty;
	}
	
	printf("Enter the Roll Number: ");
	scanf("%d",&student_number);
	
	for(i=0;i< FIFO_buf->count;i++)
	{
		//Delete a student from QUEUE
		if(student->roll == student_number)
		{
			//Deteting the student
			*student = *(FIFO_buf->tail);
			FIFO_buf->count-- ;
			
			//if(FIFO_buf->tail == FIFO_buf->length)
			if(FIFO_buf->tail == (FIFO_buf->base + (FIFO_buf->length * sizeof(SStd_Data_t))))
			{
				FIFO_buf->tail = FIFO_buf->base;
			}
			else
			{
				FIFO_buf->tail++;
			}
			check = 1;
			break;
		}
		else
		{
			check = 0;
		}
		student++;
	}
	if(check)
	{
		printf("[INFO]: Deleting the student Number %d done successfully\n",student_number);
	}
	else
	{	
		printf("[ERROR]: Deleting the student is failed\n");
	}
	return FIFO_no_error;
}
struct Sinfo* search_student_by_roll(FIFO_buf_t* FIFO_buf,int Roll)
{
	int i;
	struct Sinfo* student = FIFO_buf->tail;
	
	for(i = 0 ; i < FIFO_buf->count ; i++)
	{
		if(student->roll == Roll)
		{
			break;
		}
		//if we reach to the last element in QUEUE
		if(student == (FIFO_buf->base + (FIFO_buf->length * sizeof(element_type))))
		{
			student = FIFO_buf->base;
		}
		else
		{
			student++;
		}
	}
	
	if(i == FIFO_buf->count)
	{
		student = NULL;
	}
	return student;
}
FIFO_Buf_status_e delete_all_student(FIFO_buf_t* FIFO_buf)
{
	if(!FIFO_buf->base || !FIFO_buf->head || !FIFO_buf->tail)
	{
		printf("==========FIFO is Null==========\n");
		return FIFO_NULL;
	}
	//check if FIFO is empty
	if(FIFO_buf->count == 0)
	{
		printf("==========FIFO is empty==========\n");
		return FIFO_empty;
	}
	while(FIFO_buf->count !=0)
	{
		//	*DEQ_data = *(FIFO_buf->tail) ;
		FIFO_buf->count-- ;
		
		//if(FIFO_buf->tail == FIFO_buf->length)
		if(FIFO_buf->tail == (FIFO_buf->base + (FIFO_buf->length * sizeof(element_type))))
		{
			FIFO_buf->tail = FIFO_buf->base;
		}
		else
		{
			FIFO_buf->tail++;
		}
	}
	printf("[INFO]: Deleting the student is done successfully\n");
	return FIFO_no_error;
}
FIFO_Buf_status_e Find_Student_by_Roll_Number(FIFO_buf_t* FIFO_buf)
{
	int i,Roll_Number;
	struct Sinfo* student = FIFO_buf->tail;
	
	printf("---------------------------\n");
	printf("Add the Student Details: \n");
	printf("---------------------------\n");
	printf("Enter the Roll Number: ");
	scanf("%d",&Roll_Number);
	
	for(i=0; i<FIFO_buf->count ;i++)
	{
		if(student->roll == Roll_Number)
		{
			break;
		}
		else
		{
			//if we reach to the last element in QUEUE
			if(student == (FIFO_buf->base + (FIFO_buf->length * sizeof(element_type))))
			{
				student = FIFO_buf->base;
			}
			else
			{
				student++;
			}
		}
	}
	if(i == FIFO_buf->count)
	{
		printf("\n[ERROR]: Roll Number %d not found\n",Roll_Number);
		return FIFO_error;
	}
	else
	{
		printf("The Student Details are:\n");
		printf("\tStudent first name: %s\n",(student->fname));
		printf("\tStudent last name: %s\n",(student->lname));
		printf("\tStudent Roll Number: %d\n",(student->roll));
		printf("\tStudent GPA: %.2f\n",(student->GPA));
		for(int j=0;j<COURSES_NUMBER;j++)
		{
			printf("\tThe course ID are: %d\n",(student->cid[j]));
		}
	}
	return FIFO_no_error;
}
FIFO_Buf_status_e Find_Student_by_First_Name(FIFO_buf_t* FIFO_buf)
{
	int i;
	char First_Number[20];
	struct Sinfo* student = FIFO_buf->tail;
	
	printf("---------------------------\n");
	printf("Add the Student Details: \n");
	printf("---------------------------\n");
	printf("Enter The First Name: ");
	scanf("%s",First_Number);
	
	for(i=0; i<FIFO_buf->count ;i++)
	{
		if(!strcmp(student->fname,First_Number))
		{
			break;
		}
		else
		{
			//if we reach to the last element in QUEUE
			if(student == (FIFO_buf->base + (FIFO_buf->length * sizeof(element_type))))
			{
				student = FIFO_buf->base;
			}
			else
			{
				student++;
			}
		}
	}
	if(i == FIFO_buf->count)
	{
		printf("\n[ERROR]: First Name %s not found\n",First_Number);
		return FIFO_error;
	}
	else
	{
		printf("The Student Details are:\n");
		printf("\tStudent first name: %s\n",(student->fname));
		printf("\tStudent last name: %s\n",(student->lname));
		printf("\tStudent Roll Number: %d\n",(student->roll));
		printf("\tStudent GPA: %.2f\n",(student->GPA));
		for(int j=0;j<5;j++)
		{
			printf("\tThe course ID are: %d\n",(student->cid[j]));
		}
	}
	return FIFO_no_error;
}
FIFO_Buf_status_e Find_Student_by_Course_ID(FIFO_buf_t* FIFO_buf)
{
	int i,j,course_id,counter=0;
	struct Sinfo* student = FIFO_buf->tail;
	printf("---------------------------\n");
	if(!FIFO_buf->count || !FIFO_buf->base || !FIFO_buf->head || !FIFO_buf->tail)
	{
		printf("\n[ERROR]: Find The Student Failed\n",course_id);
		return FIFO_error;
	}
	
	printf("Enter Course ID: ");
	scanf("%d",&course_id);
	for(i=0; i<FIFO_buf->count ;i++)
	{
		//Loop throw the courses Id until finding 
		for(j=0;j<COURSES_NUMBER;j++)
		{
			if(student->cid[j] == course_id)
			{
				counter++;
				printf("--------------------------\n");
				printf("The Student Details are:\n");
				printf("\tStudent first name: %s\n",(student->fname));
				printf("\tStudent last name: %s\n",(student->lname));
				printf("\tStudent Roll Number: %d\n",(student->roll));
				printf("\tStudent GPA: %.2f\n",(student->GPA));
				break;
			}
		}
		//if we reach to the last element in QUEUE
		if(student == (FIFO_buf->base + (FIFO_buf->length * sizeof(element_type))))
		{
			student = FIFO_buf->base;
		}
		else
		{
			student++;
		}
	}
	if((counter == 0) && (i == FIFO_buf->count))
	{
		printf("\n[ERROR]: Course ID (%d) not found\n",course_id);
		return FIFO_error;
	}
	else
	{
		printf("\n[INFO] Total number of students enrolled: %d\n", counter);
	}
	return FIFO_no_error;
}
void Print_Students_Number(FIFO_buf_t* FIFO_buf)
{
	printf("---------------------------\n");
	printf("[INFO]: The total number of Student is %d\n",FIFO_buf->count);
	printf("[INFO]: You can add up to %d\n",STUDENTS_NUMBER);
	printf("[INFO]: You can add %d more students\n",(STUDENTS_NUMBER - FIFO_buf->count));
}
FIFO_Buf_status_e Update_Student_by_Roll_Number(FIFO_buf_t* FIFO_buf)
{
	int i,choice,Course_ID,Roll_Number;
	char temp[20];
	struct Sinfo* student = FIFO_buf->tail;
	printf("--------------------------------------\n");
	if(!FIFO_buf->count || !FIFO_buf->base || !FIFO_buf->head || !FIFO_buf->tail)
	{
		printf("[ERROR]: Students DataBase Empty\n");
		return FIFO_error;
	}
	
	
	printf("Enter the Roll Number: ");
	scanf("%d",&Roll_Number);
	
	for(i=0; i<FIFO_buf->count ;i++)
	{
		if(student->roll == Roll_Number)
		{
			break;
		}
		else
		{
			//if we reach to the last element in QUEUE
			if(student == (FIFO_buf->base + (FIFO_buf->length * sizeof(element_type))))
			{
				student = FIFO_buf->base;
			}
			else
			{
				student++;
			}
		}
	}
	if(i == FIFO_buf->count)
	{
		printf("\n[ERROR]: Roll Number %d not found\n",Roll_Number);
		return FIFO_error;
	}
	else
	{
		printf("Enter the roll number to update the entry:\n");
		printf("1. First name.\n");
		printf("2. Last name.\n");
		printf("3. Roll number.\n");
		printf("4. GPA.\n");
		printf("5. Courses.\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				printf("Enter the new first name of the student: ");
				scanf("%s",student->fname);
				printf("----------------------------\n");
				printf("[INFO]: UPDATED SUCCESSFULLY.\n");
				break;
			case 2: 
				printf("Enter the new last name of the student: ");
				scanf("%s",student->lname);
				printf("----------------------------\n");
				printf("[INFO]: UPDATED SUCCESSFULLY.\n");
				break;
			case 3:
				printf("Enter the new Roll Number: ");
				scanf("%d",&student->roll);
				printf("----------------------------\n");
				printf("[INFO]: UPDATED SUCCESSFULLY.\n");
				break;
			case 4:
				printf("Enter the new GPA tou obtained: ");
				scanf("%f",&student->GPA);
				printf("----------------------------\n");
				printf("[INFO]: UPDATED SUCCESSFULLY.\n");
				break;
			case 5:
			do{
				printf("Enter Course ID you want to update: ");
				scanf("%d",&Course_ID);
			
				for(i = 0; i<COURSES_NUMBER ; i++)
				{
					if(student->cid[i] == Course_ID)
					{
						printf("Enter the new Course  ID: ");
						scanf("%d",&student->cid[i]);
						choice = 0;
						break;
					}
				}
				if(!choice)
				{
					printf("[INFO]: UPDATED SUCCESSFULLY.\n");
				}
				else
				{
					printf("----------------------------\n");
					printf("[ERROR]: Course ID not found\n");
					printf("----------------------------\n");
					printf("Try Again?\n");
					printf("1. Yes\n");
					printf("0. No\n");
					scanf("%d",&choice);	
				}
			}while(choice);
				break;
		}
	}
	return FIFO_no_error;
}

