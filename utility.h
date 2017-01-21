void init(userData* user);
int searchDataBase(userData user);
int compareStruct(userData a,userData b);

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

