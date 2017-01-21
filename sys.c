#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "globals.h"
#include "utility.h"
#include "admin.h"
#include "guards.h"

/*
///// UTILITY FUNCTION //////
--------IN utility.h----------

void init(userData* user);
int searchDataBase(userData user);
int compareStruct(userData a,userData b);

//// GUARD FUNCTIONS ///// 
---------IN guards.h-----------

void guardRegister();
void guardLogin();
int guardPage(userData* user);
int guardInnerPage(int a,userData* user);

///// ADMIN FUNCTOINS /////
----------IN admin.h----------

void createAdmin();
int adminPage();
void adminLogin();
int adminInnerPage();
void generateSchedule();
*/
/// RUN && LOAD ////

void load(){
	userData user;init(&user);
	char* input = (char*) malloc(sizeof(char)*20);
	FILE* lea;
	FILE* over;
	FILE* ptr;
	FILE* timetable;
	
	ptr = fopen("data.bin","rb"); 
	if(ptr == NULL){
		ptr = fopen("data.bin","wb");
		fclose(ptr);
		createAdmin();
		ptr = fopen("data.bin","rb");
	}
	
	lea = fopen("leaves.bin","rb");
	if(lea == NULL){
		lea = fopen("leaves.bin","wb");
		fclose(lea);
		lea = fopen("leaves.bin","rb");
	}
	
	over = fopen("overduty.bin","rb");
	if(over == NULL){
		over = fopen("overduty.bin","wb");
		fclose(over);
		over = fopen("overduty.bin","rb");
	}
	
	timetable = fopen("schedule.bin","rb");
	if(timetable == NULL){
		timetable = fopen("schedule.bin","wb");
		fclose(timetable);
		timetable = fopen("schedule.bin","rb");
	}
	
	fread(dutyDiagram,sizeof(dutyDiagram),1,timetable);
	fclose(timetable);
	
	while(fread(input,sizeof(char)*20,1,lea)){
		if(feof(lea)!=0) break;
		if(noOfLeaveRequests == 0) leaveRequest = (char**)malloc(sizeof(char*));
		else leaveRequest = realloc(leaveRequest,(noOfLeaveRequests+1)*sizeof(char*));
		leaveRequest[noOfLeaveRequests] = input;
		input = (char*) malloc(sizeof(char)*20);
		noOfLeaveRequests++;
	}
	fclose(lea);
	while(fread(input,sizeof(char)*20,1,over)){
		if(feof(over)!=0) break;
		if(noOfOverDutyRequests == 0) overDutyRequest = (char**)malloc(sizeof(char*));
		else overDutyRequest = realloc(overDutyRequest,(noOfOverDutyRequests+1)*sizeof(char*));
		overDutyRequest[noOfOverDutyRequests] = input;
		input = (char*) malloc(sizeof(char)*20);
		noOfOverDutyRequests++;
	}
	fclose(over);
	while(fread(&user,sizeof(userData),1,ptr)){
		if(feof(ptr)!=0)
    			break;
	    	if(dataBaseSize == 0) database = (userData*) malloc(sizeof(userData));
			else database = realloc(database,(dataBaseSize+1)*sizeof(userData));
			database[dataBaseSize] = user;
			dataBaseSize++;
	}
	fclose(ptr);
	
	return;
}

void destructor(){
	FILE* ptr;
	FILE* ptr1;
	FILE* ptr2;
	FILE* ptr3;
	int i = 0,j=0,k=0;

	ptr1 = fopen("leaves.bin","wb");
	for( i = 0; i < noOfLeaveRequests; i++){
		fwrite(leaveRequest[i],sizeof(char)*20,1,ptr1);
	}
	fclose(ptr1);
	
	ptr2 = fopen("overduty.bin","wb");
	for( i = 0; i < noOfOverDutyRequests; i++){
		fwrite(overDutyRequest[i],sizeof(char)*20,1,ptr2);
	}
	fclose(ptr2);
	ptr = fopen("schedule.bin","wb");
	for(i = 0; i < 7; i++){
		for(j = 0; j < 3; j++){
			for(k = 0; k < NOOFPLACES; k++){
				fwrite(dutyDiagram[i][j][k],sizeof(char)*20,1,ptr);
			}
		}
	}
	fclose(ptr);
	
	ptr3 = fopen("data.bin","wb");
	for(i = 0; i < dataBaseSize; i++){
		fwrite(&database[i],sizeof(userData),1,ptr3);
	}
	fclose(ptr3);
	
	return;
}

void run(){
	int option;
	while(1){
		system("clear");
		printf("Welcome Sir!\n");
		printf("Choose what to do.\n");
		printf("1. Security Guard Login\n");
		printf("2. Security Guard Register\n");
		printf("3. Admin Login\n");
		printf("4. Exit\n");
		scanf("%d",&option);
		switch(option){
			case 1 :
				guardLogin();
				break;	
			case 2 :
				guardRegister();
				break;
			case 3 : 
				adminLogin();	
				break;	
			case 4 :
				system("clear");
				destructor();
				exit(0);			
			default :
				printf("Invalid Input,Please try again!\n");
				system("sleep 1");
		}
	}
	return;	
}

int main(){
	load();
	run();	
	return 0;
}


