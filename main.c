#include <stdio.h>
#include "teacher.h"
#include "classes_students.h"
#include "attendance_grades.h"
#include <stdlib.h>
#include <string.h>

#define MAX_DATE_LENGTH 11
#define MAX_STATUS_LENGTH 20

int main() {
    // Load data from files
    load_teachers_from_file();
    load_phone_numbers_from_file();
    load_classes_from_file();
    load_students_from_file();
    load_class_students_from_file();
    load_attendance_from_file();
    load_grades_from_file();

    int main_choice;
    do {
        printf("\nMAIN MENU\n");
        printf("1. Teacher Management\n");
        printf("2. Class Management\n");
        printf("3. Student Management\n");
        printf("4. Attendance Management\n");
        printf("5. Grade Management\n");
        printf("6. Exit and Save\n");
        printf("Enter your choice: ");
        scanf("%d", &main_choice);
        while(getchar() != '\n'); // Clear input buffer

        switch(main_choice) {
            case 1: { // Teacher Management
                int teacher_choice;
                do {
                    printf("\nTEACHER MANAGEMENT\n");
                    printf("1. Add Teacher\n");
                    printf("2. Display Teachers\n");
                    printf("3. Delete Teacher\n");
                    printf("4. Search Teacher by ID\n");
                    printf("5. Add Phone Number\n");
                    printf("6. Display Phone Numbers\n");
                    printf("7. Delete Phone Number\n");
                    printf("8. Update Phone Number\n");
                    printf("9. Search by Subject\n");
                    printf("10. Return to Main Menu\n");
                    printf("Enter choice: ");
                    scanf("%d", &teacher_choice);
                    while(getchar() != '\n');

                    switch(teacher_choice) {
                        case 1: insert_teacher(); break;
                        case 2: display(); break;
                        case 3: {
                            printf("Enter Teacher ID to delete: ");
                            int id; scanf("%d", &id);
                            delete_teacher(id);
                            break;
                        }
                        case 4: {
                            printf("Enter Teacher ID: ");
                            int id; scanf("%d", &id);
                            display_teacher_by_id(id);
                            break;
                        }
                        case 5: insert_phone_number(); break;
                        case 6: display_phone_numbers(); break;
                        case 7: {
                            int del_choice;
                            printf("\nDelete Phone Number By:\n");
                            printf("1. Teacher ID\n");
                            printf("2. Phone Number\n");
                            printf("3. Teacher ID and Phone Number\n");
                            printf("Enter your choice: ");
                            scanf("%d", &del_choice);

                            if (del_choice == 1) {
                                int teacher_id;
                                printf("Enter Teacher ID: ");
                                scanf("%d", &teacher_id);
                                delete_phone_numbers_by_teacher_id(teacher_id);
                            } else if (del_choice == 2) {
                                char phone_number[MAX_PHONE_LENGTH];
                                printf("Enter Phone Number: ");
                                scanf("%s", phone_number);
                                delete_phone_number_by_phone_number(phone_number);
                            } else if (del_choice == 3) {
                                int teacher_id;
                                char phone_number[MAX_PHONE_LENGTH];
                                printf("Enter Teacher ID: ");
                                scanf("%d", &teacher_id);
                                printf("Enter Phone Number: ");
                                scanf("%s", phone_number);
                                delete_phone_number_by_teacher_id_and_phone_number(teacher_id, phone_number);
                            } else {
                                printf("Invalid choice!\n");
                            }
                            break;
                        }
                        case 8: {
                            int update_choice;
                            printf("\nUpdate Phone Number By:\n");
                            printf("1. Phone Number (Unique Key)\n");
                            printf("2. Teacher ID and Phone Number (Composite Key)\n");
                            printf("Enter your choice: ");
                            scanf("%d", &update_choice);

                            if (update_choice == 1) {
                                update_phone_number_by_phone_number();
                            } else if (update_choice == 2) {
                                update_phone_number_by_teacher_id_and_phone_number();
                            } else {
                                printf("Invalid choice!\n");
                            }
                            break;
                        }
                        case 9: {
                            char subject[MAX_SUBJECT_LENGTH];
                            printf("Enter Subject Expertise: ");
                            scanf("%s", subject);
                            select_teachers_by_subject(subject);
                            break;
                        }
                        case 10: break;
                        default: printf("Invalid choice!\n");
                    }
                } while(teacher_choice != 10);
                break;
            }

            case 2: { // Class Management
                int class_choice;
                do {
                    printf("\nCLASS MANAGEMENT\n");
                    printf("1. Add Class\n");
                    printf("2. Display Classes\n");
                    printf("3. Update Class Teacher\n");
                    printf("4. Delete Class\n");
                    printf("5. Update Class Name\n");
                    printf("6. Update Scheduled Days\n");
                    printf("7. Show Classes Without Teacher\n");
                    printf("8. Return to Main Menu\n");
                    printf("Enter choice: ");
                    scanf("%d", &class_choice);
                    while(getchar() != '\n');

                    switch(class_choice) {
                        case 1: insert_class(); break;
                        case 2: display_classes(); break;
                        case 3: {
                            int class_id, teacher_id;
                            printf("Enter Class ID: ");
                            scanf("%d", &class_id);
                            printf("Enter Teacher ID: ");
                            scanf("%d", &teacher_id);
                            update_teacher_id_for_class(class_id, teacher_id);
                            break;
                        }
                        case 4: {
                            int class_id;
                            printf("Enter Class ID to delete: ");
                            scanf("%d", &class_id);
                            delete_class(class_id);
                            break;
                        }
                        case 5: {
                            int class_id;
                            char new_class_name[MAX_CLASS_NAME_LENGTH];
                            printf("Enter Class ID: ");
                            scanf("%d", &class_id);
                            printf("Enter New Class Name: ");
                            scanf("%s", new_class_name);
                            update_class_name(class_id, new_class_name);
                            break;
                        }
                        case 6: {
                            char class_name[MAX_CLASS_NAME_LENGTH];
                            char new_scheduled_days[MAX_SCHEDULED_DAYS_LENGTH];
                            printf("Enter Class Name: ");
                            scanf("%s", class_name);
                            printf("Enter New Scheduled Days: ");
                            scanf(" %[^\n]", new_scheduled_days);
                            update_scheduled_days(class_name, new_scheduled_days);
                            break;
                        }
                        case 7: display_classes_without_teacher(); break;
                        case 8: break;
                        default: printf("Invalid choice!\n");
                    }
                } while(class_choice != 8);
                break;
            }

            case 3: { // Student Management
                int student_choice;
                do {
                    printf("\nSTUDENT MANAGEMENT\n");
                    printf("1. Add Student\n");
                    printf("2. Display Students\n");
                    printf("3. Search Student by ID\n");
                    printf("4. Delete Student\n");
                    printf("5. Search by Birth Year\n");
                    printf("6. Manage Class Enrollment\n");
                    printf("7. Show Student Classes\n");
                    printf("8. Show All Class Rosters\n");
                    printf("9. Return to Main Menu\n");
                    printf("Enter choice: ");
                    scanf("%d", &student_choice);
                    while(getchar() != '\n');

                    switch(student_choice) {
                        case 1: insert_student(); break;
                        case 2: display_students(); break;
                        case 3: {
                            int student_id;
                            printf("Enter Student ID: ");
                            scanf("%d", &student_id);
                            display_student_by_id(student_id);
                            break;
                        }
                        case 4: {
                            int student_id;
                            printf("Enter Student ID to delete: ");
                            scanf("%d", &student_id);
                            delete_student(student_id);
                            break;
                        }
                        case 5: {
                            int year;
                            printf("Enter Year of Birth (YYYY): ");
                            scanf("%d", &year);
                            search_students_by_year_of_birth(year);
                            break;
                        }
                        case 6: {
                            int sub_choice;
                            printf("\n1. Enroll Student\n2. Update Enrollment\n3. Remove Enrollment\nChoice: ");
                            scanf("%d", &sub_choice);

                            if (sub_choice == 1) {
                                insert_class_student();
                            } else if (sub_choice == 2) {
                                int student_id, old_class_id, new_class_id;
                                printf("Enter Student ID: ");
                                scanf("%d", &student_id);
                                printf("Enter Old Class ID: ");
                                scanf("%d", &old_class_id);
                                printf("Enter New Class ID: ");
                                scanf("%d", &new_class_id);
                                update_student_class_assignment(student_id, old_class_id, new_class_id);
                            } else if (sub_choice == 3) {
                                int student_id, class_id;
                                printf("Enter Student ID: ");
                                scanf("%d", &student_id);
                                printf("Enter Class ID: ");
                                scanf("%d", &class_id);
                                delete_student_class_assignment(student_id, class_id);
                            } else {
                                printf("Invalid choice!\n");
                            }
                            break;
                        }
                        case 7: {
                            int student_id;
                            printf("Enter Student ID: ");
                            scanf("%d", &student_id);
                            display_classes_for_student(student_id);
                            break;
                        }
                        case 8: display_all_classes_with_students(); break;
                        case 9: break;
                        default: printf("Invalid choice!\n");
                    }
                } while(student_choice != 9);
                break;
            }

            case 4: { // Attendance Management
                int attendance_choice;
                do {
                    printf("\nATTENDANCE MANAGEMENT\n");
                    printf("1. Add Attendance\n");
                    printf("2. Display All Attendance\n");
                    printf("3. Display Attendance by Date\n");
                    printf("4. Delete Attendance\n");
                    printf("5. Update Attendance Status\n");
                    printf("6. Return to Main Menu\n");
                    printf("Enter choice: ");
                    scanf("%d", &attendance_choice);
                    while(getchar() != '\n');

                    switch(attendance_choice) {
                        case 1: insert_attendance(); break;
                        case 2: display_all_attendance(); break;
                        case 3: {
                            char date[MAX_DATE_LENGTH];
                            printf("Enter Date (YYYY-MM-DD): ");
                            scanf("%s", date);
                            display_attendance_by_date(date);
                            break;
                        }
                        case 4: {
                            int attendance_id;
                            printf("Enter Attendance ID to delete: ");
                            scanf("%d", &attendance_id);
                            delete_attendance(attendance_id);
                            break;
                        }
                        case 5: {
                            int attendance_id;
                            char new_status[MAX_STATUS_LENGTH];
                            printf("Enter Attendance ID: ");
                            scanf("%d", &attendance_id);
                            printf("Enter New Status (e.g., Present, Absent, Late): ");
                            scanf("%s", new_status);
                            update_attendance_status(attendance_id, new_status);
                            break;
                        }
                        case 6: break;
                        default: printf("Invalid choice!\n");
                    }
                } while(attendance_choice != 6);
                break;
            }

            case 5: { // Grade Management
                int grade_choice;
                do {
                    printf("\nGRADE MANAGEMENT\n");
                    printf("1. Add Grades\n");
                    printf("2. Display All Grades\n");
                    printf("3. Display Grades by Assessment Type\n");
                    printf("4. Delete Grades\n");
                    printf("5. Update Grades Score and Remark\n");
                    printf("6. Update Grades Assessment Type\n");
                    printf("7. Return to Main Menu\n");
                    printf("Enter choice: ");
                    scanf("%d", &grade_choice);
                    while(getchar() != '\n');

                    switch(grade_choice) {
                        case 1: insert_grades(); break;
                        case 2: display_all_grades(); break;
                        case 3: {
                            char assessment_type[MAX_assesTYPE];
                            printf("Enter Assessment Type (e.g., mid-score, final-score): ");
                            scanf("%s", assessment_type);
                            display_grades_by_assessment_type(assessment_type);
                            break;
                        }
                        case 4: {
                            int grades_id;
                            printf("Enter Grades ID to delete: ");
                            scanf("%d", &grades_id);
                            delete_grades(grades_id);
                            break;
                        }
                        case 5: {
                            int grades_id, new_score;
                            char new_remark[MAX_REMARK];
                            printf("Enter Grades ID: ");
                            scanf("%d", &grades_id);
                            printf("Enter New Score: ");
                            scanf("%d", &new_score);
                            printf("Enter New Remark: ");
                            scanf(" %[^\n]", new_remark);
                            update_grades_score_and_remark(grades_id, new_score, new_remark);
                            break;
                        }
                        case 6: {
                            int grades_id;
                            char new_assessment_type[MAX_assesTYPE];
                            printf("Enter Grades ID: ");
                            scanf("%d", &grades_id);
                            printf("Enter New Assessment Type: ");
                            scanf(" %[^\n]", new_assessment_type);
                            update_grades_assessment_type(grades_id, new_assessment_type);
                            break;
                        }
                        case 7: break;
                        default: printf("Invalid choice!\n");
                    }
                } while(grade_choice != 7);
                break;
            }

            case 6:
                printf("Saving data and exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while(main_choice != 6);

    // Save data to files
    save_teachers_to_file();
    save_phone_numbers_to_file();
    save_classes_to_file();
    save_students_to_file();
    save_class_students_to_file();
    save_attendance_to_file();
    save_grades_to_file();

    return 0;
}