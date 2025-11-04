#ifndef ATTENDANCE_GRADES_H
#define ATTENDANCE_GRADES_H

#include <stdbool.h>

#define MAX_DATE_LENGTH 11  // Format: YYYY-MM-DD
#define MAX_STATUS_LENGTH 20 // Example: "Present", "Absent", "Late"
#define MAX_assesTYPE 50  // Format: YYYY-MM-DD
#define MAX_REMARK 70     // Example: "Present", "Absent", "Late"

// Struct for Attendance
typedef struct Attendance {
    int attendance_id;
    int class_id;
    int student_id;
    char date[MAX_DATE_LENGTH];  // Initially empty
    char status[MAX_STATUS_LENGTH]; // Initially empty
    struct Attendance *next;
    struct Attendance *prev;
} Attendance;

typedef struct {
    int attendance_id;
    int class_id;
    int student_id;
    char date[MAX_DATE_LENGTH];  // Initially empty
    char status[MAX_STATUS_LENGTH]; // Initially empty
} AttendanceFile;

// Struct for Grades
typedef struct Grades {
    int grades_id;
    int class_id;
    int student_id;
    char assessment_type[MAX_assesTYPE];  // mid-score, final-score
    int score; 
    char remark[MAX_REMARK];
    struct Grades *next;
    struct Grades *prev; 
} Grades;

typedef struct {
    int grades_id;
    int class_id;
    int student_id;
    char assessment_type[MAX_assesTYPE];  // mid-score, final-score
    int score; 
    char remark[MAX_REMARK];
} GradesFile;

// Global pointers for the doubly linked lists
extern Attendance *attend_Stu_head;  // Pointer to the first attendance node
extern Attendance *attend_Stu_tail;  // Pointer to the last attendance node
extern Grades *grade_Stu_head;       // Pointer to the first grades node
extern Grades *grade_Stu_tail;       // Pointer to the last grades node

// Function prototypes for Attendance
void load_attendance_from_file();
void save_attendance_to_file();
void display_all_attendance();
void display_attendance_by_date(const char *date);
void insert_attendance();
void delete_attendance(int attendance_id);
void update_attendance_status(int attendance_id, const char *new_status);
int is_attendance_id_unique(int attendance_id);

// Function prototypes for Grades
void load_grades_from_file();
void save_grades_to_file();
void display_all_grades();
void display_grades_by_assessment_type(const char *assessment_type);
void insert_grades();
void delete_grades(int grades_id);
void update_grades_score_and_remark(int grades_id, int new_score, const char *new_remark);
void update_grades_assessment_type(int grades_id, const char *new_assessment_type);
int is_grades_id_unique(int grades_id);
int is_student_and_class_valid(int student_id, int class_id);

#endif // ATTENDANCE_GRADES_H