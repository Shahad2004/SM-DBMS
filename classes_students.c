#include "classes_students.h"
#include "teacher.h"
#include "attendance_grades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global pointers for the doubly linked lists
Class *class_head = NULL;  // Pointer to the first class node
Class *class_tail = NULL;  // Pointer to the last class node
Student *student_head = NULL;  // Pointer to the first student node
Student *student_tail = NULL;  // Pointer to the last student node
Class_Students *class_Stu_head = NULL;
Class_Students *class_Stu_tail = NULL;



// Function to check if a class ID is unique
int is_class_id_unique(int class_id) {
    Class *current = class_head;
    while (current != NULL) {
        if (current->class_id == class_id) {
            return 0; // Class ID is not unique
        }
        current = current->next;
    }
    return 1; // Class ID is unique
}void insert_class() {
    Class *new_class = (Class *)malloc(sizeof(Class));
    if (new_class == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Input class details
    printf("Enter Class ID: ");
    scanf("%d", &new_class->class_id);

    // Check if the class ID is unique
    if (!is_class_id_unique(new_class->class_id)) {
        printf("Error: Class ID %d already exists!\n", new_class->class_id);
        free(new_class);
        return;
    }

    // Prompt for teacher ID (optional)
    printf("Enter Teacher ID (or -1 for NULL): ");
    scanf("%d", &new_class->teacher_id);

    // Validate teacher ID
    if (new_class->teacher_id != -1) {
        if (!teacher_id_exists(new_class->teacher_id)) {
            printf("Error: Teacher ID %d does not exist in the Teacher table!\n", new_class->teacher_id);
            free(new_class);
            return;
        }
    }

    // Clear the input buffer
    while (getchar() != '\n');

    // Input class name
    printf("Enter Class Name: ");
    fgets(new_class->class_name, MAX_CLASS_NAME_LENGTH, stdin);
    new_class->class_name[strcspn(new_class->class_name, "\n")] = '\0';  // Remove newline

    // Input academic year
    printf("Enter Academic Year (YYYY-YYYY): ");
    fgets(new_class->academic_year, MAX_ACADEMIC_YEAR_LENGTH, stdin);
    new_class->academic_year[strcspn(new_class->academic_year, "\n")] = '\0';  // Remove newline

    // Input scheduled days
    printf("Enter Scheduled Days (e.g., Sun, Tue, Thu): ");
    fgets(new_class->scheduled_days, MAX_SCHEDULED_DAYS_LENGTH, stdin);
    new_class->scheduled_days[strcspn(new_class->scheduled_days, "\n")] = '\0';  // Remove newline

    // Initialize next and prev pointers
    new_class->next = NULL;
    new_class->prev = NULL;

    // Add the new class to the doubly linked list
    if (class_head == NULL) {
        class_head = new_class;
        class_tail = new_class;
    } else {
        class_tail->next = new_class;
        new_class->prev = class_tail;
        class_tail = new_class;
    }

    // Append the new class to the file
    const char *file_path = "C:\\Projects\\DBMS_project\\classes.dat";
    FILE *file = fopen(file_path, "ab");  // Open in append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Create a ClassFile struct to write to the file
    ClassFile temp;
    temp.class_id = new_class->class_id;
    temp.teacher_id = new_class->teacher_id;  // Will be -1 if not provided
    strcpy(temp.class_name, new_class->class_name);
    strcpy(temp.academic_year, new_class->academic_year);
    strcpy(temp.scheduled_days, new_class->scheduled_days);

    // Write the class to the file
    fwrite(&temp, sizeof(ClassFile), 1, file);
    fclose(file);

    printf("Class added successfully!\n");
}
void update_teacher_id_for_class(int class_id, int teacher_id) {
    Class *current = class_head;
    int updated = 0;

    while (current != NULL) {
        if (current->class_id == class_id) {
            // Update the teacher_id (can be -1 to represent NULL)
            current->teacher_id = teacher_id;
            updated = 1;
            break;
        }
        current = current->next;
    }

    if (updated) {
        printf("Teacher ID updated successfully for Class ID %d.\n", class_id);
        // Save the updated list to the file
        save_classes_to_file();
    } else {
        printf("Class ID %d not found.\n", class_id);
    }
}

// Function to delete a class by class_id
void delete_class(int class_id) {
    if (class_head == NULL) {
        printf("No classes found. Cannot delete.\n");
        return;
    }

    Class *current = class_head;
    int found = 0;

    // Search for the class in the linked list
    while (current != NULL) {
        if (current->class_id == class_id) {
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Class with ID %d not found.\n", class_id);
        return;
    }

    // Delete the class from the linked list
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        class_head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        class_tail = current->prev;
    }

    // Free the memory allocated for the deleted class
    free(current);
    printf("Class with ID %d deleted successfully from the linked list.\n", class_id);

    // Now, update the file to remove the deleted class
    const char *file_path = "C:\\Projects\\DBMS_project\\classes.dat";
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate the number of classes in the file
    int num_classes = file_size / sizeof(ClassFile);

    // Read all classes from the file into a buffer
    ClassFile *buffer = (ClassFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    fread(buffer, sizeof(ClassFile), num_classes, file);
    fclose(file);

    // Open the file in write mode to overwrite it
    file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        free(buffer);
        return;
    }

    // Write all classes except the deleted one back to the file
    int classes_written = 0;
    for (int i = 0; i < num_classes; i++) {
        if (buffer[i].class_id != class_id) {
            fwrite(&buffer[i], sizeof(ClassFile), 1, file);
            classes_written++;
        }
    }

    fclose(file);
    free(buffer);

    if (classes_written == num_classes) {
        printf("Class with ID %d not found in the file.\n", class_id);
    } else {
        printf("Class with ID %d deleted successfully from the file.\n", class_id);
    }
}

// Function to update class name by class_id
void update_class_name(int class_id, const char *new_class_name) {
    Class *current = class_head;
    int updated = 0;

    while (current != NULL) {
        if (current->class_id == class_id) {
            // Update the class name
            strcpy(current->class_name, new_class_name);
            updated = 1;
            break;
        }
        current = current->next;
    }

    if (updated) {
        printf("Class name updated successfully for Class ID %d.\n", class_id);
        // Save the updated list to the file
        save_classes_to_file();
    } else {
        printf("Class ID %d not found.\n", class_id);
    }
}

// Function to update scheduled days by class name
void update_scheduled_days(const char *class_name, const char *new_scheduled_days) {
    Class *current = class_head;
    int updated = 0;

    while (current != NULL) {
        if (strcmp(current->class_name, class_name) == 0) {
            // Update the scheduled days
            strcpy(current->scheduled_days, new_scheduled_days);
            updated = 1;
            break;
        }
        current = current->next;
    }

    if (updated) {
        printf("Scheduled days updated successfully for Class Name %s.\n", class_name);
        // Save the updated list to the file
        save_classes_to_file();
    } else {
        printf("Class Name %s not found.\n", class_name);
    }
}

// Function to load classes from the file into the linked list
void load_classes_from_file() {
    const char *file_path = "classes.dat";
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("No existing class data found.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    ClassFile *buffer = (ClassFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    // Read the file into the buffer
    fread(buffer, 1, file_size, file);
    fclose(file);

    // Calculate the number of classes in the file
    int num_classes = file_size / sizeof(ClassFile);

    // Add each class from the buffer to the doubly linked list
    for (int i = 0; i < num_classes; i++) {
        Class *new_class = (Class *)malloc(sizeof(Class));
        if (new_class == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }

        // Copy data from the buffer to the new class
        new_class->class_id = buffer[i].class_id;
        new_class->teacher_id = buffer[i].teacher_id;
        strcpy(new_class->class_name, buffer[i].class_name);
        strcpy(new_class->academic_year, buffer[i].academic_year);
        strcpy(new_class->scheduled_days, buffer[i].scheduled_days);
        new_class->next = NULL;
        new_class->prev = NULL;

        // Add the new class to the doubly linked list
        if (class_head == NULL) {
            class_head = new_class;
            class_tail = new_class;
        } else {
            class_tail->next = new_class;
            new_class->prev = class_tail;
            class_tail = new_class;
        }
    }

    free(buffer);  // Free the buffer
    printf("Classes loaded from file.\n");
}

// Function to save classes to the file
void save_classes_to_file() {
    const char *file_path = "classes.dat";
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Class *current = class_head;
    while (current != NULL) {
        // Save only the data fields (excluding next and prev pointers)
        ClassFile temp;
        temp.class_id = current->class_id;
        temp.teacher_id = current->teacher_id;
        strcpy(temp.class_name, current->class_name);
        strcpy(temp.academic_year, current->academic_year);
        strcpy(temp.scheduled_days, current->scheduled_days);

        fwrite(&temp, sizeof(ClassFile), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Classes saved to file.\n");
}
// Function to check if a student ID is unique
int is_student_id_unique(int student_id) {
    Student *current = student_head;
    while (current != NULL) {
        if (current->student_id == student_id) {
            return 0; // Student ID is not unique
        }
        current = current->next;
    }
    return 1; // Student ID is unique
}

// Function to insert a new student
void insert_student() {
    Student *new_student = (Student *)malloc(sizeof(Student));
    if (new_student == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Input student details
    printf("Enter Student ID: ");
    scanf("%d", &new_student->student_id);

    // Check if the student ID is unique
    if (!is_student_id_unique(new_student->student_id)) {
        printf("Error: Student ID %d already exists!\n", new_student->student_id);
        free(new_student);
        return;
    }

    // Clear the input buffer
    while (getchar() != '\n');

    printf("Enter First Name: ");
    fgets(new_student->first_name, MAX_NAME_LENGTH, stdin);
    new_student->first_name[strcspn(new_student->first_name, "\n")] = '\0';  // Remove newline

    printf("Enter Last Name: ");
    fgets(new_student->last_name, MAX_NAME_LENGTH, stdin);
    new_student->last_name[strcspn(new_student->last_name, "\n")] = '\0';  // Remove newline

    printf("Enter Date of Birth (YYYY-MM-DD): ");
    fgets(new_student->date_of_birth, MAX_DATE_LENGTH, stdin);
    new_student->date_of_birth[strcspn(new_student->date_of_birth, "\n")] = '\0';  // Remove newline

    printf("Enter Enrollment Date (YYYY-MM-DD): ");
    fgets(new_student->enrollment_date, MAX_DATE_LENGTH, stdin);
    new_student->enrollment_date[strcspn(new_student->enrollment_date, "\n")] = '\0';  // Remove newline

    // Initialize next and prev pointers
    new_student->next = NULL;
    new_student->prev = NULL;

    // Add the new student to the doubly linked list
    if (student_head == NULL) {
        student_head = new_student;
        student_tail = new_student;
    } else {
        student_tail->next = new_student;
        new_student->prev = student_tail;
        student_tail = new_student;
    }

    // Append the new student to the file
    const char *file_path = "C:\\Projects\\DBMS_project\\students.dat";
    FILE *file = fopen(file_path, "ab");  // Open in append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Create a StudentFile struct to write to the file
    StudentFile temp;
    temp.student_id = new_student->student_id;
    strcpy(temp.first_name, new_student->first_name);
    strcpy(temp.last_name, new_student->last_name);
    strcpy(temp.date_of_birth, new_student->date_of_birth);
    strcpy(temp.enrollment_date, new_student->enrollment_date);

    // Write the student to the file
    fwrite(&temp, sizeof(StudentFile), 1, file);
    fclose(file);

    printf("Student added successfully!\n");
}

// Function to display all students
// Function to display all classes
void display_classes() {
    if (class_head == NULL) {
        printf("No classes found.\n");
        return;
    }

    printf("\nClasses:\n");
    Class *current = class_head;
    while (current != NULL) {
        printf("Class ID: %d, Teacher ID: %d, Class Name: %s, Academic Year: %s, Scheduled Days: %s\n",
               current->class_id,
               current->teacher_id,  
               current->class_name,
               current->academic_year,
               current->scheduled_days);
        current = current->next;
    }
}

// Function to display a student by ID
void display_student_by_id(int student_id) {
    Student *current = student_head;
    int found = 0;

    while (current != NULL) {
        if (current->student_id == student_id) {
            printf("Student ID: %d, First Name: %s, Last Name: %s, Date of Birth: %s, Enrollment Date: %s\n",
                   current->student_id,
                   current->first_name,
                   current->last_name,
                   current->date_of_birth,
                   current->enrollment_date);
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Student with ID %d not found.\n", student_id);
    }
}

// Function to delete a student by ID
void delete_student(int student_id) {
    if (student_head == NULL) {
        printf("No students found. Cannot delete.\n");
        return;
    }

    Student *current = student_head;
    int found = 0;

    // Search for the student in the linked list
    while (current != NULL) {
        if (current->student_id == student_id) {
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Student with ID %d not found.\n", student_id);
        return;
    }

    // Delete the student from the linked list
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        student_head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        student_tail = current->prev;
    }

    // Free the memory allocated for the deleted student
    free(current);
    printf("Student with ID %d deleted successfully from the linked list.\n", student_id);

    // Now, update the file to remove the deleted student
    const char *file_path = "C:\\Projects\\DBMS_project\\students.dat";
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate the number of students in the file
    int num_students = file_size / sizeof(StudentFile);

    // Read all students from the file into a buffer
    StudentFile *buffer = (StudentFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    fread(buffer, sizeof(StudentFile), num_students, file);
    fclose(file);

    // Open the file in write mode to overwrite it
    file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        free(buffer);
        return;
    }

    // Write all students except the deleted one back to the file
    int students_written = 0;
    for (int i = 0; i < num_students; i++) {
        if (buffer[i].student_id != student_id) {
            fwrite(&buffer[i], sizeof(StudentFile), 1, file);
            students_written++;
        }
    }

    fclose(file);
    free(buffer);

    if (students_written == num_students) {
        printf("Student with ID %d not found in the file.\n", student_id);
    } else {
        printf("Student with ID %d deleted successfully from the file.\n", student_id);
    }
}

// Function to load students from the file into the linked list
void load_students_from_file() {
    const char *file_path = "students.dat";
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("No existing student data found.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    StudentFile *buffer = (StudentFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    // Read the file into the buffer
    fread(buffer, 1, file_size, file);
    fclose(file);

    // Calculate the number of students in the file
    int num_students = file_size / sizeof(StudentFile);

    // Add each student from the buffer to the doubly linked list
    for (int i = 0; i < num_students; i++) {
        Student *new_student = (Student *)malloc(sizeof(Student));
        if (new_student == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }

        // Copy data from the buffer to the new student
        new_student->student_id = buffer[i].student_id;
        strcpy(new_student->first_name, buffer[i].first_name);
        strcpy(new_student->last_name, buffer[i].last_name);
        strcpy(new_student->date_of_birth, buffer[i].date_of_birth);
        strcpy(new_student->enrollment_date, buffer[i].enrollment_date);
        new_student->next = NULL;
        new_student->prev = NULL;

        // Add the new student to the doubly linked list
        if (student_head == NULL) {
            student_head = new_student;
            student_tail = new_student;
        } else {
            student_tail->next = new_student;
            new_student->prev = student_tail;
            student_tail = new_student;
        }
    }

    free(buffer);  // Free the buffer
    printf("Students loaded from file.\n");
}

// Function to save students to the file
void save_students_to_file() {
    const char *file_path = "students.dat";
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Student *current = student_head;
    while (current != NULL) {
        // Save only the data fields (excluding next and prev pointers)
        StudentFile temp;
        temp.student_id = current->student_id;
        strcpy(temp.first_name, current->first_name);
        strcpy(temp.last_name, current->last_name);
        strcpy(temp.date_of_birth, current->date_of_birth);
        strcpy(temp.enrollment_date, current->enrollment_date);

        fwrite(&temp, sizeof(StudentFile), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Students saved to file.\n");
}
// Function to search for students by year of birth
void search_students_by_year_of_birth(int year) {
    if (student_head == NULL) {
        printf("No students found.\n");
        return;
    }

    Student *current = student_head;
    int found = 0;

    printf("\nStudents born in the year %d:\n", year);
    while (current != NULL) {
        // Extract the year from the date_of_birth string (format: YYYY-MM-DD)
        int birth_year;
        sscanf(current->date_of_birth, "%d", &birth_year); // Extracts the first 4 digits (year)

        if (birth_year == year) {
            printf("Student ID: %d, First Name: %s, Last Name: %s, Date of Birth: %s, Enrollment Date: %s\n",
                   current->student_id,
                   current->first_name,
                   current->last_name,
                   current->date_of_birth,
                   current->enrollment_date);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No students found born in the year %d.\n", year);
    }
}

// Function to display all students
void display_students() {
    if (student_head == NULL) {
        printf("No students found.\n");
        return;
    }

    printf("\nStudents:\n");
    Student *current = student_head;
    while (current != NULL) {
        printf("Student ID: %d, First Name: %s, Last Name: %s, Date of Birth: %s, Enrollment Date: %s\n",
               current->student_id,
               current->first_name,
               current->last_name,
               current->date_of_birth,
               current->enrollment_date);
        current = current->next;
    }
}
// Function to display classes where no teacher is assigned (teacher_id = -1)
void display_classes_without_teacher() {
    if (class_head == NULL) {
        printf("No classes found.\n");
        return;
    }

    printf("\nClasses without a teacher assigned:\n");
    Class *current = class_head;
    int found = 0;

    while (current != NULL) {
        if (current->teacher_id == -1) {
            printf("Class ID: %d, Teacher ID: %d, Class Name: %s, Academic Year: %s, Scheduled Days: %s\n",
                   current->class_id,
                   current->teacher_id,
                   current->class_name,
                   current->academic_year,
                   current->scheduled_days);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No classes found without a teacher assigned.\n");
    }
}

void load_class_students_from_file() {
    const char *file_path = "Class_Student.dat";
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("No existing class-student data found.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    Class_StudentsFile *buffer = (Class_StudentsFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    // Read the file into the buffer
    fread(buffer, 1, file_size, file);
    fclose(file);

    // Calculate the number of class-student entries in the file
    int num_entries = file_size / sizeof(Class_StudentsFile);

    // Add each entry from the buffer to the doubly linked list
    for (int i = 0; i < num_entries; i++) {
        Class_Students *new_entry = (Class_Students *)malloc(sizeof(Class_Students));
        if (new_entry == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }

        // Copy data from the buffer to the new entry
        new_entry->Class_Id = buffer[i].Class_Id;
        new_entry->Student_Id = buffer[i].Student_Id;
        new_entry->next = NULL;
        new_entry->prev = NULL;

        // Add the new entry to the doubly linked list
        if (class_Stu_head == NULL) {
            class_Stu_head = new_entry;
            class_Stu_tail = new_entry;
        } else {
            class_Stu_tail->next = new_entry;
            new_entry->prev = class_Stu_tail;
            class_Stu_tail = new_entry;
        }
    }

    free(buffer);  // Free the buffer
    printf("Class-Student data loaded from file.\n");
}
void save_class_students_to_file() {
    const char *file_path = "Class_Student.dat";
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Class_Students *current = class_Stu_head;
    while (current != NULL) {
        Class_StudentsFile temp;
        temp.Class_Id = current->Class_Id;
        temp.Student_Id = current->Student_Id;

        fwrite(&temp, sizeof(Class_StudentsFile), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Class-Student data saved to file.\n");
}
//A function to check if a class_id exists in the class table
int class_exists(int class_id) {
    Class *current = class_head;
    while (current != NULL) {
        if (current->class_id == class_id) {
            return 1; // Class exists
        }
        current = current->next;
    }
    return 0; // Class does not exist
}
//A function to check if a student exists
int student_exists(int student_id) {
    Student *current = student_head;
    while (current != NULL) {
        if (current->student_id == student_id) {
            return 1; // Student exists
        }
        current = current->next;
    }
    return 0; // Student does not exist
}
//A function to check if the composite key the combinations of class_id and student_id already exist in the class_student table
int class_student_exists(int class_id, int student_id) {
    Class_Students *current = class_Stu_head;
    while (current != NULL) {
        if (current->Class_Id == class_id && current->Student_Id == student_id) {
            return 1; // Combination exists
        }
        current = current->next;
    }
    return 0; // Combination does not exist
}
//Insert class id with the student id in the class_Student file
void insert_class_student() {
    int class_id, student_id;

    // Input class ID
    printf("Enter Class ID: ");
    scanf("%d", &class_id);

    // Check if the class exists
    if (!class_exists(class_id)) {
        printf("Error: Class ID %d does not exist in the Class table!\n", class_id);
        return;
    }

    // Input student ID
    printf("Enter Student ID: ");
    scanf("%d", &student_id);

    // Check if the student exists
    if (!student_exists(student_id)) {
        printf("Error: Student ID %d does not exist in the Student table!\n", student_id);
        return;
    }

    // Check if the combination already exists
    if (class_student_exists(class_id, student_id)) {
        printf("Error: Class ID %d and Student ID %d already exist together in the Class-Student table!\n", class_id, student_id);
        return;
    }

    // Create a new Class_Students entry
    Class_Students *new_entry = (Class_Students *)malloc(sizeof(Class_Students));
    if (new_entry == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    new_entry->Class_Id = class_id;
    new_entry->Student_Id = student_id;
    new_entry->next = NULL;
    new_entry->prev = NULL;

    // Add the new entry to the doubly linked list
    if (class_Stu_head == NULL) {
        class_Stu_head = new_entry;
        class_Stu_tail = new_entry;
    } else {
        class_Stu_tail->next = new_entry;
        new_entry->prev = class_Stu_tail;
        class_Stu_tail = new_entry;
    }

    // Append the new entry to the file
    const char *file_path = "C:\\Projects\\DBMS_project\\Class_Student.dat";
    FILE *file = fopen(file_path, "ab");  // Open in append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Class_StudentsFile temp;
    temp.Class_Id = new_entry->Class_Id;
    temp.Student_Id = new_entry->Student_Id;

    fwrite(&temp, sizeof(Class_StudentsFile), 1, file);
    fclose(file);

    printf("Class-Student entry added successfully!\n");
}
void display_classes_for_student(int student_id) {
    // Check if the student exists
    if (!student_exists(student_id)) {
        printf("Error: Student with ID %d does not exist in the Student table!\n", student_id);
        return;
    }

    // Retrieve the student's details
    Student *student = student_head;
    while (student != NULL) {
        if (student->student_id == student_id) {
            break;
        }
        student = student->next;
    }

    if (student == NULL) {
        printf("Error: Student with ID %d not found.\n", student_id);
        return;
    }

    // Display the student's details
    printf("\nStudent Details:\n");
    printf("Student ID: %d, First Name: %s, Last Name: %s, Date of Birth: %s, Enrollment Date: %s\n",
           student->student_id,
           student->first_name,
           student->last_name,
           student->date_of_birth,
           student->enrollment_date);

    // Retrieve and display all classes for the student
    printf("\nClasses assigned to Student ID %d:\n", student_id);

    Class_Students *current = class_Stu_head;
    int found = 0;

    while (current != NULL) {
        if (current->Student_Id == student_id) {
            // Retrieve the class details for the current class_id
            Class *class_node = class_head;
            while (class_node != NULL) {
                if (class_node->class_id == current->Class_Id) {
                    printf("Class ID: %d, Teacher ID: %d, Class Name: %s, Academic Year: %s, Scheduled Days: %s\n",
                           class_node->class_id,
                           class_node->teacher_id,
                           class_node->class_name,
                           class_node->academic_year,
                           class_node->scheduled_days);
                    found = 1;
                    break;
                }
                class_node = class_node->next;
            }
        }
        current = current->next;
    }

    if (!found) {
        printf("No classes found for Student ID %d.\n", student_id);
    }
}
void display_all_classes_with_students() {
    if (class_head == NULL) {
        printf("No classes found.\n");
        return;
    }

    Class *current_class = class_head;
    while (current_class != NULL) {
        printf("\nClass ID: %d, Class Name: %s, Academic Year: %s, Scheduled Days: %s\n",
               current_class->class_id,
               current_class->class_name,
               current_class->academic_year,
               current_class->scheduled_days);

        printf("Students enrolled in this class:\n");

        Class_Students *current_class_student = class_Stu_head;
        int found = 0;

        while (current_class_student != NULL) {
            if (current_class_student->Class_Id == current_class->class_id) {
                // Find the student details
                Student *current_student = student_head;
                while (current_student != NULL) {
                    if (current_student->student_id == current_class_student->Student_Id) {
                        printf("  Student ID: %d, First Name: %s, Last Name: %s, Date of Birth: %s, Enrollment Date: %s\n",
                               current_student->student_id,
                               current_student->first_name,
                               current_student->last_name,
                               current_student->date_of_birth,
                               current_student->enrollment_date);
                        found = 1;
                        break;
                    }
                    current_student = current_student->next;
                }
            }
            current_class_student = current_class_student->next;
        }

        if (!found) {
            printf("  No students enrolled in this class.\n");
        }

        current_class = current_class->next;
    }
}
void update_student_class_assignment(int student_id, int old_class_id, int new_class_id) {
    // Check if the student exists
    if (!student_exists(student_id)) {
        printf("Error: Student with ID %d does not exist in the Student table!\n", student_id);
        return;
    }

    // Check if the old class exists
    if (!class_exists(old_class_id)) {
        printf("Error: Old Class ID %d does not exist in the Class table!\n", old_class_id);
        return;
    }

    // Check if the new class exists
    if (!class_exists(new_class_id)) {
        printf("Error: New Class ID %d does not exist in the Class table!\n", new_class_id);
        return;
    }

    // Check if the old assignment exists
    if (!class_student_exists(old_class_id, student_id)) {
        printf("Error: Student ID %d is not enrolled in Class ID %d.\n", student_id, old_class_id);
        return;
    }

    // Check if the new assignment already exists
    if (class_student_exists(new_class_id, student_id)) {
        printf("Error: Student ID %d is already enrolled in Class ID %d.\n", student_id, new_class_id);
        return;
    }

    // Update the assignment
    Class_Students *current = class_Stu_head;
    while (current != NULL) {
        if (current->Class_Id == old_class_id && current->Student_Id == student_id) {
            current->Class_Id = new_class_id;
            printf("Student ID %d has been moved from Class ID %d to Class ID %d.\n",
                   student_id, old_class_id, new_class_id);
            save_class_students_to_file();  // Save changes to the file
            return;
        }
        current = current->next;
    }

    printf("Error: Assignment not found.\n");
}
void delete_student_class_assignment(int student_id, int class_id) {
    // Check if the student exists
    if (!student_exists(student_id)) {
        printf("Error: Student with ID %d does not exist in the Student table!\n", student_id);
        return;
    }

    // Check if the class exists
    if (!class_exists(class_id)) {
        printf("Error: Class ID %d does not exist in the Class table!\n", class_id);
        return;
    }

    // Check if the assignment exists
    if (!class_student_exists(class_id, student_id)) {
        printf("Error: Student ID %d is not enrolled in Class ID %d.\n", student_id, class_id);
        return;
    }

    // Delete the assignment
    Class_Students *current = class_Stu_head;
    Class_Students *prev = NULL;

    while (current != NULL) {
        if (current->Class_Id == class_id && current->Student_Id == student_id) {
            if (prev == NULL) {
                // If the node to be deleted is the head
                class_Stu_head = current->next;
            } else {
                prev->next = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = prev;
            }

            free(current);
            printf("Student ID %d has been removed from Class ID %d.\n", student_id, class_id);
            save_class_students_to_file();  // Save changes to the file
            return;
        }

        prev = current;
        current = current->next;
    }

    printf("Error: Assignment not found.\n");
}
