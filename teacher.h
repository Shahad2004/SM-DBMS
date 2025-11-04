#ifndef TEACHER_H
#define TEACHER_H

#include <stdbool.h>
#include <pthread.h>

// Define constants for maximum lengths
#define MAX_NAME_LENGTH 50
#define MAX_SUBJECT_LENGTH 50
#define MAX_PHONE_LENGTH 15

// Struct for Teacher (used for file storage)
typedef struct {
    int teacher_id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char subject_expertise[MAX_SUBJECT_LENGTH];
} TeacherFile;

// Struct for Teacher_PhoneNumber (used for file storage)
typedef struct {
    int teacher_id;
    char phone_number[MAX_PHONE_LENGTH];
} Teacher_PhoneNumberFile;

// Struct for Teacher_PhoneNumber (used for linked list)
typedef struct TeacherPhone {
    int teacher_id;
    char phone_number[MAX_PHONE_LENGTH];
    struct TeacherPhone *next;
    struct TeacherPhone *prev;
} TeacherPhone;

// Struct for Teacher (used for linked list)
typedef struct Teacher {
    int teacher_id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char subject_expertise[MAX_SUBJECT_LENGTH];
    struct Teacher *next;
    struct Teacher *prev;
} Teacher;

// Teacher functions
void insert_teacher(); // Renamed to teacher_insert in implementation
void display(); // Renamed to teacher_display in implementation
void delete_teacher(); // Renamed to teacher_delete in implementation
int teacher_id_exists(int teacher_id);
int is_teacher_id_unique(int teacher_id);
Teacher* select_teacher_by_id(int teacher_id);
void select_teachers_by_subject(const char *subject);
void display_teacher_by_id(int teacher_id);
void load_teachers_from_file(); // Renamed to teacher_load_from_file in implementation
void save_teachers_to_file(); // Renamed to teacher_save_to_file in implementation

// Teacher phone number functions
void insert_phone_number(); // Renamed to phone_insert in implementation
void display_phone_numbers(); // Renamed to phone_display in implementation
void delete_phone_numbers_by_teacher_id(int teacher_id); // Renamed to phone_delete_by_teacher_id in implementation
void delete_phone_number_by_phone_number(const char *phone_number); // Renamed to phone_delete_by_phone_number in implementation
void delete_phone_number_by_teacher_id_and_phone_number(int teacher_id, const char *phone_number); // Renamed to phone_delete_by_teacher_id_and_phone_number in implementation
void update_phone_number_by_phone_number();
void update_phone_number_by_teacher_id_and_phone_number();
void load_phone_numbers_from_file(); // Renamed to phone_load_from_file in implementation
void save_phone_numbers_to_file(); // Renamed to phone_save_to_file in implementation
int is_phone_number_unique(const char *phone_number);

#endif // TEACHER_H