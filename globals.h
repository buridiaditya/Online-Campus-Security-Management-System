
#define NOOFGUARD 150
#define MAXLEAVES 5
#define LOGOUT -1
#define NOOFPLACES 1	

typedef struct{
	char name[20];
	char identityNumber[20];
	char password[20];
	int onLeave;
	int onOverDuty;
	int noOfLeaves;
	int noOfOverDuty;
	int salary;
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

