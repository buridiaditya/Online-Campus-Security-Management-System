#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef struct userData{
	char* name;
	char* identityNumber;
	char* password;
} userData; 	

void init(userData* user){
	user->name = (char*)malloc(20*sizeof(char));		
	user->identityNumber = (char*)malloc(20*sizeof(char));
	user->password = (char*)malloc(20*sizeof(char));
	return;
}

char* Admin = "Buridi";
char* AdminPassword = "buridi";

userData* database;
int dataBaseSize = 0;

void guardRegister(){
	userData user;
	init(&user);
	system("clear");
	printf("\nUsername : ");scanf("%s",user.name);
	printf("Indentity Number : ");scanf("%s",user.identityNumber);
	printf("Password : ");scanf("%s",user.password);
	
	if(realloc(database,(dataBaseSize+1)*sizeof(userData))){
		printf("User Registered Succesfully!\n");
	}
	else{
		printf("Could Not Register.Out of Space!!\n");
	}
	database[dataBaseSize] = user;
	dataBaseSize++;
	return;
}

int adminPage(){
	system("clear");
	printf("NO DATA HERE!\n");
	printf("Press 2 to LogOut!\n");
	if(getc() == '2') return 1;
}

void adminLogin(){
	userData user;
	init(&user);
	int guide;
	while(1){
		system("clear");
		printf("\nAdmin UserName : ");scanf("%s",user.name);
		printf("Password : ");scanf("%s",user.password);
		if(strcmp(user.name,Admin) == 0 && strcmp(user.password,AdminPassword) == 0){
			if(adminPage() == 1) return;
		}
		else{	
			printf("Invalid UserName or Password.\nPress 1 to retry.\nPress 2 to go to previous page.\nThankYou.\n");
			scanf("%d",&guide);
			if(guide == 2) return;
		}			
	}
	return;
}


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
		scanf("%d",&option);
		switch(option){
			case 1 :
			//	guardLogin();
				break;	
			case 2 :
				guardRegister();
				break;
			case 3 : 
				adminLogin();	
				break;	
			}
	}
	return;	
}

int main(){
	run();	
	return 0;
}
