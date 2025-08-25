#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Number_Of_Subjects 5
#define FILE_NAME "Student_Record_Management.csv"
#define MAX_LINE_LENGTH 300
const char Subjects[Number_Of_Subjects][10] = {"English", "Hindi", "Maths", "Science", "CS"};

typedef struct Student_Record_Management
{
    char Name[100];
    int Roll_Number;
    float Marks[Number_Of_Subjects];
} Student;

void Clear_Buffer();
int Duplicate_Roll_Number_Check(int *);
void fgets_Correction(char *);
void Main_Menu_Interface(int *);
int Add_New_Student();
void Print_Data(Student *);
int View_All_Students();
int Search_Student(int);
void Grade_Calculation(float);

void Grade_Calculation(float Percentage)
{
    if (Percentage > 90.0)
    {
        printf("Grade : A\n");
    }
    else if (Percentage > 80.0)
    {
        printf("Grade : B\n");
    }
    else if (Percentage > 65.0)
    {
        printf("Grade : C\n");
    }
    else if (Percentage > 50.0)
    {
        printf("Grade : D\n");
    }
    else if (Percentage > 40.0)
    {
        printf("Grade : E\n");
    }
    else
    {
        printf("Grade : F\n");
    }
}

void Clear_Buffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

int Duplicate_Roll_Number_Check(int *temp)
{
    char Line[MAX_LINE_LENGTH];
    FILE *ptr;
    ptr = fopen(FILE_NAME, "r");
    if (ptr == NULL)
    {
        printf("Not Able To Open File For Duplicate Check " FILE_NAME "\n");
        return 2;
    }
    while (fgets(Line, sizeof(Line), ptr) != NULL)
    {
        char Line_Temp[MAX_LINE_LENGTH];
        strcpy(Line_Temp, Line);
        char *token;
        token = strtok(Line_Temp, ",");
        token = strtok(NULL, ",");
        int Roll_Number = atoi(token);
        if (Roll_Number == *temp)
        {
            fclose(ptr);
            return 1;
        }
    }
    fclose(ptr);
    return 0;
}

void fgets_Correction(char *Name)
{
    if (Name == NULL) // If Name Is Empty It Won't Check Anything
    {
        return;
    }
    int String_Length = strlen(Name);
    if (String_Length > 0 && Name[String_Length - 1] == '\n')
    {
        Name[String_Length - 1] = '\0';
    }
}

void Main_Menu_Interface(int *choice)
{
    printf("\n===== Student Record Management =====\n");
    printf("1. Add New Student\n");
    printf("2. View All Students\n");
    printf("3. Search Student By Roll Number\n");
    printf("4. Exit\n");
    printf("Enter Your Choice : ");
    scanf("%d", choice);
    Clear_Buffer();
}

int Add_New_Student()
{
    Student S;
    FILE *ptr;
    int check;
    ptr = fopen(FILE_NAME, "a+");
    if (ptr == NULL)
    {
        printf("Not Able To Create / Write In File " FILE_NAME "\n");
        return 1;
    }
    fseek(ptr, 0, SEEK_END);
    long long size = ftell(ptr);
    if (size == 0)
    {
        fprintf(ptr, "Name,Roll Number,English Marks,Hindi Marks,Maths Marks,Science Marks,CS Marks\n");
    }
    printf("\n1. Add New Student\n");
    printf("Enter The Name Of Student : ");
    if (fgets(S.Name, sizeof(S.Name), stdin))
    {
        fgets_Correction(&S.Name[0]);
    }
    do
    {
        printf("Enter The Roll Number : ");
        scanf("%d", &S.Roll_Number);
        Clear_Buffer();
        check = Duplicate_Roll_Number_Check(&S.Roll_Number);
        if (check == 1)
        {
            printf("Existing Roll Number Found\n");
            printf("Please Try Again With Valid Roll Number\n");
        }
    } while (check != 0);
    for (int i = 0; i < Number_Of_Subjects; i++)
    {
        do
        {
            printf("Enter Marks for %s (Subject %d) : ", Subjects[i], i + 1);
            scanf("%f", &S.Marks[i]);
            Clear_Buffer();
            if (S.Marks[i] < 0 || S.Marks[i] > 100)
            {
                printf("Invalid Input Please Try Again\n");
            }
        } while (S.Marks[i] < 0 || S.Marks[i] > 100);
    }
    fprintf(ptr, "%s,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n", S.Name, S.Roll_Number, S.Marks[0], S.Marks[1], S.Marks[2], S.Marks[3], S.Marks[4]);
    printf("\nPress Enter To Continue\n");
    Clear_Buffer();
    fclose(ptr);
    return 0;
}

void Print_Data(Student *S)
{
    float Total_Marks = 0.0;
    printf("\n===== Student Record =====\n");
    printf("Student Name : %s\n", S->Name);
    printf("Student Roll Number : %d\n", S->Roll_Number);
    for (int i = 0; i < Number_Of_Subjects; i++)
    {
        printf("Student Marks In Subject %d (%s) : %.2f\n", i + 1, Subjects[i], S->Marks[i]);
        Total_Marks += S->Marks[i];
    }
    float Percentage = Total_Marks / Number_Of_Subjects;
    printf("Pecentage Of Marks Scored By Student : %.2f%\n", Percentage);
    Grade_Calculation(Percentage);
}

int View_All_Students()
{
    char Line[MAX_LINE_LENGTH];
    Student S;
    FILE *ptr;
    ptr = fopen(FILE_NAME, "r");
    if (ptr == NULL)
    {
        printf("Not Able To Find / Read From File " FILE_NAME "\n");
        return 1;
    }
    fgets(Line, sizeof(Line), ptr); // Skips Header Line
    while (fgets(Line, sizeof(Line), ptr) != NULL)
    {
        char Line_Temp[MAX_LINE_LENGTH];
        strcpy(Line_Temp, Line);
        char *token;
        token = strtok(Line_Temp, ",");
        strcpy(S.Name, token);
        token = strtok(NULL, ",");
        S.Roll_Number = atoi(token);
        for (int i = 0; i < Number_Of_Subjects; i++)
        {
            token = strtok(NULL, ",");
            S.Marks[i] = atof(token);
        }
        Print_Data(&S);
    }
    printf("\nData Was Read Successfully\n");
    Clear_Buffer();
    printf("\nPress Enter To Continue\n");
    fclose(ptr);
    return 0;
}

int Search_Student(int Entered_Roll_Number)
{
    char Line[MAX_LINE_LENGTH];
    Student S;
    FILE *ptr;
    int found = 0;
    ptr = fopen(FILE_NAME, "r");
    if (ptr == NULL)
    {
        printf("Not Able To Find / Read From File " FILE_NAME "\n");
        return 1;
    }
    fgets(Line, sizeof(Line), ptr); // Skips Header Line
    while (fgets(Line, sizeof(Line), ptr) != NULL)
    {
        char Line_Temp[MAX_LINE_LENGTH];
        strcpy(Line_Temp, Line);
        fgets_Correction(Line);
        char *token;
        token = strtok(Line_Temp, ",");
        strcpy(S.Name, token);
        token = strtok(NULL, ",");
        S.Roll_Number = atoi(token);
        for (int i = 0; i < Number_Of_Subjects; i++)
        {
            token = strtok(NULL, ",");
            S.Marks[i] = atof(token);
        }
        if (Entered_Roll_Number == S.Roll_Number)
        {
            found = 1;
            Print_Data(&S);
        }
    }
    if (found == 0)
    {
        printf("Data Not Found\n");
    }
    Clear_Buffer();
    printf("\nPress Enter To Continue\n");
    fclose(ptr);
    return 0;
}

int main()
{
    int choice;
    int Entered_Roll_Number;
    do
    {
        Main_Menu_Interface(&choice);
        switch (choice)
        {
        case 1:
            if (Add_New_Student() != 0)
            {
                printf("Error Adding New Student\n");
            }
            break;
        case 2:
            if (View_All_Students() != 0)
            {
                printf("Error Viewing All Students\n");
            }
            break;
        case 3:
            printf("Enter The Roll Number To Search For Particular Data\n");
            scanf("%d", &Entered_Roll_Number);
            Clear_Buffer();
            if (Search_Student(Entered_Roll_Number) != 0)
            {
                printf("Error Searching For Student\n");
            }
            break;
        case 4:
            printf("Exiting Program\n");
            break;
        default:
            printf("Invalid Input");
            break;
        }
    } while (choice != 4);
    return 0;
}