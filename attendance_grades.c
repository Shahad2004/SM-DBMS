#include "classes_students.h"
#include "teacher.h"
#include "attendance_grades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Attendance *attend_Stu_head = NULL;
Attendance *attend_Stu_tail = NULL;
Grades *grade_Stu_head = NULL;
Grades *grade_Stu_tail = NULL;



// Function implementations
void load_attendance_from_file() {
    const char *file_path = "attendance.dat";  // Use a relative path
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("No existing attendance data found.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    AttendanceFile *buffer = (AttendanceFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    // Read the file into the buffer
    fread(buffer, sizeof(AttendanceFile), file_size / sizeof(AttendanceFile), file);
    fclose(file);

    // Calculate the number of attendance records in the file
    int num_records = file_size / sizeof(AttendanceFile);

    // Add each attendance record from the buffer to the doubly linked list
    for (int i = 0; i < num_records; i++) {
        Attendance *new_attendance = (Attendance *)malloc(sizeof(Attendance));
        if (new_attendance == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }

        // Copy data from the buffer to the new attendance record
        new_attendance->attendance_id = buffer[i].attendance_id;
        new_attendance->class_id = buffer[i].class_id;
        new_attendance->student_id = buffer[i].student_id;
        strcpy(new_attendance->date, buffer[i].date);
        strcpy(new_attendance->status, buffer[i].status);
        new_attendance->next = NULL;
        new_attendance->prev = NULL;

        // Add the new attendance record to the doubly linked list
        if (attend_Stu_head == NULL) {
            attend_Stu_head = new_attendance;
            attend_Stu_tail = new_attendance;
        } else {
            attend_Stu_tail->next = new_attendance;
            new_attendance->prev = attend_Stu_tail;
            attend_Stu_tail = new_attendance;
        }
    }

    free(buffer);  // Free the buffer
    printf("Attendance records loaded from file.\n");
}

void save_attendance_to_file() {
    const char *file_path = "C:\\Projects\\DBMS_project\\attendance.dat";
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Attendance *current = attend_Stu_head;
    while (current != NULL) {
        // Save only the data fields (excluding next and prev pointers)
        AttendanceFile temp;
        temp.attendance_id = current->attendance_id;
        temp.class_id = current->class_id;
        temp.student_id = current->student_id;
        strcpy(temp.date, current->date);
        strcpy(temp.status, current->status);

        fwrite(&temp, sizeof(AttendanceFile), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Attendance records saved to file.\n");
}

void display_all_attendance() {
    if (attend_Stu_head == NULL) {
        printf("No attendance records found.\n");
        return;
    }

    printf("\nAttendance Records:\n");
    Attendance *current = attend_Stu_head;
    while (current != NULL) {
        printf("Attendance ID: %d, Class ID: %d, Student ID: %d, Date: %s, Status: %s\n",
               current->attendance_id,
               current->class_id,
               current->student_id,
               current->date,
               current->status);
        current = current->next;
    }
}

void display_attendance_by_date(const char *date) {
    if (attend_Stu_head == NULL) {
        printf("No attendance records found.\n");
        return;
    }

    printf("\nAttendance Records for Date %s:\n", date);
    Attendance *current = attend_Stu_head;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->date, date) == 0) {
            printf("Attendance ID: %d, Class ID: %d, Student ID: %d, Status: %s\n",
                   current->attendance_id,
                   current->class_id,
                   current->student_id,
                   current->status);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No attendance records found for the date %s.\n", date);
    }
}

int is_attendance_id_unique(int attendance_id) {
    Attendance *current = attend_Stu_head;
    while (current != NULL) {
        if (current->attendance_id == attendance_id) {
            return 0;  // ID is not unique
        }
        current = current->next;
    }
    return 1;  // ID is unique
}

void insert_attendance() {
    Attendance *new_attendance = (Attendance *)malloc(sizeof(Attendance));
    if (new_attendance == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Input attendance details
    printf("Enter Attendance ID: ");
    scanf("%d", &new_attendance->attendance_id);

    // Check if the attendance ID is unique
    if (!is_attendance_id_unique(new_attendance->attendance_id)) {
        printf("Error: Attendance ID %d already exists!\n", new_attendance->attendance_id);
        free(new_attendance);
        return;
    }

    printf("Enter Class ID: ");
    scanf("%d", &new_attendance->class_id);

    // Check if the class exists
    if (!class_exists(new_attendance->class_id)) {
        printf("Error: Class ID %d does not exist in the Class table!\n", new_attendance->class_id);
        free(new_attendance);
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &new_attendance->student_id);

    // Check if the student exists
    if (!student_exists(new_attendance->student_id)) {
        printf("Error: Student ID %d does not exist in the Student table!\n", new_attendance->student_id);
        free(new_attendance);
        return;
    }

    // Clear the input buffer
    while (getchar() != '\n');

    printf("Enter Date (YYYY-MM-DD): ");
    fgets(new_attendance->date, MAX_DATE_LENGTH, stdin);
    new_attendance->date[strcspn(new_attendance->date, "\n")] = '\0';  // Remove newline

    printf("Enter Status (e.g., Present, Absent, Late): ");
    fgets(new_attendance->status, MAX_STATUS_LENGTH, stdin);
    new_attendance->status[strcspn(new_attendance->status, "\n")] = '\0';  // Remove newline

    // Initialize next and prev pointers
    new_attendance->next = NULL;
    new_attendance->prev = NULL;

    // Add the new attendance record to the doubly linked list
    if (attend_Stu_head == NULL) {
        attend_Stu_head = new_attendance;
        attend_Stu_tail = new_attendance;
    } else {
        attend_Stu_tail->next = new_attendance;
        new_attendance->prev = attend_Stu_tail;
        attend_Stu_tail = new_attendance;
    }

    // Append the new attendance record to the file
    const char *file_path = "attendance.dat";
    FILE *file = fopen(file_path, "ab");  // Open in append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Create an AttendanceFile struct to write to the file
    AttendanceFile temp;
    temp.attendance_id = new_attendance->attendance_id;
    temp.class_id = new_attendance->class_id;
    temp.student_id = new_attendance->student_id;
    strcpy(temp.date, new_attendance->date);
    strcpy(temp.status, new_attendance->status);

    // Write the attendance record to the file
    fwrite(&temp, sizeof(AttendanceFile), 1, file);
    fclose(file);

    printf("Attendance record added successfully!\n");
}
void delete_attendance(int attendance_id) {
    if (attend_Stu_head == NULL) {
        printf("No attendance records found. Cannot delete.\n");
        return;
    }

    Attendance *current = attend_Stu_head;
    int found = 0;

    // Search for the attendance record in the linked list
    while (current != NULL) {
        if (current->attendance_id == attendance_id) {
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Attendance record with ID %d not found.\n", attendance_id);
        return;
    }

    // Delete the attendance record from the linked list
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        attend_Stu_head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        attend_Stu_tail = current->prev;
    }

    // Free the memory allocated for the deleted attendance record
    free(current);
    printf("Attendance record with ID %d deleted successfully from the linked list.\n", attendance_id);

    // Now, update the file to remove the deleted attendance record
    const char *file_path = "attendance.dat";  // Use a relative path
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate the number of attendance records in the file
    int num_records = file_size / sizeof(AttendanceFile);

    // Read all attendance records from the file into a buffer
    AttendanceFile *buffer = (AttendanceFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    fread(buffer, sizeof(AttendanceFile), num_records, file);
    fclose(file);

    // Open the file in write mode to overwrite it
    file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        free(buffer);  // Free the buffer before returning
        return;
    }

    // Write all attendance records except the deleted one back to the file
    int records_written = 0;
    for (int i = 0; i < num_records; i++) {
        if (buffer[i].attendance_id != attendance_id) {
            fwrite(&buffer[i], sizeof(AttendanceFile), 1, file);
            records_written++;
        }
    }

    fclose(file);
    free(buffer);

    if (records_written == num_records) {
        printf("Attendance record with ID %d not found in the file.\n", attendance_id);
    } else {
        printf("Attendance record with ID %d deleted successfully from the file.\n", attendance_id);
    }
}

void update_attendance_status(int attendance_id, const char *new_status) {
    Attendance *current = attend_Stu_head;
    int updated = 0;

    while (current != NULL) {
        if (current->attendance_id == attendance_id) {
            // Update the status
            strcpy(current->status, new_status);
            updated = 1;
            break;
        }
        current = current->next;
    }

    if (updated) {
        printf("Attendance status updated successfully for Attendance ID %d.\n", attendance_id);
        // Save the updated list to the file
        save_attendance_to_file();
    } else {
        printf("Attendance ID %d not found.\n", attendance_id);
    }
}
// ==================== Grades Functions ====================

// Load grades from file into the linked list
void load_grades_from_file() {
    const char *file_path = "grades.dat";  // Use a relative path
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("No existing grades data found.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    GradesFile *buffer = (GradesFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    // Read the file into the buffer
    fread(buffer, sizeof(GradesFile), file_size / sizeof(GradesFile), file);
    fclose(file);

    // Calculate the number of grades records in the file
    int num_records = file_size / sizeof(GradesFile);

    // Add each grades record from the buffer to the doubly linked list
    for (int i = 0; i < num_records; i++) {
        Grades *new_grades = (Grades *)malloc(sizeof(Grades));
        if (new_grades == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }

        // Copy data from the buffer to the new grades record
        new_grades->grades_id = buffer[i].grades_id;
        new_grades->class_id = buffer[i].class_id;
        new_grades->student_id = buffer[i].student_id;
        strcpy(new_grades->assessment_type, buffer[i].assessment_type);
        new_grades->score = buffer[i].score;
        strcpy(new_grades->remark, buffer[i].remark);
        new_grades->next = NULL;
        new_grades->prev = NULL;

        // Add the new grades record to the doubly linked list
        if (grade_Stu_head == NULL) {
            grade_Stu_head = new_grades;
            grade_Stu_tail = new_grades;
        } else {
            grade_Stu_tail->next = new_grades;
            new_grades->prev = grade_Stu_tail;
            grade_Stu_tail = new_grades;
        }
    }

    free(buffer);  // Free the buffer
    printf("Grades records loaded from file.\n");
}

// Save grades from the linked list to the file
void save_grades_to_file() {
    const char *file_path = "grades.dat";
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Grades *current = grade_Stu_head;
    while (current != NULL) {
        // Save only the data fields (excluding next and prev pointers)
        GradesFile temp;
        temp.grades_id = current->grades_id;
        temp.class_id = current->class_id;
        temp.student_id = current->student_id;
        strcpy(temp.assessment_type, current->assessment_type);
        temp.score = current->score;
        strcpy(temp.remark, current->remark);

        fwrite(&temp, sizeof(GradesFile), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Grades records saved to file.\n");
}

// Display all grades records
void display_all_grades() {
    if (grade_Stu_head == NULL) {
        printf("No grades records found.\n");
        return;
    }

    printf("\nGrades Records:\n");
    Grades *current = grade_Stu_head;
    while (current != NULL) {
        printf("Grades ID: %d, Class ID: %d, Student ID: %d, Assessment Type: %s, Score: %d, Remark: %s\n",
               current->grades_id,
               current->class_id,
               current->student_id,
               current->assessment_type,
               current->score,
               current->remark);
        current = current->next;
    }
}

// Display grades records by assessment type
void display_grades_by_assessment_type(const char *assessment_type) {
    if (grade_Stu_head == NULL) {
        printf("No grades records found.\n");
        return;
    }

    printf("\nGrades Records for Assessment Type %s:\n", assessment_type);
    Grades *current = grade_Stu_head;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->assessment_type, assessment_type) == 0) {
            printf("Grades ID: %d, Class ID: %d, Student ID: %d, Score: %d, Remark: %s\n",
                   current->grades_id,
                   current->class_id,
                   current->student_id,
                   current->score,
                   current->remark);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No grades records found for the assessment type %s.\n", assessment_type);
    }
}

// Check if a grades ID is unique
int is_grades_id_unique(int grades_id) {
    Grades *current = grade_Stu_head;
    while (current != NULL) {
        if (current->grades_id == grades_id) {
            return 0;  // ID is not unique
        }
        current = current->next;
    }
    return 1;  // ID is unique
}

// Check if a student and class exist in their respective tables
int is_student_and_class_valid(int student_id, int class_id) {
    // Implement these functions in classes_students.h
    return student_exists(student_id) && class_exists(class_id);
}

// Insert a new grades record
void insert_grades() {
    Grades *new_grades = (Grades *)malloc(sizeof(Grades));
    if (new_grades == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Input grades details
    printf("Enter Grades ID: ");
    scanf("%d", &new_grades->grades_id);

    // Check if the grades ID is unique
    if (!is_grades_id_unique(new_grades->grades_id)) {
        printf("Error: Grades ID %d already exists!\n", new_grades->grades_id);
        free(new_grades);
        return;
    }

    printf("Enter Class ID: ");
    scanf("%d", &new_grades->class_id);

    printf("Enter Student ID: ");
    scanf("%d", &new_grades->student_id);

    // Check if the student and class exist
    if (!is_student_and_class_valid(new_grades->student_id, new_grades->class_id)) {
        printf("Error: Student ID %d or Class ID %d does not exist in their respective tables!\n",
               new_grades->student_id, new_grades->class_id);
        free(new_grades);
        return;
    }

    // Clear the input buffer
    while (getchar() != '\n');

    printf("Enter Assessment Type (e.g., mid-score, final-score): ");
    fgets(new_grades->assessment_type, MAX_assesTYPE, stdin);
    new_grades->assessment_type[strcspn(new_grades->assessment_type, "\n")] = '\0';  // Remove newline

    printf("Enter Score: ");
    scanf("%d", &new_grades->score);

    // Clear the input buffer
    while (getchar() != '\n');

    printf("Enter Remark: ");
    fgets(new_grades->remark, MAX_REMARK, stdin);
    new_grades->remark[strcspn(new_grades->remark, "\n")] = '\0';  // Remove newline

    // Initialize next and prev pointers
    new_grades->next = NULL;
    new_grades->prev = NULL;

    // Add the new grades record to the doubly linked list
    if (grade_Stu_head == NULL) {
        grade_Stu_head = new_grades;
        grade_Stu_tail = new_grades;
    } else {
        grade_Stu_tail->next = new_grades;
        new_grades->prev = grade_Stu_tail;
        grade_Stu_tail = new_grades;
    }

    // Append the new grades record to the file
    const char *file_path = "grades.dat";
    FILE *file = fopen(file_path, "ab");  // Open in append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Create a GradesFile struct to write to the file
    GradesFile temp;
    temp.grades_id = new_grades->grades_id;
    temp.class_id = new_grades->class_id;
    temp.student_id = new_grades->student_id;
    strcpy(temp.assessment_type, new_grades->assessment_type);
    temp.score = new_grades->score;
    strcpy(temp.remark, new_grades->remark);

    // Write the grades record to the file
    fwrite(&temp, sizeof(GradesFile), 1, file);
    fclose(file);

    printf("Grades record added successfully!\n");
}

// Delete a grades record by ID
void delete_grades(int grades_id) {
    if (grade_Stu_head == NULL) {
        printf("No grades records found. Cannot delete.\n");
        return;
    }

    Grades *current = grade_Stu_head;
    int found = 0;

    // Search for the grades record in the linked list
    while (current != NULL) {
        if (current->grades_id == grades_id) {
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Grades record with ID %d not found.\n", grades_id);
        return;
    }

    // Delete the grades record from the linked list
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        grade_Stu_head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        grade_Stu_tail = current->prev;
    }

    // Free the memory allocated for the deleted grades record
    free(current);
    printf("Grades record with ID %d deleted successfully from the linked list.\n", grades_id);

    // Now, update the file to remove the deleted grades record
    const char *file_path = "grades.dat";  // Use a relative path
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate the number of grades records in the file
    int num_records = file_size / sizeof(GradesFile);

    // Read all grades records from the file into a buffer
    GradesFile *buffer = (GradesFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    fread(buffer, sizeof(GradesFile), num_records, file);
    fclose(file);

    // Open the file in write mode to overwrite it
    file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        free(buffer);  // Free the buffer before returning
        return;
    }

    // Write all grades records except the deleted one back to the file
    int records_written = 0;
    for (int i = 0; i < num_records; i++) {
        if (buffer[i].grades_id != grades_id) {
            fwrite(&buffer[i], sizeof(GradesFile), 1, file);
            records_written++;
        }
    }

    fclose(file);
    free(buffer);

    if (records_written == num_records) {
        printf("Grades record with ID %d not found in the file.\n", grades_id);
    } else {
        printf("Grades record with ID %d deleted successfully from the file.\n", grades_id);
    }
}

// Update the score and remark of a grades record by ID
void update_grades_score_and_remark(int grades_id, int new_score, const char *new_remark) {
    Grades *current = grade_Stu_head;
    int updated = 0;

    while (current != NULL) {
        if (current->grades_id == grades_id) {
            // Update the score and remark
            current->score = new_score;
            strcpy(current->remark, new_remark);
            updated = 1;
            break;
        }
        current = current->next;
    }

    if (updated) {
        printf("Grades score and remark updated successfully for Grades ID %d.\n", grades_id);
        // Save the updated list to the file
        save_grades_to_file();
    } else {
        printf("Grades ID %d not found.\n", grades_id);
    }
}

// Update the assessment type of a grades record by ID
void update_grades_assessment_type(int grades_id, const char *new_assessment_type) {
    Grades *current = grade_Stu_head;
    int updated = 0;

    while (current != NULL) {
        if (current->grades_id == grades_id) {
            // Update the assessment type
            strcpy(current->assessment_type, new_assessment_type);
            updated = 1;
            break;
        }
        current = current->next;
    }

    if (updated) {
        printf("Grades assessment type updated successfully for Grades ID %d.\n", grades_id);
        // Save the updated list to the file
        save_grades_to_file();
    } else {
        printf("Grades ID %d not found.\n", grades_id);
    }
}