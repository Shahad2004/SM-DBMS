🧾 Student Monitoring Database Management System (SM-DBMS)
📘 1. Database and DBMS Design
1.1 Database Requirements
1.1.1 Scenario

Managing student attendance and performance in an academic setting is made easier by the Student Monitoring Database Management System (SM-DBMS).

Teachers can log into the system to view their assigned classes, check the list of enrolled students, and record attendance for each class session. For every assessment conducted (e.g., quiz, assignment, or exam), teachers can enter grades, scores, and remarks.

Students, in turn, can log in to monitor their academic progress — including attendance history and grades — helping them stay on track with their coursework.

The system ensures:

Smooth data handling and linkage between classes, teachers, and students.

A clear, easy-to-use interface for teachers to monitor progress.

Transparency and accountability by providing students access to their academic records.

1.1.2 Data Requirements

The SM-DBMS maintains structured records for all entities involved in academic monitoring.

Entities and Attributes:

Teacher
(teacher_id PK, first_name, last_name, phone_number, subject_expertise)

Student
(student_id PK, first_name, last_name, date_of_birth, phone_number, enrollment_date)

Class
(class_id PK, class_name, academic_year, scheduled_days)

Grades
(grade_id PK, assessment_type, score, remarks)

Attendance
(attendance_id PK, date, status)

Relationships:

Each teacher can teach multiple classes, but each class has one teacher.

Each class can have multiple students, and each student can belong to multiple classes.

Each student can have multiple attendance and grade records associated with their classes.

The database enforces these relationships to ensure integrity and consistency between all components.

1.1.3 User and System Requirements
👩‍💼 User Requirements
Admin:

Responsible for full database management.
Operations:

Insert: Add teachers, students, classes, grades, attendance, and teacher phone numbers.

Update: Modify class details, phone numbers, and student assignments.

Delete: Remove teachers, students, classes, phone numbers, grades, and attendance records by ID or composite keys.

Select: View or search all records in the database.

Save: Commit all modifications to the .dat files for data persistence.

Teacher:

Manages attendance and grades for assigned classes.
Operations:

Insert: Add students to their classes, record attendance, and add grades.

Update: Modify attendance status, assessment types, or grades.

Select: View students, their attendance, and performance records.

Delete: Remove student-class relationships if necessary.

Student:

Can view personal data and academic records.
Operations:

Select:

View enrolled class details and schedules.

Check attendance records.

View grades and remarks for each assessment.

⚙️ System Requirements
Functional Requirements:

Data Storage & Management:
All data (teachers, students, classes, attendance, and grades) is stored in .dat files using binary file handling for persistence.

Transaction Management:

Atomicity: Transactions (insert, update, delete) are fully completed or rolled back.

Consistency: Data remains valid after every operation.

Isolation: Concurrent access does not cause interference.

Durability: Data changes are permanently saved to disk.

User Management & Security:
Role-based access ensures teachers, students, and admins only perform allowed operations.

Constraint Enforcement:
Primary and foreign keys maintain data integrity between entities.

Non-Functional Requirements:

Performance:
Efficient file-handling and in-memory data structures ensure fast retrieval and updates.

Data Security:
Access control prevents unauthorized modifications.

Usability:
Menu-driven interface simplifies user interaction with the system.

📁 Project Structure Overview

This project uses C language for logic and .dat files for persistent data storage.
The structure of files (as shown in your screenshot) is as follows:

File/Folder	Type	Description
.vscode/	Folder	VS Code configuration files.
main.c	C Source File	The entry point of the system; initializes menus and user roles (Admin, Teacher, Student).
attendance_grades.c / .h	C Source & Header	Functions for managing attendance and grades — including insert, update, delete, and search operations.
classes_students.c / .h	C Source & Header	Handles linking students to classes, and manages class-related data.
teacher.c / .h	C Source & Header	Manages teacher data including insertion, updates, and phone numbers.
*.dat Files	Data Files	Store persistent records for each entity (students, teachers, classes, attendance, grades).
🗂 Example Data Files:

students.dat → Student records.

teachers.dat → Teacher details.

classes.dat → Class information.

grades.dat → Grade records per assessment.

attendance.dat → Attendance history.

Class_Student.dat → Relationship between students and classes.

teacher_phonenumbers.dat → Stores each teacher’s contact numbers.

Each .dat file acts as a mini-database table, while .c and .h files define modules that handle the logic for reading, writing, updating, and displaying that data.

🧩 Technologies Used

Language: C

Data Storage: Binary File Handling (.dat files)

Paradigm: Modular Programming

Development Environment: Visual Studio Code

🚀 Features

✅ Role-based access for Admin, Teacher, and Student
✅ Add, update, delete, and search records
✅ Attendance and grades linked automatically to classes
✅ Persistent data storage using .dat files
✅ User-friendly menu navigation
