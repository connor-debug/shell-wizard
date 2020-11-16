#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>

void detectShells(){
	system("cat /etc/shells");
}

void remove_new_line(char* string){
    size_t length = strlen(string);
    if((length > 0) && (string[length-1] == '\n')){
        string[length-1] ='\0';
    }
}

void alias(char * fulldir){
	
	FILE *fptr;
	char albuf[32]; char combuf[32];
	const char ch = '\'';
	printf("Enter Alias:");
	scanf("%s", &albuf);
	getchar();
	printf("Enter Command:");
	fgets(combuf, sizeof combuf, stdin);
	
	remove_new_line(combuf);

	fptr = fopen(fulldir, "a");
	
	if (fptr == NULL){
		printf("Error.\n");
		exit(1);
	}
	
	fprintf(fptr,"\nalias %s='%s'\n", albuf, combuf);
	fclose(fptr);
	
	printf("Successfully Configured %s\n ",fulldir);
	
}

int remalias(char * fulldir){
	
	FILE *fptr;
	char albuf[32]; char buf [255];
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	getchar();
	printf("Enter alias to be removed:\n");
	scanf("%s", albuf);
	
	fptr = fopen(fulldir, "r");
	
	if (fptr == NULL){
		printf("Error.\n");
		exit(1);
	}
	
	while(fgets(buf, 255, fptr)){
	printf("%s", buf);
	}
	
	fclose(fptr);
	
	printf("Successfully Configured %s\n ",fulldir);
	
	return 1;
}

int showalias(char * fulldir){
	
	FILE *fptr;
	char buf [255];
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	fptr = fopen(fulldir, "r");
	
	if (fptr == NULL){
		printf("Error.\n");
		exit(1);
	}
	
	while(fgets(buf, 255, fptr)){
		if(strncmp(buf, "alias ", 6) == 0){
			printf("%s", buf);
		}
	}
	
	printf("\n");
	fclose(fptr);
	
	return 1;
}

int showpath(char * fulldir){
	
	FILE *fptr;
	char buf [255];
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	fptr = fopen(fulldir, "r");
	
	if (fptr == NULL){
		printf("Error.\n");
		exit(1);
	}
	
	while(fgets(buf, 255, fptr)){
		if(strncmp(buf, "PATH", 4) == 0){
			printf("%s", buf);
		}
	}
	
	printf("\n");
	fclose(fptr);
	
	return 1;
}

int path(char * fulldir){
	
	FILE *fptr;
	char dirpath[64];
	
	printf("Enter Directory Path:\n");
	scanf("%s", &dirpath);
	
	fptr = fopen(fulldir, "a");
	
	if (fptr == NULL){
		printf("Error.\n");
		exit(1);
	}
	
	fprintf(fptr,"PATH=%s:$PATH", dirpath);
	fclose(fptr);
	
	printf("Successfully Configured %s\n ",fulldir);
	
	return 1;
}

int main(int argc, char *argv[]){
	char userinput[2];
	char *bash = "/.bashrc";
	struct passwd *pw = getpwuid(getuid());
	char *homedir = pw->pw_dir;
	printf("Welcome to Shell Wizard.\n Home Directory: %s.\n", homedir);
	char * fulldir = strcat(homedir, bash);
	
	while(1){
	memset(&userinput[0], 0, sizeof(userinput));
	printf("Select an option:\n");
	printf("1. Add Alias\n2. Remove Alias\n3. Show Configured Aliases\n");
	printf("4. Add Path Variable\n5. Remove Path Variable\n6. Show Path Variables\n");
	printf("7. Views Shells\n8. Quit\n");
	
	scanf("%s", &userinput);
	
	if (strcmp(userinput,"1") == 0){
		alias(fulldir);
	}
	
	else if (strcmp(userinput,"2") == 0){
		remalias(fulldir);
	}
	
	else if (strcmp(userinput,"3") == 0){
		printf("Retrieving Aliases...\n");
		showalias(fulldir);
	}
	
	else if (strcmp(userinput,"4") == 0){
		printf("Retrieving Aliases...\n");
		path(fulldir);
	}
	
	else if (strcmp(userinput,"5") == 0){
		printf("Retrieving Aliases...\n");
		showalias(fulldir);
	}
	
	else if (strcmp(userinput,"6") == 0){
		printf("Retrieving Path Variables...\n");
		showpath(fulldir);
	}
	
	else if (strcmp(userinput,"7") == 0){
		printf("Detecting Shells...\n");
		detectShells();
	}
	
	else if (strcmp(userinput,"8") == 0){
		printf("Exiting...\n");
		break;
	}
	
}
	
}

