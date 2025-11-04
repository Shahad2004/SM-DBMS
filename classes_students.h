#ifndef CLASSES_STUDENTS_H
#define CLASSES_STUDENTS_H

#include <stdbool.h>

#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 11  // Format: YYYY-MM-DD
#define MAX_CLASS_NAME_LENGTH 50
#define MAX_ACADEMIC_YEAR_LENGTH 10
#define MAX_SCHEDULED_DAYS_LENGTH 20

// Struct for Class (used for file storage)
typedef struct {
    int class_id;
    int teacher_id;  // Can be NULL (represented as -1)
    char class_name[MAX_CLASS_NAME_LENGTH];
    char academic_year[MAX_ACADEMIC_YEAR_LENGTH];  // Format: YYYY-YYYY
    char scheduled_days[MAX_SCHEDULED_DAYS_LENGTH];  // Example: "Sun, Tue, Thu"
} ClassFile;

// Struct for Class (used for linked list)
typedef struct Class {
    int class_id;
    int teacher_id;  // Can be NULL (represented as -1)
    char class_name[MAX_CLASS_NAME_LENGTH];
    char academic_year[MAX_ACADEMIC_YEAR_LENGTH];
    char scheduled_days[MAX_SCHEDULED_DAYS_LENGTH];
    struct Class *next;
    struct Class *prev;
} Class;

// Struct for Student (used for file storage)
typedef struct {
    int student_id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char date_of_birth[MAX_DATE_LENGTH];  // Format: YYYY-MM-DD
    char enrollment_date[MAX_DATE_LENGTH];
} StudentFile;

// Struct for Student (used for linked list)
typedef struct Student {
    int student_id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char date_of_birth[MAX_DATE_LENGTH];  // Format: YYYY-MM-DD
    char enrollment_date[MAX_DATE_LENGTH];
    struct Student *next;
    struct Student *prev;
} Student;

typedef struct Class_Students {
    int Class_Id;
    int Student_Id;
    struct Class_Students *next;  // Corrected pointer type
    struct Class_Students *prev;  // Corrected pointer type
} Class_Students;


typedef struct{
     int Class_Id ;
     int Student_Id;
} Class_StudentsFile;

// Global pointers for the doubly linked lists
extern Class *class_head;  // Pointer to the first class node
extern Class *class_tail;  // Pointer to the last class node
extern Student *student_head;  // Pointer to the first student node
extern Student *student_tail;  // Pointer to the last student node
extern Class_Students *class_Stu_head;
extern Class_Students *class_Stu_tail;
// Function prototypes
int is_class_id_unique(int class_id);
void insert_class();
void display_classes();
void update_teacher_id_for_class(int class_id, int teacher_id);
void delete_class(int class_id);
void update_class_name(int class_id, const char *new_class_name);
void update_scheduled_days(const char *class_name, const char *new_scheduled_days);
void display_classes_without_teacher();
void load_classes_from_file();
void save_classes_to_file();

// Student-related function prototypes
int is_student_id_unique(int student_id);
void insert_student();
void display_students();
void display_student_by_id(int student_id);
void delete_student(int student_id);
void load_students_from_file();
void save_students_to_file();
void search_students_by_year_of_birth(int year);

//Class_Student Functions
void load_class_students_from_file();
void save_class_students_to_file();
int class_exists(int class_id);
int student_exists(int student_id);
int class_student_exists(int class_id, int student_id);
void insert_class_student();
void display_classes_for_student(int student_id);
void display_all_classes_with_students();
void update_student_class_assignment(int student_id, int old_class_id, int new_class_id);
void delete_student_class_assignment(int student_id, int class_id);
#endif // CLASSES_STUDENTS_H