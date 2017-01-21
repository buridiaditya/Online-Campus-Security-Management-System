
void createAdmin();
int adminPage();
void adminLogin();
int adminInnerPage();
void generateSchedule();

void createAdmin(){
	FILE* fp;
	userData user;
	strcpy(user.name,Admin);
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
							for(i = 1; i < dataBaseSize; i++){
								if(strcmp(database[i].name,overDutyRequest[0])==0){
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
						
						for(i = 1; i < dataBaseSize; i++){
							if(strcmp(database[i].name,leaveRequest[b]+8)==0){
								database[i].onLeave = 1;
								database[i].noOfLeaves++;
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
