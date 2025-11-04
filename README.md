# Student Monitoring Database Management System (SM-DBMS)

## 1. Database and DBMS Design

####  Scenario
The **Student Monitoring Database Management System (SM-DBMS)** simplifies the process of managing student attendance and academic performance.  

Teachers can log into the system, view their assigned classes, and record attendance for each class session. They can also input grades for assessments such as quizzes, assignments, and exams, along with remarks and scores.  

Students can log in to monitor their academic progress, view attendance history, and track grades for all assessments they have taken.  

The system:
- Ensures smooth data handling between teachers, students, and classes.
- Provides a user-friendly interface for teachers to track progress.
- Offers students transparent access to their academic data for accountability.

---

## 📊 Database Schema

The system’s database is composed of **seven interrelated tables** that manage classes, students, teachers, attendance, grades, and their relationships.

| # | Table Name | Description |
|---|-------------|-------------|
| **1** | **Class** | Stores essential information about each class. |
| **2** | **Teacher** | Stores teacher details and their subject specializations. |
| **3** | **Student** | Stores student personal and enrollment data. |
| **4** | **Attendance** | Records attendance status for students per class and date. |
| **5** | **Grades** | Records students’ grades for various assessments. |
| **6** | **Teacher_PhoneNumber** | Stores multiple phone numbers for each teacher. |
| **7** | **Class_Student** | Tracks the many-to-many relationship between students and classes. |

---

### 🧩 Table Details

#### **1. Class**
| Field | Type | Key | Constraints | Description |
|--------|------|-----|--------------|--------------|
| Class_ID | INT | PK | NOT NULL | Uniquely identifies each class. |
| Teacher_ID | INT | FK | NOT NULL | Links to `Teacher.Teacher_ID`, ensuring every class is assigned to a teacher. |
| Class_Name | VARCHAR(50) |  | NOT NULL | Name of the class. |
| Academic_Year | CHAR(4) |  | NOT NULL | The academic year (e.g., “2025”). |
| Scheduled_Days | VARCHAR(30) |  | NOT NULL | Days the class is held. |

---

#### **2. Teacher**
| Field | Type | Key | Constraints | Description |
|--------|------|-----|--------------|--------------|
| Teacher_ID | INT | PK | NOT NULL | Uniquely identifies each teacher. |
| First_Name | VARCHAR(20) |  | NOT NULL | Teacher’s first name. |
| Last_Name | VARCHAR(20) |  | NOT NULL | Teacher’s last name. |
| Subject_Expertise | VARCHAR(50) |  | NOT NULL | Subject area taught by the teacher. |

---

#### **3. Student**
| Field | Type | Key | Constraints | Description |
|--------|------|-----|--------------|--------------|
| Student_ID | INT | PK | NOT NULL | Uniquely identifies each student. |
| First_Name | VARCHAR(20) |  | NOT NULL | Student’s first name. |
| Last_Name | VARCHAR(20) |  | NOT NULL | Student’s last name. |
| Date_of_Birth | DATE |  | NOT NULL | Student’s birth date. |
| Enrollment_Date | DATE |  | NOT NULL | Date the student enrolled. |

---

#### **4. Attendance**
| Field | Type | Key | Constraints | Description |
|--------|------|-----|--------------|--------------|
| Attendance_ID | INT | PK | NOT NULL | Unique ID for each attendance record. |
| Class_ID | INT | FK | NOT NULL | Links to `Class.Class_ID`, tracking class-specific attendance. |
| Student_ID | INT | FK | NOT NULL | Links to `Student.Student_ID`, ensuring attendance is tied to a student. |
| Date | DATE |  | NOT NULL | Date of attendance. |
| Status | VARCHAR(10) |  | NOT NULL | Attendance status (“Present” / “Absent”). |

---

#### **5. Grades**
| Field | Type | Key | Constraints | Description |
|--------|------|-----|--------------|--------------|
| Grades_ID | INT | PK | NOT NULL | Unique ID for each grade record. |
| Class_ID | INT | FK | NOT NULL | Links to `Class.Class_ID`. |
| Student_ID | INT | FK | NOT NULL | Links to `Student.Student_ID`. |
| Assessment_Type | VARCHAR(30) |  | NOT NULL | Type of assessment (Exam, Quiz, Project, etc.). |
| Score | DECIMAL(5,2) |  | NOT NULL | Numeric score achieved. |
| Remarks | TEXT |  | NULL | Optional teacher remarks or feedback. |

---

#### **6. Teacher_PhoneNumber**
| Field | Type | Key | Constraints | Description |
|--------|------|-----|--------------|--------------|
| Teacher_ID | INT | PK (Part 1), FK | NOT NULL | Links to `Teacher.Teacher_ID`. |
| Phone_Number | VARCHAR(15) | PK (Part 2) | NOT NULL | Unique phone number for the teacher. |

---

#### **7. Class_Student**
| Field | Type | Key | Constraints | Description |
|--------|------|-----|--------------|--------------|
| Class_ID | INT | PK (Part 1), FK | NOT NULL | Links to `Class.Class_ID`. |
| Student_ID | INT | PK (Part 2), FK | NOT NULL | Links to `Student.Student_ID`. |

---

### 🔗 Relationships Overview
- **Teacher → Class:** One-to-Many  
- **Class → Student:** Many-to-Many (via `Class_Student`)  
- **Student → Attendance:** One-to-Many  
- **Student → Grades:** One-to-Many  
- **Teacher → Teacher_PhoneNumber:** One-to-Many  

---

`

#### Relationships
- A teacher can teach multiple classes, but each class is taught by one teacher.  
- A class can have many students, and each student can belong to multiple classes.  
- A student can have multiple attendance and grade records associated with their classes.  

The database enforces these relationships through primary and foreign keys, ensuring data integrity and consistency.
Note: You can find the entire conceptual, logical and physical design in the shared Word Document.
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


---
