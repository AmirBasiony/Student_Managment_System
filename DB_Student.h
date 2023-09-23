/*
 * DB_Student.h
 *
 *  Created on: 23/2/2023
 *      Author: Amir
 */

#ifndef DB_STUDENT_H_
#define DB_STUDENT_H_

#define NAME_LENGTH 20
#define COURSES_NUMBER 5
#define STUDENTS_NUMBER 50
//User configuration
struct Sinfo{
	char fname[40];
	char lname[40];
	int roll;
	float GPA;
	int cid[10];
};

typedef struct Sinfo SStd_Data_t;

#define element_type SStd_Data_t

//FIFO data type
typedef struct {
	unsigned int count;
	unsigned int length;
	element_type* base;
	element_type* head;
	element_type* tail;
}FIFO_buf_t;

typedef enum{
	FIFO_no_error,
	FIFO_NULL,
	FIFO_FULL,
	FIFO_empty,
	FIFO_error
}FIFO_Buf_status_e;


//APIs
FIFO_Buf_status_e DB_Student_init(FIFO_buf_t* FIFO_buf , SStd_Data_t* buf , unsigned int length);
FIFO_Buf_status_e FIFO_enqueue(FIFO_buf_t* FIFO_buf , SStd_Data_t ENQ_data);
FIFO_Buf_status_e add_student_manually(FIFO_buf_t* FIFO_buf );
FIFO_Buf_status_e Add_Student_From_Text_File(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e delete_student_by_roll(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e delete_all_student(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e view_student(FIFO_buf_t* FIFO_buf );
FIFO_Buf_status_e Find_Student_by_Roll_Number(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e Find_Student_by_First_Name(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e Find_Student_by_Course_ID(FIFO_buf_t* FIFO_buf);
void  Print_Students_Number(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e Update_Student_by_Roll_Number(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e Reverse_the_List(FIFO_buf_t* FIFO_buf);
FIFO_Buf_status_e FIFO_IS_Buf_Full(FIFO_buf_t* FIFO_buf);
struct Sinfo* search_student_by_roll(FIFO_buf_t* FIFO_buf,int Roll);

#endif /* DB_STUDENT_H_ */
