/*
 ============================================================================
 Name        : StudentGradeSystem.c
 Author      : John Satterfield
 Description : Enhanced artifact from CS-410: Software Reverse Engineering
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> // in gcc, use < conio.h > in turboc
#include <string.h>

// Student list with grades
struct Student {
  int id;
  char name[10];
  char grade[2];
};

static struct termios old, new;
 
// Initialize new terminal i/o settings
void initTermios(int echo) {
  tcgetattr(0, &old); // grab old terminal i/o settings
  new = old; // make new settings same as old settings
  new.c_lflag &= ~ICANON; // disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; // set echo mode
  tcsetattr(0, TCSANOW, &new); // use these new terminal i/o settings now
}
 
// Restore old terminal i/o settings
void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}
 
// Read 1 character - echo defines echo mode
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
 
// Read 1 character without echo
char getch(void) {
  return getch_(0);
}
 
// Read 1 character with echo
char getche(void) {
  return getch_(1);
}

// Function to get password
void getPassword(char *pass) {
  int c=0, len=0;
  char buff[30]={0},ch;
  
  while((ch=getch())!='\n') {
    if (ch==0x7f) { // use 0x08 in turboc (WINDOWS)
      if (len==0) {
        continue;
      }
      printf("\b \b");
      len--;
      continue;
    }
    printf("%c",'*');
    pass[len]=ch;
    len++;
  }
  pass[len]='\0';
}

// Function to validate user's input
int CheckUserPermissionAccess(char* name, char* p) {
  int access = 0;
  FILE *fp;
  int line_num = 1, find_result=0;
	char temp[512];
  
  // Open file
  fp = fopen("pw.txt", "r");
  if (fp == NULL) printf("File doesn't exist\n");
  
  // Create pattern to search
  char* credentials = name;
  strcat(credentials, " ");
  strcat(credentials, p);
  //printf("\n%s",credentials);
  
  while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, credentials)) != NULL) {
      printf("\nUser Authorized.\n");
      access = 1;
      find_result++;
      return 1;
		}
		line_num++;
	}
  // No matching results - deny
  if(find_result == 0) {
		printf("\nUser Not Authorized.\n");
    return 0;
	}
 
  // If file still open, close it
  if (fp) {
    fclose(fp);
  }
}

// Function to read the user's name and password
int ReadUserInfo() {
  // Initialize variables
  char name[10], pass[10];
  int access = 0;

  // Get user name
  printf("\nEnter name: ");
  scanf("%s",name);
  
  // Get password
  printf ("Enter password: ");
  getPassword(pass);
  getPassword(pass);
  
  // Pass credentials for validation
  access = CheckUserPermissionAccess(name, pass);

  return access;
}

// Function to display menu
void printMenu() {
  printf("\nPlease select a menu option below\n");
  printf("1. List students and grades\n");
  printf("2. Update student grades\n");
  printf("3. Find students by grade\n");
  printf("0. Exit the program.\n");
  printf("\nEnter choice: ");
}

// Function to get student data
void getStudents(struct Student records[80], int s) {
  int i;
  
  // Loop through number of records requested
  for (i = 0; i < s; i++) {
    printf("\nEnter data for student record #%d", i + 1);
    records[i].id = i;
    
    // Get student name
    printf("\nEnter name: ");
    scanf("%s", records[i].name);
    
    // Get student grade
    printf("Enter grade: ");
    scanf("%s", records[i].grade);
  } 
}

// Function to display students and grades
void displayStudents(struct Student records[80], int s) {
  int i;  
  
  printf("\nID\tName\t\tGrade\n");
  printf("===\t=====\t\t======\n");

  // Display students and grades
  for (i=0; i<s; i=i+1) {
    printf("%d\t%s\t\t%s\n", records[i].id, records[i].name, records[i].grade);
  }
}

// Function to find students with specific grade
void findStudentGrades(struct Student records[80], int s, char grade[2]) {
  int i;
  
  // Print title
  printf("\nName\t\tGrade\n");
  printf("=====\t\t======\n");
  
  // Loop through students
  for (i=0; i<s; i++) {
    // If student grade matches
    if (strcmp(records[i].grade,grade) == 0) {
      printf("%s\t\t%s\n", records[i].name, records[i].grade);
      return;
    }
  }
  // No students found
  printf("No students found with grade %s\n", grade);
}

// Function to update student grades
void updateStudentGrades(struct Student records[80], int s) {
  int i;
  
  // Loop through students
  printf("\nEnter new grade for each student\n");
  for (i=0; i<s; i=i+1) {
    printf("%s: ", records[i].name); // Print name
    scanf("%s", records[i].grade); // Get new grade
  }
}

// Function main
int main() {
  // Initialize variables and struct
  struct Student records[20];
  int access = 0, n, input, choice=0, id;
  char g[2];
  
  // Log in as user
  access = ReadUserInfo();
  
  // If access not equal to 1, exit program
  if (access != 1) {
    exit(0);
  }
  
  // Display initial welcome message
  printf("\nWelcome Professor\n");

  printf("How many records do you want to enter?: ");
  scanf("%d", &n);
  getStudents(records, n);
  
  // Loop Menu
  while(choice != 5) {
    printMenu();
    scanf("%d",&choice);
    
    // Switch case for menu options
    switch(choice) {
    case 1:
        // Display students
        displayStudents(records, n);
        break;
    case 2:
        // Update student grades
        updateStudentGrades(records, n);
        break;
    case 3:
        // Find students by grade
        printf("\nEnter grade to search: ");
        scanf("%s", g);
        findStudentGrades(records, n, g);
        break;
    case 0:
        // Exit program
        printf("\nHave a great day!\n");
        exit(0);
        break;
    default:
        // Catch invalid input
        printf("\nPlease enter valid option.\n");
        break;
    }    
  }

  return 0;
}
