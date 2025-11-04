# 🎓 Student Monitoring Database Management System (SM-DBMS)

## 1. Database and DBMS Design

### 1.1 Database Requirements

#### 1.1.1 Scenario
The **Student Monitoring Database Management System (SM-DBMS)** simplifies the process of managing student attendance and academic performance.  

Teachers can log into the system, view their assigned classes, and record attendance for each class session. They can also input grades for assessments such as quizzes, assignments, and exams, along with remarks and scores.  

Students can log in to monitor their academic progress, view attendance history, and track grades for all assessments they have taken.  

The system:
- Ensures smooth data handling between teachers, students, and classes.
- Provides a user-friendly interface for teachers to track progress.
- Offers students transparent access to their academic data for accountability.

---

### 1.1.2 Data Requirements

#### Entities and Attributes
**Teacher**  
`teacher_id (PK), first_name, last_name, phone_number, subject_expertise`

**Student**  
`student_id (PK), first_name, last_name, date_of_birth, phone_number, enrollment_date`

**Class**  
`class_id (PK), class_name, academic_year, scheduled_days`

**Grades**  
`grade_id (PK), assessment_type, score, remarks`

**Attendance**  
`attendance_id (PK), date, status`

#### Relationships
- A teacher can teach multiple classes, but each class is taught by one teacher.  
- A class can have many students, and each student can belong to multiple classes.  
- A student can have multiple attendance and grade records associated with their classes.  

The database enforces these relationships through primary and foreign keys, ensuring data integrity and consistency.

---

### 1.1.3 User and System Requirements

#### 👩‍💼 User Requirements

**Admin**
- **Insert:** Add teachers, students, classes, grades, attendance records, and phone numbers.  
- **Update:** Modify phone numbers, class details, and student-class links.  
- **Delete:** Remove teachers, students, classes, phone numbers, attendance, and grades by ID.  
- **Select:** View and search all records.  
- **Save:** Save changes to `.dat` files for persistence.  

**Teacher**
- **Insert:** Add students to assigned classes and record attendance or grades.  
- **Update:** Update attendance status or grade information.  
- **Select:** View students, attendance, and grade records for their classes.  
- **Delete:** Remove students from a class when needed.  

**Student**
- **Select:**  
  - View enrolled class information and schedules.  
  - View attendance history.  
  - View grades and remarks for all assessments.  

---

### ⚙️ System Requirements

#### Functional Requirements
- **Data Storage and Management:**  
  Uses binary `.dat` files to store and manage teachers, students, classes, attendance, and grades.

- **Transaction Management:**  
  - **Atomicity:** Operations are fully completed or rolled back on failure.  
  - **Consistency:** Data remains valid after every transaction.  
  - **Isolation:** Multiple users can access data without interference.  
  - **Durability:** Committed data is saved permanently.

- **User Management and Security:**  
  Role-based access ensures users can only perform authorized actions.

- **Constraints Enforcement:**  
  Primary and foreign keys maintain valid relationships among entities.

#### Non-Functional Requirements
- **Performance:** Efficient file-handling and indexing for fast operations.  
- **Data Security:** Controlled access based on user roles.  
- **Usability:** Simple, menu-driven interface for ease of use.  

---

## 2. Project Structure Overview

The system is developed in **C** and uses `.dat` files for persistent storage.  
Each module (`.c` and `.h` files) is responsible for specific functionalities.  

### 📂 Folder and File Structure

| File/Folder | Type | Description |
|--------------|------|-------------|
| `.vscode/` | Folder | Visual Studio Code configuration files |
| `main.c` | C Source | Entry point of the program; contains the main menu and logic for user roles |
| `attendance_grades.c` / `attendance_grades.h` | C Source & Header | Manage attendance and grades (insert, update, delete, search) |
| `classes_students.c` / `classes_students.h` | C Source & Header | Manage student-class relationships and class data |
| `teacher.c` / `teacher.h` | C Source & Header | Manage teacher data and phone numbers |
| `students.dat` | Data File | Stores student information |
| `teachers.dat` | Data File | Stores teacher information |
| `classes.dat` | Data File | Stores class details |
| `grades.dat` | Data File | Stores grades and remarks |
| `attendance.dat` | Data File | Stores attendance records |
| `Class_Student.dat` | Data File | Links students to their classes |
| `teacher_phonenumbers.dat` | Data File | Stores each teacher’s phone numbers |

Each `.dat` file acts as a **database table**, storing binary data that represents records for each entity.

---

## 3. Technologies Used

- **Programming Language:** C  
- **Data Storage:** Binary File Handling (`.dat` files)  
- **Programming Paradigm:** Modular Programming  
- **Development Environment:** Visual Studio Code  

---

## 4. Features

- Role-based access for Admin, Teacher, and Student  
- Add, update, delete, and search data records  
- Track attendance and grades per class  
- Persistent data storage using `.dat` files  
- Secure, organized, and user-friendly command-line interface  

---

## 5. Author

**Project Title:** Student Monitoring Database Management System (SM-DBMS)  
**Developer:** Shahad Feras  
**Institution:** Hashemite University  
**Course:** Database Programming  
**Year:** 2025  

---
