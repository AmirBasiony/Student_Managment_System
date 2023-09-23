/*
 * mian.c
 *
 *  Created on: 23/2/2023
 *      Author: Amir
 */
#include"stdio.h"
#include"DB_Student.h"




int main()
{
	int choice;
	SStd_Data_t students_buffer[STUDENTS_NUMBER];
	FIFO_buf_t Student_DB;
	
	//Initialize QUEUE
	DB_Student_init(&Student_DB , students_buffer , STUDENTS_NUMBER);
	
	do{
		printf("\t Choose on of the following options\n\n");
		printf("1: Add Student Manually\n");
		printf("2: Add Student From Text File\n");
		printf("3: Find Student by Roll Number\n");
		printf("4: Find Student by First Name\n");
		printf("5: Find Student by Course ID\n");
		printf("6: Find The total Number of The Students \n");
		printf("7: Delete Student by Roll Number\n");
		printf("8: Update Student by Roll Number\n");
		printf("9: Show all informations of the Students\n");
		printf("10: Delete All Student\n");
		printf("\nEnter Option Number (1,2,3,4,....,10): ");
		scanf("%d",&choice);
		printf("Your choice is number: %d  \n\n",choice);
		switch(choice)
		{
			case 1:
				add_student_manually(&Student_DB);
				break;
			case 2: 
				Add_Student_From_Text_File(&Student_DB);
				break;
			case 3:
				Find_Student_by_Roll_Number(&Student_DB);
				break;
			case 4:
				Find_Student_by_First_Name(&Student_DB);
				break;
			case 5:
				Find_Student_by_Course_ID(&Student_DB);
				break;
			case 6:
				Print_Students_Number(&Student_DB);
				break;
			case 7:
				delete_student_by_roll(&Student_DB);
				break;
			case 8:
				Update_Student_by_Roll_Number(&Student_DB);
				break;
			case 9:
				view_student(&Student_DB);
				break;
			case 10:
				delete_all_student(&Student_DB);
				break;
			default :
				printf("\n========Your choice isn't in the list========\n");
				break;
		}
		printf("\n-------------------------------\n");
		printf("Choose any thing again or exit?\n");
		printf("1: Yes.\n");
		printf("2: No.\n");
		printf("-------------------------------\n");
		scanf("%d",&choice);
	}while(choice == 1);

	return 0;
}

