#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define NOOFGUARD 60
#define MAXLEAVES 5
#define LOGOUT -1
#define NOOFPLACES 10	

///// DATATYPE /////

typedef struct{
	char* name;
	char* identityNumber;
	char* password;
	int day;
	int time_start;
	int time_end;
	int placeIndex;
	int onLeave;
	int onOverDuty;
	int noOfLeaves;
	int noOfOverDuty;
} userData; 	

////// GLOBAL DATA ////
	
char* Admin = "Buridi";
char* AdminPassword = "buridi";
userData* database;

int dataBaseSize = 0;
int noOfLeaveRequests = 0;
int noOfOverDutyRequests = 0;
char** leaveRequest;
char** overDutyRequest;

///// UTILITY FUNCTION //////

void init(userData* user);
int searchDataBase(userData user);
int compareStruct(userData a,userData b);

//// GUARD FUNCTIONS ///// 

void guardRegister();
void guardLogin();
int guardPage();
int guardInnerPage(int a,userData user);

///// ADMIN FUNCTOINS /////

int adminPage();
void adminLogin();
int adminInnerPage();

/// RUN ////

void run(){
	int option;
	
	userData user;
	dataBaseSize = 1;
	
	database = (userData*) malloc(sizeof(userData));
	user.name = Admin;
	user.identityNumber = "ADMIN";
	user.password = AdminPassword;
	database[0] = user;
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
				exit(0);			
			default :
				printf("Invalid Input,Please try again!\n");
				system("sleep 1");
		}
	}
	return;	
}

int main(){
	run();	
	return 0;
}

//----------------------------//

void guardRegister(){
	userData user;
	init(&user);
	system("clear");
	printf("\nUsername : ");scanf("%s",user.name);
	printf("Identity Number : ");scanf("%s",user.identityNumber);
	printf("Password : ");scanf("%s",user.password);
	
	database = realloc(database,(dataBaseSize+1)*sizeof(userData));
		printf("User Registered Succesfully!\n");
		system("sleep 1");
	// else{
	// 	printf("Could Not Register.Out of Space!!\n");
	// }
	database[dataBaseSize] = user;
	dataBaseSize++;
	return;
}

void guardLogin(){
	userData user;
	init(&user);
	int guide;
	while(1){
		system("clear");
		printf("\nUserName : ");scanf("%s",user.name);
		printf("Identity Number : ");scanf("%s",user.identityNumber);
		printf("Password : ");scanf("%s",user.password);
		if(searchDataBase(user)){
			if(guardPage(user) == LOGOUT) return;
		}
		else{	
			printf("Invalid UserName or Password.\nPress 1 to retry.\nPress 2 to go to previous page.\nThankYou.\n");
			scanf("%d",&guide);
			if(guide == 2) return;
		}			
	}
	return;
}

int guardPage(userData user){
	int a;
	while(1){
		system("clear");
		printf("Welcome , Mr.%s",user.name);
		printf("\n1. View Next Duty.");
		printf("\n2. Ask For Leave.");
		printf("\n3. Ask For Over Duty.");
		if(user.onLeave == 0 && user.onOverDuty == 0)
			printf("\n4. Check Leave/Over Duty request.");
		else
			printf("\n4. Check Leave/Over Duty request. -- You have unseen notifications..");
		printf("\n5. Leave Record Summary.");
		printf("\n6. LogOut!\n");
		scanf("%d",&a);
		if(guardInnerPage(a,user) == LOGOUT) return LOGOUT;	
	}
	return 0;
}

int guardInnerPage(int a,userData user){
	int b;
	while(1){
		system("clear");
		switch(a){
			case 1:			
				printf("Date Of Duty : %d, StartTime : %d EndTime : %d\n",user.day,user.time_start,user.time_end);
				printf("Press 1 to return to Previous Page\n");
				while(1){
					scanf("%d",&b);
					if(b == 1) return 1;
				}
				break;
			case 2:
				if(noOfLeaveRequests == 0){
					leaveRequest = (char**)(malloc(sizeof(char*)));
					noOfLeaveRequests++;
					leaveRequest[0] = user.identityNumber;
				}
				else{
					leaveRequest = realloc(leaveRequest,sizeof(char*)*(noOfLeaveRequests+1));
					leaveRequest[noOfLeaveRequests] = user.name;
					noOfLeaveRequests++;
				}
				printf("Applied For Leave!\n");
				system("sleep 1");
				system("clear");
				return 1;
			case 3:
				if(noOfOverDutyRequests == 0){
					overDutyRequest = (char**)(malloc(sizeof(char*)));
					noOfOverDutyRequests++;
					overDutyRequest[0] = user.identityNumber;
				}
				else{
					overDutyRequest = realloc(overDutyRequest,sizeof(char*)*(noOfOverDutyRequests+1));
					overDutyRequest[noOfOverDutyRequests] = user.name;
					noOfOverDutyRequests++;
				}
				printf("Applied For OverDuty!\n");
				system("sleep 1");
				system("clear");
				return 1;
			case 4:
				if(user.onLeave == -1){
					printf("\nLeave Request rejeted by Admin.");
					user.onLeave = 0;
				}
				if(user.onOverDuty == -1){
					printf("\nOver Duty Request rejected by Admin.");
					user.onOverDuty = 0;
				}
				if(user.onLeave == 0 && user.onOverDuty == 0){
					printf("NO Notifications");
				}
				if(user.onLeave == 1){
					printf("\nYour request for leave is granted.Check your upadated Date of Duty.");
					user.onLeave = 0;
				}
				if(user.onOverDuty == 1){
					printf("\nYour request for overtime is approved.Check your updated Date of Duty");
				}
				printf("\nPress 1 to return to Previous Page\n");
				while(1){
					scanf("%d",&b);
					if(b == 1) return 1;
				}
				return 1;
			case 5:
				printf("Leave Summary\n");
				printf("No of Leaves Taken : %d",user.noOfLeaves);
				printf("\nNo of Pending Leaves : %d",MAXLEAVES - user.noOfLeaves);
				printf("\nPress 1 to return to Previous Page\n");
				while(1){
					scanf("%d",&b);
					if(b == 1) return 1;
				}
				system("sleep 1");
				system("clear");
				return 1;
			case 6:
				return LOGOUT;
				break;
			default :
				printf("\nInvalid Input.Try Again.\n");	
		}
	}
	return 0;
}

//----------------------------//

int adminPage(){
	int a;
	while(1){
		system("clear");
		printf("1. Create Routine.\n");	
		printf("2. Manage Leave Requests (%d)\n",noOfLeaveRequests);
		printf("3. Manage OverDuty Requests (%d)\n",noOfOverDutyRequests);
		printf("4. Moniter Locations.\n");
		printf("5. LogOut!\n");
		scanf("%d",&a);
		if(adminInner(a) == LOGOUT) return LOGOUT;
	}
	return 0;
}

int adminInnerPage(int a){
	int i = 0,b;
	while(1){
		system("clear");
		switch(a){
			case 1:
				printf("TODO\n");
				return 1;
			case 2:
				system("clear");
				if(noOfLeaveRequests == 0){
					printf("No Leave Requests.\n");
					system("sleep 1");
					return 1;
				}
				printf("Leave Requests:\n");
				for( i = 0; i < noOfLeaveRequests; i++){
					printf("%d. %s",i+1,leaveRequest[i]);
				}
				printf("%d. Previous Page\n",noOfLeaveRequests+1);
				scanf("%d",&b);
				if(b == noOfLeaveRequests+1) return 0;
				else{
					printf("Press 1 to Approve\nPress 2 do disapprove\n");
					scanf("%d",&b);
					if(b == 1){
						system("clear");
						printf("Select a guard Guard to Assign.\n");
						system("sleep 1");
					}
					else{
						for(i  = 0; i < dataBaseSize; i++){
							if(database[i].name == leaveRequest[i]){
								database[i].leaveRequest = -1;
							}
						}
						for(i=b-1; i < noOfLeaveRequests-1; i++){
				            leaveRequest[i] = leaveRequest[i+1];
				        }
				        noOfLeaveRequests--;
					}
				}
			case 5:
				return LOGOUT;
			default:
				printf("\nInvalid Input.Try Again.\n");	
		}
	}
}

void adminLogin(){
	userData user;
	init(&user);
	int guide;
	while(1){
		system("clear");
		printf("Admin UserName : ");scanf("%s",user.name);
		printf("Password : ");scanf("%s",user.password);
		if(strcmp(user.name,Admin) == 0 && strcmp(user.password,AdminPassword) == 0){
			if(adminPage() == LOGOUT) return;
		}
		else{	
			printf("Invalid UserName or Password.\nPress 1 to retry.\nPress 2 to go to previous page.\nThankYou.\n");
			scanf("%d",&guide);
			if(guide == 2) return;
		}			
	}
	return;
}

//----------------------------//

void init(userData* user){
	user->name = (char*)malloc(20*sizeof(char));		
	user->identityNumber = (char*)malloc(20*sizeof(char));
	user->password = (char*)malloc(20*sizeof(char));
	user->onLeave = 0;
	user->onOverDuty = 0;
	user->noOfLeaves = 0;
	user->noOfOverDuty = 0;
	user->day = 0;
	user->time_end = 0;
	user->time_start = 0;
	return;
}

int searchDataBase(userData user){
	int i = 0;
	for(i = 0; i < dataBaseSize; i++){
		if(compareStruct(user,database[i])) return 1; 
	}
	return 0;
}

int compareStruct(userData a,userData b){
	if(strcmp(a.name,b.name) == 0 && strcmp(a.identityNumber,b.identityNumber) == 0 && strcmp(a.password,b.password) == 0){
		return 1;
	}
	return 0;
}

//----------------------------//

