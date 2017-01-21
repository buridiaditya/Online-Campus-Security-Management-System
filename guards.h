void guardRegister();
void guardLogin();
int guardPage(userData* user);
int guardInnerPage(int a,userData* user);

void guardRegister(){
	userData user;
	
	FILE* data;
	
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
	data = fopen("data.bin","ab");
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
	FILE* ptr ;
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
						for(i = 0; i < noOfLeaveRequests; i++){
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
						ptr = fopen("leaves.bin","ab");
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

