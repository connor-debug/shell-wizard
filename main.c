#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void deleteLine(FILE *srcFile, FILE *tempFile, int line)
{
    char buffer[256];
    int count = 1;

    while ((fgets(buffer, 256, srcFile)) != NULL)
    {
        if (line != count)
            fputs(buffer, tempFile);

        count++;
    }
}

int findLineAlias(char * string, char * fulldir)
{
	
const char * al = "alias ";

string = concat(al, string);
printf("Looking for: %s\n", string); 

FILE *fp = fopen(fulldir, "r");

    if(fp == NULL) {
        perror("Unable to open file!");
        exit(1);
    }

    char chunk[128];
    int line_count = 1;
    while(fgets(chunk, sizeof(chunk), fp) != NULL) {
         fputs(chunk, stdout);
         fputs("|\n", stdout);
         if (strncmp(chunk, string, strlen(string)) == 0)
            return line_count;
         line_count++;
    }
    fclose(fp);
    return 0;
}

int findLinePath(char * string, char * fulldir)
{

const char * pa = "PATH=";

string = concat(pa, string);
printf("Looking for: %s\n", string); 

FILE *fp = fopen(fulldir, "r");

    if(fp == NULL) {
        perror("Unable to open file!");
        exit(1);
    }

    char chunk[128];
    int line_count = 1;
    while(fgets(chunk, sizeof(chunk), fp) != NULL) {
         fputs(chunk, stdout);
         fputs("|\n", stdout);
         if (strncmp(chunk, string, strlen(string)) == 0)
            return line_count;
         line_count++;
    }
    fclose(fp);
    return 0;
}

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
	FILE *srcFile;
    FILE *tempFile;
	char albuf[32]; char buf [255];
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int n = 0;
	getchar();
	printf("Enter alias to be removed:\n");
	fflush(stdin);
	scanf("%s", albuf);
	getchar();
	
	n = findLineAlias(albuf, fulldir);
	
	if ( n != 0 )
		printf("\nFound alias at line %d.\n Removing Alias...\n",n);
		
	if ( n == 0 ) {
		printf("\nNo Aliases found. Exiting...\n");
		exit(1);
	}
	
	srcFile  = fopen(fulldir, "r");
    tempFile = fopen("delete-line.tmp", "w");
	
	if (srcFile == NULL || tempFile == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write previleges.\n");

        exit(EXIT_FAILURE);
    }


    // Move src file pointer to beginning
    rewind(srcFile);

    // Delete given line from file.
    deleteLine(srcFile, tempFile, n);


    /* Close all open files */
    fclose(srcFile);
    fclose(tempFile);


    /* Delete src file and rename temp file as src */
    remove("testrem.txt");
    rename("delete-line.tmp", fulldir);
	
	return 1;
}

int rempath(char * fulldir){
	
	FILE *fptr;
	FILE *srcFile;
    FILE *tempFile;
	char albuf[32]; char buf [255];
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int n = 0;
	getchar();
	printf("Enter path to be removed:\n");
	fflush(stdin);
	scanf("%s", albuf);
	getchar();
	
	n = findLinePath(albuf, fulldir);
	
	if ( n != 0 )
		printf("\nFound alias at line %d.\n Removing Path...\n",n);
		
	if ( n == 0 ) {
		printf("\nNo Paths found. Exiting...\n");
		exit(1);
	}
	
	srcFile  = fopen(fulldir, "r");
    tempFile = fopen("delete-line.tmp", "w");
	
	if (srcFile == NULL || tempFile == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write previleges.\n");

        exit(EXIT_FAILURE);
    }


    // Move src file pointer to beginning
    rewind(srcFile);

    // Delete given line from file.
    deleteLine(srcFile, tempFile, n);


    /* Close all open files */
    fclose(srcFile);
    fclose(tempFile);


    /* Delete src file and rename temp file as src */
    remove("testrem.txt");
    rename("delete-line.tmp", fulldir);
	
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
	fflush(stdin);
	
	scanf("%s", &userinput);
	
	if (strcmp(userinput,"1") == 0){
		alias(fulldir);
	}
	
	else if (strcmp(userinput,"2") == 0){
		printf("REMOVE ALIAS\n");
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
		printf("REMOVE PATH VARIABLES\n");
		rempath(fulldir);
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

