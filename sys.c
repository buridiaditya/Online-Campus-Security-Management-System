#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define NOOFGUARD 150
#define MAXLEAVES 5
#define LOGOUT -1
#define NOOFPLACES 1	

///// DATATYPE /////

typedef struct{
	char name[20];
	char identityNumber[20];
	char password[20];
	int onLeave;
	int onOverDuty;
	int noOfLeaves;
	int noOfOverDuty;
} userData; 	

////// GLOBAL DATA ////
	
char* Admin = "Buridi";
char* AdminPassword = "buridi";
userData* database;
struct tm currentTime;
int dataBaseSize = 0;
int noOfLeaveRequests = 0;
int noOfOverDutyRequests = 0;
char** leaveRequest;
char** overDutyRequest;
char dutyDiagram[7][3][NOOFPLACES][20];
char* days[7] = {"Monday" , "Tuesday" , "Wednesday" , "Thursday" , "Friday" , "Saturday" , "Sunday" };

///// UTILITY FUNCTION //////

void init(userData* user);
int searchDataBase(userData user);
int compareStruct(userData a,userData b);

//// GUARD FUNCTIONS ///// 

void guardRegister();
void guardLogin();
int guardPage(userData* user);
int guardInnerPage(int a,userData* user);

///// ADMIN FUNCTOINS /////

void createAdmin();
int adminPage();
void adminLogin();
int adminInnerPage();
void generateSchedule();

/// RUN && LOAD ////

void load(){
	userData user;init(&user);
	char* input = (char*) malloc(sizeof(char)*20);
	FILE* lea;
	FILE* over;
	FILE* ptr;
	FILE* timetable;
	lea = fopen("leaves.bin","rb");
	ptr = fopen("data.bin","rb");
	over = fopen("overduty.bin","rb");
	timetable = fopen("schedule.bin","rb");
	if(lea == NULL){
		lea = fopen("leaves.bin","wb");
		fclose(lea);
		lea = fopen("leaves.bin","rb");
	}
	if(over == NULL){
		over = fopen("overduty.bin","wb");
		fclose(over);
		over = fopen("overduty.bin","rb");
	}
	if(timetable == NULL){
		timetable = fopen("schedule.bin","wb");
		fclose(timetable);
		timetable = fopen("schedule.bin","rb");
	}
	fread(dutyDiagram,sizeof(dutyDiagram),1,timetable);
	while(fread(input,sizeof(char)*20,1,lea)){
		if(feof(lea)!=0) break;
		if(noOfLeaveRequests == 0) leaveRequest = (char**)malloc(sizeof(char*));
		else leaveRequest = realloc(leaveRequest,(noOfLeaveRequests+1)*sizeof(char*));
		leaveRequest[noOfLeaveRequests] = input;
		input = (char*) malloc(sizeof(char)*20);
		noOfLeaveRequests++;
	}
	while(fread(input,sizeof(char)*20,1,over)){
		if(feof(over)!=0) break;
		if(noOfOverDutyRequests == 0) overDutyRequest = (char**)malloc(sizeof(char*));
		else overDutyRequest = realloc(overDutyRequest,(noOfOverDutyRequests+1)*sizeof(char*));
		overDutyRequest[noOfOverDutyRequests] = input;
		input = (char*) malloc(sizeof(char)*20);
		noOfOverDutyRequests++;
	}
	while(fread(&user,sizeof(userData),1,ptr)){
		if(feof(ptr)!=0)
    		break;
    	if(dataBaseSize == 0) database = (userData*) malloc(sizeof(userData));
		else database = realloc(database,(dataBaseSize+1)*sizeof(userData));
		database[dataBaseSize] = user;
		dataBaseSize++;
	}
	fclose(ptr);
	fclose(lea);
	fclose(over);
	fclose(timetable);
	// system("sleep 3");
	return;
}

void destructor(){
	FILE* ptr;
	FILE* ptr1;
	FILE* ptr2;
	int i = 0,j=0,k=0;
	ptr = fopen("schedule.bin","wb");
	ptr1 = fopen("leaves.bin","wb");
	ptr2 = fopen("overduty.bin","wb");
	for( i = 0; i < noOfLeaveRequests; i++){
		fwrite(leaveRequest[i],sizeof(char)*20,1,ptr1);
	}
	for( i = 0; i < noOfOverDutyRequests; i++){
		fwrite(overDutyRequest[i],sizeof(char)*20,1,ptr2);
	}
	for(i = 0; i < 7; i++){
		for(j = 0; j < 3; j++){
			for(k = 0; k < NOOFPLACES; k++){
				fwrite(dutyDiagram[i][j][k],sizeof(char)*20,1,ptr);
			}
		}
	}
	fclose(ptr);
	fclose(ptr1);
	fclose(ptr2);
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
	// createAdmin();
	load();
	run();	
	return 0;
}

//----------------------------//

void guardRegister(){
	userData user;
	
	FILE* data;
	data = fopen("data.bin","ab");
	
	init(&user);
	int b;
	system("clear");
	printf("Username : ");scanf("%s",user.name);
	printf("UserID : ");scanf("%s",user.identityNumber);
	printf("Password : ");scanf("%s",user.password);
	database = realloc(database,(dataBaseSize+1)*sizeof(userData));
	database[dataBaseSize] = user;
	dataBaseSize++;
	printf("User Registered Succesfully!\n");
	
	fwrite(&database[dataBaseSize-1],sizeof(userData),1,data);
	fclose(data);
	printf("\nPress 1 to return to Previous Page\n");
	while(1){
		scanf("%d",&b);
		if(b == 1) return;
		else printf("Invalid Input!\n");
	}
	return;
}

void guardLogin(){
	userData user;
	init(&user);
	int guide,a;
	while(1){
		system("clear");
		printf("UserName : ");scanf("%s",user.name);
		printf("Identity Number : ");scanf("%s",user.identityNumber);
		printf("Password : ");scanf("%s",user.password);
		a = searchDataBase(user);
		if(a != -1){
			if(guardPage(&database[a]) == LOGOUT) return;
		}
		else{	
			printf("Invalid UserName or Password.\nPress 1 to retry.\nPress 2 to go to previous page.\nThankYou.\n");
			scanf("%d",&guide);
			if(guide == 2) return;
		}			
	}
	return;
}

int guardPage(userData* user){
	int a;
	while(1){
		system("clear");
		printf("Welcome , Mr. %s",user->name);
		printf("\n1. View Duty.");
		printf("\n2. Ask For Over Duty.");
		if(user->onLeave == 0 && user->onOverDuty == 0)
			printf("\n3. Check Leave/Over Duty request.");
		else
			printf("\n3. Check Leave/Over Duty request. -- You have unseen notifications.");
		printf("\n4. Leave Record Summary.");
		printf("\n5. LogOut!\n");
		scanf("%d",&a);
		if(guardInnerPage(a,user) == LOGOUT) return LOGOUT;	
	}
	return 0;
}

int guardInnerPage(int a,userData* user){
	int b,i,j,k;
	char* leaveDate = (char*)malloc(sizeof(char)*20);
	char* encryptedDate = (char*)malloc(sizeof(char)*20);
	FILE* ptr = fopen("leaves.bin","ab");
	while(1){
		system("clear");
		switch(a){
			case 1:	
				for(i = currentTime.tm_wday; i < 7 ; i++){
					for(j = currentTime.tm_hour / 24; j < 3 ; j++){
						for(k = 0; k < NOOFPLACES ; k++){
							if(strcmp(user->name,dutyDiagram[i][j][k]) == 0){
								printf("Day : %s --- Time : %d:00 - %d:00 --- Location No : %d\n",days[i],(j*8)%24,((j+1)*8)%24,k);
							}
						}
					}
				}
				printf("Press 1 to return to Previous Page.\n");
				printf("Press 2 to apply for leave.\n");
				while(1){
					scanf("%d",&b);
					if(b == 1) return 1;
					else if(b==2){
						printf("Enter the Day Number(D),startTime(SS),LocationID in D:SS:LLL\n");
						scanf(" %s",leaveDate);
						leaveDate = strcat(leaveDate,user->name);
						for(int i = 0; i < noOfLeaveRequests; i++){
							if(strcmp(leaveDate,leaveRequest[i]) == 0) {
								printf("Already Applied for this leave slot. Wait for Admin Reply.\n");
								system("sleep 1");
								return 1;
							}
						}
						if(noOfLeaveRequests == 0){
							leaveRequest = (char**)(malloc(sizeof(char*)));
							noOfLeaveRequests++;
							leaveRequest[0] = leaveDate;
						}
						else{
							leaveRequest = realloc(leaveRequest,sizeof(char*)*(noOfLeaveRequests+1));
							leaveRequest[noOfLeaveRequests] = leaveDate;
							noOfLeaveRequests++;
						}
						fwrite(leaveDate,sizeof(char)*20,1,ptr);
						fclose(ptr);
						printf("Applied For Leave!\n");
						system("sleep 1");
						system("clear");
						return 1;
					}
					else printf("Invalid Input!");
				}
			case 2:
				if(noOfOverDutyRequests == 0){
					overDutyRequest = (char**)(malloc(sizeof(char*)));
					noOfOverDutyRequests++;
					overDutyRequest[0] = user->identityNumber;
				}
				else{
					overDutyRequest = realloc(overDutyRequest,sizeof(char*)*(noOfOverDutyRequests+1));
					overDutyRequest[noOfOverDutyRequests] = user->name;
					noOfOverDutyRequests++;
				}
				printf("Applied For OverDuty!\n");
				system("sleep 1");
				system("clear");
				return 1;
			case 3:
				if(user->onLeave == 0 && user->onOverDuty == 0){
					printf("NO Notifications.\n");
				}
				if(user->onLeave == -1){
					printf("\nLeave Request rejeted by Admin.");
					user->onLeave = 0;
				}
				if(user->onOverDuty == -1){
					printf("\nOver Duty Request rejected by Admin.");
					user->onOverDuty = 0;
				}	
				else if(user->onLeave == 1){
					printf("\nYour request for leave is granted.Check your upadated Date of Duty.");
					user->onLeave = 0;
				}
				else if(user->onOverDuty == 1){
					printf("\nYour request for overtime is approved.Check your updated Date of Duty");
				}
				printf("\nPress 1 to return to Previous Page\n");
				while(1){
					scanf("%d",&b);
					if(b == 1) return 1;
				}
				return 1;
			case 4:
				printf("Leave Summary\n");
				printf("No of Leaves Taken : %d",user->noOfLeaves);
				printf("\nNo of Pending Leaves : %d",MAXLEAVES - user->noOfLeaves);
				printf("\nPress 1 to return to Previous Page\n");
				while(1){
					scanf("%d",&b);
					if(b == 1) return 1;
				}
				system("sleep 1");
				system("clear");
				return 1;
			case 5:
				return LOGOUT;
				break;
			default :
				printf("\nInvalid Input.Try Again.\n");	
		}
	}
	return 0;
}

//----------------------------//

void createAdmin(){
	FILE* fp;
	userData user;
	strcpy(user.name ,Admin);
	strcpy(user.identityNumber ,"ADMIN");
	strcpy(user.password , AdminPassword);
	fp = fopen("data.bin","wb");
	fwrite(&user,sizeof(userData),1,fp);
	fclose(fp);
	return;
}

int adminPage(){
	int a;
	while(1){
		system("clear");
		printf("1. Create Routine.\n");	
		printf("2. Manage Leave Requests (%d)\n",noOfLeaveRequests);
		printf("3. Check OverDuty Requests (%d)\n",noOfOverDutyRequests);
		printf("4. Moniter Locations.\n");
		printf("5. LogOut!\n");
		scanf("%d",&a);
		if(adminInnerPage(a) == LOGOUT) return LOGOUT;
	}
	return 0;
}

int adminInnerPage(int a){
	int i = 0,b,c,j,k,d,count,wDay,sTime,Location;
	char* temp;
	while(1){
		system("clear");
		switch(a){
			case 1:
				system("clear");
				generateSchedule();
				printf("Schedule Generated!\n");
				system("sleep 1");
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
					printf("%d. %s\n",i+1,(leaveRequest[i]+8));
				}
				printf("%d. Previous Page\n",noOfLeaveRequests+1);
				scanf("%d",&b);
				if(b == noOfLeaveRequests+1) return 0;
				else{
					printf("You have %d Guard OverDuty Requests.\n",noOfOverDutyRequests);
					printf("Press 1 to Approve\nPress 2 do disapprove\n");
					scanf("%d",&c);
					b--;
					if(c == 1){
						wDay = leaveRequest[b][0]-'0';
						sTime = ((leaveRequest[b][2]-'0')*10 + (leaveRequest[b][3]-'0'))/8;
						Location = (leaveRequest[b][5]-'0')*100+(leaveRequest[b][6]-'0')*10+(leaveRequest[b][7]-'0');
						system("clear");
						if(noOfOverDutyRequests != 0){
							strcpy(dutyDiagram[wDay][sTime][Location] ,overDutyRequest[0]);
							for(int i = 1; i < dataBaseSize; i++){
								if(strcmp(database[i].name,overDutyRequest[b])==0){
									database[i].onOverDuty = 1;
									break;
								}
							}
							for(i=0; i < noOfOverDutyRequests-1; i++){
					            overDutyRequest[i] = overDutyRequest[i+1];
					        }
					        noOfOverDutyRequests--;
						}
						else{
							printf("Select a Guard to Assign.\n");
							for(i = 1; i < dataBaseSize; i++){
								if(strcmp(database[i].name,leaveRequest[b]+8) != 0){
									count++;
									printf("%d . %s\n",i,database[i].name);
								}
							}
							scanf("%d",&d);
							strcpy(dutyDiagram[wDay][sTime][Location] , database[d].name);
						}
						for(int i = 1; i < dataBaseSize; i++){
							if(strcmp(database[i].name,leaveRequest[b]+8)==0){
								database[i].onLeave = 1;
								break;
							}
						}
						for(i=b-1; i < noOfLeaveRequests-1; i++){
				            leaveRequest[i] = leaveRequest[i+1];
				        }
				        noOfLeaveRequests--;
						system("sleep 1");
					}
					else{
						for(i  = 1; i < dataBaseSize; i++){
							if(strcmp(database[i].name,leaveRequest[b-1]) == 0){
								database[i].onLeave = -1;
							}
						}
						for(i=b-1; i < noOfLeaveRequests-1; i++){
				            leaveRequest[i] = leaveRequest[i+1];
				        }
				        noOfLeaveRequests--;
					}
					break;
				}
			case 3:
				system("clear");
				if(noOfOverDutyRequests == 0){
					printf("No OverDuty Requests.\n");
					system("sleep 1");
					return 1;
				}
				printf("OverDuty Requests:\n");
				for( i = 0; i < noOfOverDutyRequests; i++){
					printf("%d. %s\n",i+1,overDutyRequest[i]);
				}
				printf("%d. Previous Page\n",noOfOverDutyRequests+1);
				scanf("%d",&b);
				if(b == noOfOverDutyRequests+1) return 0;
				if(noOfLeaveRequests == 0){ 
					printf("No leave requests found. Cannot create an empty Slot for overDuty!\n");
					system("sleep 1");
					return 0;
				}
				else{
					printf("Press 1 to Approve\nPress 2 do disapprove\n");
					scanf("%d",&c);
					if(c == 1){
						temp = overDutyRequest[0];
						overDutyRequest[0] = overDutyRequest[b-1];
						overDutyRequest[b-1] = temp;
						printf("You will be redirected to the Leave Requests Page.Kindly accept a leave request to assign overDuty.");
						a = 2;
						system("sleep 2");
					}
					else{
						for(i  = 1; i < dataBaseSize; i++){
							if(strcmp(database[i].name,overDutyRequest[b-1]) == 0){
								database[i].onOverDuty = -1;
							}
						}
						for(i=b-1; i < noOfOverDutyRequests-1; i++){
				            overDutyRequest[i] = overDutyRequest[i+1];
				        }
				        noOfOverDutyRequests--;
					}
					break;
				}
			case 4 : 
				system("clear");
				for(i = 0; i < 7; i++){
					printf("Day %d : \n",i+1);
					for(j = 0; j < 3; j++){
						if(j==0) printf("00:00-08:00 : \n");
						if(j==1) printf("08:00-16:00 : \n");
						if(j==2) printf("16:00-24:00 : \n");
						for( k = 0;k < NOOFPLACES; k++){
							printf("  Place %d : %s\n",k,dutyDiagram[i][j][k]);
						}
					}
				}
				printf("\nPress 1 for previous page\n");
				while(1){
					scanf("%d",&b);
					if(b == 1) return 0;
					else printf("\nInvalid Input!");
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

void generateSchedule(){	
	FILE* ptr = fopen("schedule.bin","wb");
	int count = 0,i,j,k;
	for(i = 0; i < 7; i++){
		for(j = 0; j < 3; j++){
			for(k = 0; k < NOOFPLACES; k++){
				if(count == dataBaseSize-1) count = 0;
				strcpy(dutyDiagram[i][j][k] ,database[count+1].name);
				fwrite(dutyDiagram[i][j][k],sizeof(char)*20,1,ptr);
				count++;
			}
		}
	}
	fclose(ptr);
	return;
}
//----------------------------//

void init(userData* user){
	user->onLeave = 0;
	user->onOverDuty = 0;
	user->noOfLeaves = 0;
	user->noOfOverDuty = 0;
	return;
}

int searchDataBase(userData user){
	int i = 0;
	for(i = 0; i < dataBaseSize; i++){
		if(compareStruct(user,database[i])) return i; 
	}
	return -1;
}

int compareStruct(userData a,userData b){
	if(strcmp(a.name,b.name) == 0 && strcmp(a.identityNumber,b.identityNumber) == 0 && strcmp(a.password,b.password) == 0){
		return 1;
	}
	return 0;
}

//----------------------------//

