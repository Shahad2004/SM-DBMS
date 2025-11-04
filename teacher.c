#include "teacher.h"
#include "classes_students.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 

// Global pointers for the doubly linked list
Teacher *head = NULL; // Pointer to the first node
Teacher *tail = NULL; // Pointer to the last node
TeacherPhone *phone_head = NULL;    // Pointer to the first phone node
TeacherPhone *phone_tail = NULL;    // Pointer to the last phone node

#define HASH_TABLE_SIZE 1000  // Size of the hash table
#define PHONE_HASH_TABLE_SIZE 1000
// Define a dummy item for marking deleted slots in the hash table
#define DUMMY_KEY -1          // For Teacher hash table (integer)
#define DUMMY_PHONE_KEY "-1"  // For TeacherPhone hash table (string)
#define DUMMY_DATA NULL       // For both has

// Structure for hash table items
typedef struct DataItem {
    int key;                  // teacher_id
    Teacher *data;            // Pointer to the teacher record
} DataItem;

// Structure for phone hash table items
typedef struct PhoneDataItem {
    char key[MAX_PHONE_LENGTH];  // phone_number (unique key)
    TeacherPhone *data;          // Pointer to the phone record
} PhoneDataItem;

// Global hash table array
DataItem *hashArray[HASH_TABLE_SIZE] = {NULL};  // Initialize all slots to NULL
PhoneDataItem *phoneHashArray[PHONE_HASH_TABLE_SIZE] = {NULL};

// Hash function to map a key to an index
int hashCode(int key) {
    return key % HASH_TABLE_SIZE;
}

// Hash function to map a phone number (string) to an index
int phoneHashCode(const char *key) {
    unsigned long hash = 0;
    int c;
    while ((c = *key++)) {
        hash = hash * 31 + c; // Simple hash function
    }
    return hash % PHONE_HASH_TABLE_SIZE;
}

// Insert a key-data pair into the hash table
void insert(int key, Teacher *data) {
    DataItem *item = (DataItem *)malloc(sizeof(DataItem));
    if (item == NULL) {
        printf("Memory allocation failed for hash table item!\n");
        return;
    }
    item->key = key;
    item->data = data;

    // Get the hash index
    int hashIndex = hashCode(key);

    // Move in the array until an empty or deleted slot is found
    while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != DUMMY_KEY) {
        // Go to the next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= HASH_TABLE_SIZE;
    }

    // Insert the item
    hashArray[hashIndex] = item;
}
// Insert a phone number into the phone hash table
void insertPhone(const char *key, TeacherPhone *data) {
    PhoneDataItem *item = (PhoneDataItem *)malloc(sizeof(PhoneDataItem));
    if (item == NULL) {
        printf("Memory allocation failed for phone hash table item!\n");
        return;
    }
    strcpy(item->key, key);
    item->data = data;

    // Get the hash index
    int hashIndex = phoneHashCode(key);

    // Move in the array until an empty or deleted slot is found
    while (phoneHashArray[hashIndex] != NULL && strcmp(phoneHashArray[hashIndex]->key, DUMMY_PHONE_KEY) != 0) {
        // Go to the next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= PHONE_HASH_TABLE_SIZE;
    }

    // Insert the item
    phoneHashArray[hashIndex] = item;
}
// Search for a record by key in the hash table
Teacher *search(int key) {
    // Get the hash index
    int hashIndex = hashCode(key);

    // Move in the array until an empty slot is found
    while (hashArray[hashIndex] != NULL) {
        if (hashArray[hashIndex]->key == key) {
            // Return the data if the key is found
            return hashArray[hashIndex]->data;
        }
        // Go to the next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= HASH_TABLE_SIZE;
    }

    // Key not found
    return NULL;
}
// Search for a phone number in the phone hash table
TeacherPhone *searchPhone(const char *key) {
    // Get the hash index
    int hashIndex = phoneHashCode(key);

    // Move in the array until an empty slot is found
    while (phoneHashArray[hashIndex] != NULL) {
        if (strcmp(phoneHashArray[hashIndex]->key, key) == 0) {
            // Return the data if the key is found
            return phoneHashArray[hashIndex]->data;
        }
        // Go to the next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= PHONE_HASH_TABLE_SIZE;
    }

    // Key not found
    return NULL;
}

// Delete a record by key from the hash table
void deleteFromHashTable(int key) {
    // Get the hash index
    int hashIndex = hashCode(key);

    // Move in the array until an empty slot is found
    while (hashArray[hashIndex] != NULL) {
        if (hashArray[hashIndex]->key == key) {
            // Free the memory allocated for the item
            free(hashArray[hashIndex]);
            // Assign a dummy item at the deleted position
            hashArray[hashIndex] = (DataItem *)malloc(sizeof(DataItem));
            hashArray[hashIndex]->key = DUMMY_KEY;
            hashArray[hashIndex]->data = DUMMY_DATA;
            return;
        }
        // Go to the next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= HASH_TABLE_SIZE;
    }
}
// Delete a phone number from the phone hash table
void deleteFromPhoneHashTable(const char *key) {
    // Get the hash index
    int hashIndex = phoneHashCode(key);

    // Move in the array until an empty slot is found
    while (phoneHashArray[hashIndex] != NULL) {
        if (strcmp(phoneHashArray[hashIndex]->key, key) == 0) {
            // Free the memory allocated for the item
            free(phoneHashArray[hashIndex]);
            // Assign a dummy item at the deleted position
            phoneHashArray[hashIndex] = (PhoneDataItem *)malloc(sizeof(PhoneDataItem));
            strcpy(phoneHashArray[hashIndex]->key, DUMMY_PHONE_KEY);
            phoneHashArray[hashIndex]->data = DUMMY_DATA;
            return;
        }
        // Go to the next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= PHONE_HASH_TABLE_SIZE;
    }
}
// Function to check if a teacher ID is unique
int is_teacher_id_unique(int teacher_id) {
    return search(teacher_id) == NULL;
}

// Function to select a teacher by teacher_id
Teacher* select_teacher_by_id(int teacher_id) {
    return search(teacher_id);
}

// Function to display a teacher by ID
void display_teacher_by_id(int teacher_id) {
    Teacher *teacher = select_teacher_by_id(teacher_id);

    if (teacher != NULL) {
        // Display the teacher details
        printf("Teacher found:\n");
        printf("ID: %d, Name: %s %s, Subject: %s\n",
               teacher->teacher_id,
               teacher->first_name,
               teacher->last_name,
               teacher->subject_expertise);
    } else {
        printf("Teacher with ID %d not found.\n", teacher_id);
    }
}

// Function to insert a new teacher
void insert_teacher() {
    Teacher *new_teacher = (Teacher *)malloc(sizeof(Teacher));
    if (new_teacher == NULL) {
        printf("Memory allocation failed for new teacher!\n");
        return;
    }

    // Input teacher details
    printf("Enter Teacher ID: ");
    scanf("%d", &new_teacher->teacher_id);

    // Check if the teacher ID is unique
    if (!is_teacher_id_unique(new_teacher->teacher_id)) {
        printf("Error: Teacher ID %d already exists!\n", new_teacher->teacher_id);
        free(new_teacher);
        return;
    }

    printf("Enter First Name: ");
    scanf("%s", new_teacher->first_name);

    printf("Enter Last Name: ");
    scanf("%s", new_teacher->last_name);

    printf("Enter Subject Expertise: ");
    scanf("%s", new_teacher->subject_expertise);

    // Initialize next and prev pointers
    new_teacher->next = NULL;
    new_teacher->prev = NULL;

    // Add the new teacher to the doubly linked list
    if (head == NULL) {
        head = new_teacher;
        tail = new_teacher;
    } else {
        tail->next = new_teacher;
        new_teacher->prev = tail;
        tail = new_teacher;
    }

    // Add the new teacher to the hash table
    insert(new_teacher->teacher_id, new_teacher);

    printf("Teacher added successfully!\n");
}

// Function to display all teachers
void display_teachers() {
    if (head == NULL) {
        printf("No teachers found.\n");
        return;
    }

    printf("\nTeachers:\n");
    Teacher *current = head;
    while (current != NULL) {
        printf("ID: %d, Name: %s %s, Subject: %s\n",
               current->teacher_id,
               current->first_name,
               current->last_name,
               current->subject_expertise);
        current = current->next;
    }
}

// Function to delete a teacher by ID
void delete_teacher(int teacher_id) {
    if (head == NULL) {
        printf("No teachers found. Cannot delete.\n");
        return;
    }

    // Find the teacher using the hash table
    Teacher *teacher = select_teacher_by_id(teacher_id);
    if (teacher == NULL) {
        printf("Teacher with ID %d not found.\n", teacher_id);
        return;
    }

    // Remove from the doubly linked list
    if (teacher->prev != NULL) {
        teacher->prev->next = teacher->next;
    } else {
        head = teacher->next;
    }

    if (teacher->next != NULL) {
        teacher->next->prev = teacher->prev;
    } else {
        tail = teacher->prev;
    }

    // Remove from the hash table
    deleteFromHashTable(teacher_id);

    // Free the memory
    free(teacher);
    printf("Teacher with ID %d deleted successfully.\n", teacher_id);
}

// Function to load teachers from the file into a dynamically allocated buffer
void load_teachers_from_file() {
    const char *file_path = "teachers.dat";
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("No existing teacher data found.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    TeacherFile *buffer = (TeacherFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed for buffer!\n");
        fclose(file);
        return;
    }

    // Read the file into the buffer
    fread(buffer, 1, file_size, file);
    fclose(file);

    // Calculate the number of teachers in the file
    int num_teachers = file_size / sizeof(TeacherFile);

    // Add each teacher from the buffer to the doubly linked list and hash table
    for (int i = 0; i < num_teachers; i++) {
        Teacher *new_teacher = (Teacher *)malloc(sizeof(Teacher));
        if (new_teacher == NULL) {
            printf("Memory allocation failed for new teacher!\n");
            return;
        }

        // Copy data from the buffer to the new teacher
        new_teacher->teacher_id = buffer[i].teacher_id;
        strcpy(new_teacher->first_name, buffer[i].first_name);
        strcpy(new_teacher->last_name, buffer[i].last_name);
        strcpy(new_teacher->subject_expertise, buffer[i].subject_expertise);
        new_teacher->next = NULL;
        new_teacher->prev = NULL;

        // Add the new teacher to the doubly linked list
        if (head == NULL) {
            head = new_teacher;
            tail = new_teacher;
        } else {
            tail->next = new_teacher;
            new_teacher->prev = tail;
            tail = new_teacher;
        }

        // Add the new teacher to the hash table
        insert(new_teacher->teacher_id, new_teacher);
    }

    free(buffer); // Free the buffer
    printf("Teachers loaded from file.\n");
}

// Function to save teachers to the file
void save_teachers_to_file() {
    const char *file_path = "C:\\Projects\\DBMS_project\\teachers.dat";
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Teacher *current = head;
    while (current != NULL) {
        // Save only the data fields (excluding next and prev pointers)
        TeacherFile temp;
        temp.teacher_id = current->teacher_id;
        strcpy(temp.first_name, current->first_name);
        strcpy(temp.last_name, current->last_name);
        strcpy(temp.subject_expertise, current->subject_expertise);

        fwrite(&temp, sizeof(TeacherFile), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Teachers saved to file.\n");
}

void select_teachers_by_subject(const char *subject) {
    int found = 0;

    printf("\nTeachers with Subject Expertise '%s':\n", subject);
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashArray[i] != NULL && hashArray[i]->key != DUMMY_KEY) {
            Teacher *teacher = hashArray[i]->data;
            if (strcmp(teacher->subject_expertise, subject) == 0) {
                printf("ID: %d, Name: %s %s, Subject: %s\n",
                       teacher->teacher_id,
                       teacher->first_name,
                       teacher->last_name,
                       teacher->subject_expertise);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No teachers found with Subject Expertise '%s'.\n", subject);
    }
}


// Function to check if a teacher ID exists in the Teacher table
int teacher_id_exists(int teacher_id) {
    return search(teacher_id) != NULL;
}

// Function to check if a phone number is unique in the Teacher_PhoneNumber table
int is_phone_number_unique(const char *phone_number) {
    return searchPhone(phone_number) == NULL;
}
void insert_phone_number() {
    TeacherPhone *new_phone = (TeacherPhone *)malloc(sizeof(TeacherPhone));
    if (new_phone == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Input teacher phone details
    printf("Enter Teacher ID: ");
    scanf("%d", &new_phone->teacher_id);

    // Check if the teacher ID exists in the Teacher table
    if (!teacher_id_exists(new_phone->teacher_id)) {
        printf("Error: Teacher ID %d does not exist in the Teacher table!\n", new_phone->teacher_id);
        free(new_phone);
        return;
    }

    printf("Enter Phone Number: ");
    scanf("%s", new_phone->phone_number);

    // Check if the phone number is unique
    if (!is_phone_number_unique(new_phone->phone_number)) {
        printf("Error: Phone number %s already exists!\n", new_phone->phone_number);
        free(new_phone);
        return;
    }

    // Initialize next and prev pointers
    new_phone->next = NULL;
    new_phone->prev = NULL;

    // Add the new phone number to the doubly linked list
    if (phone_head == NULL) {
        phone_head = new_phone;
        phone_tail = new_phone;
    } else {
        phone_tail->next = new_phone;
        new_phone->prev = phone_tail;
        phone_tail = new_phone;
    }

    // Add the new phone number to the hash table
    insertPhone(new_phone->phone_number, new_phone);

    printf("Phone number added successfully!\n");
}
// Function to display all teacher phone numbers
void display_phone_numbers() {
    if (phone_head == NULL) {
        printf("No phone numbers found.\n");
        return;
    }

    printf("\nTeacher Phone Numbers:\n");
    TeacherPhone *current = phone_head;
    while (current != NULL) {
        printf("Teacher ID: %d, Phone Number: %s\n",
               current->teacher_id,
               current->phone_number);
        current = current->next;
    }
}

// Function to delete all phone numbers for a specific teacher_id
void delete_phone_numbers_by_teacher_id(int teacher_id) {
    int deleted = 0;

    for (int i = 0; i < PHONE_HASH_TABLE_SIZE; i++) {
        if (phoneHashArray[i] != NULL && strcmp(phoneHashArray[i]->key, DUMMY_PHONE_KEY) != 0) {
            TeacherPhone *phone = phoneHashArray[i]->data;
            if (phone->teacher_id == teacher_id) {
                // Remove from the doubly linked list
                if (phone->prev != NULL) {
                    phone->prev->next = phone->next;
                } else {
                    phone_head = phone->next;
                }

                if (phone->next != NULL) {
                    phone->next->prev = phone->prev;
                } else {
                    phone_tail = phone->prev;
                }

                // Remove from the hash table
                deleteFromPhoneHashTable(phone->phone_number);

                // Free the memory
                TeacherPhone *temp = phone;
                free(temp);
                deleted = 1;
            }
        }
    }

    if (deleted) {
        printf("All phone numbers for Teacher ID %d deleted successfully.\n", teacher_id);
    } else {
        printf("No phone numbers found for Teacher ID %d.\n", teacher_id);
    }
}
void delete_phone_number_by_phone_number(const char *phone_number) {
    // Remove from the hash table
    deleteFromPhoneHashTable(phone_number);

    // Remove from the doubly linked list
    TeacherPhone *current = phone_head;
    int deleted = 0;

    while (current != NULL) {
        if (strcmp(current->phone_number, phone_number) == 0) {
            // Phone number entry found, delete it
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                phone_head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                phone_tail = current->prev;
            }

            TeacherPhone *temp = current;
            current = current->next;
            free(temp);
            deleted = 1;
        } else {
            current = current->next;
        }
    }

    if (deleted) {
        printf("Phone number %s deleted successfully.\n", phone_number);
    } else {
        printf("Phone number %s not found.\n", phone_number);
    }
}

// Function to delete a specific phone number by teacher_id and phone_number
void delete_phone_number_by_teacher_id_and_phone_number(int teacher_id, const char *phone_number) {
    TeacherPhone *current = phone_head;
    int deleted = 0;

    while (current != NULL) {
        if (current->teacher_id == teacher_id && strcmp(current->phone_number, phone_number) == 0) {
            // Phone number entry found, delete it
            if (current->prev != NULL) {
                // If the node is not the head, update the previous node's next pointer
                current->prev->next = current->next;
            } else {
                // If the node is the head, update the head pointer
                phone_head = current->next;
            }

            if (current->next != NULL) {
                // If the node is not the tail, update the next node's prev pointer
                current->next->prev = current->prev;
            } else {
                // If the node is the tail, update the tail pointer
                phone_tail = current->prev;
            }

            // Free the memory allocated for the deleted phone number entry
            TeacherPhone *temp = current;
            current = current->next;
            free(temp);
            deleted = 1;
            break; // Exit after deleting the specific record
        } else {
            current = current->next;
        }
    }

    if (deleted) {
        printf("Phone number %s for Teacher ID %d deleted successfully.\n", phone_number, teacher_id);
    } else {
        printf("Phone number %s for Teacher ID %d not found.\n", phone_number, teacher_id);
    }
}

// Function to update a phone number using phone_number (Unique Key)
void update_phone_number_by_phone_number() {
    if (phone_head == NULL) {
        printf("No phone numbers found. Cannot update.\n");
        return;
    }

    char old_phone_number[MAX_PHONE_LENGTH];
    char new_phone_number[MAX_PHONE_LENGTH];

    // Input old and new phone numbers
    printf("Enter Old Phone Number: ");
    scanf("%s", old_phone_number);
    printf("Enter New Phone Number: ");
    scanf("%s", new_phone_number);

    // Check if the new phone number is unique
    if (!is_phone_number_unique(new_phone_number)) {
        printf("Error: Phone number %s already exists!\n", new_phone_number);
        return;
    }

    // Find the old phone number in the hash table
    TeacherPhone *phone = searchPhone(old_phone_number);
    if (phone != NULL) {
        // Update the phone number in the linked list
        strcpy(phone->phone_number, new_phone_number);

        // Update the hash table
        deleteFromPhoneHashTable(old_phone_number);
        insertPhone(new_phone_number, phone);

        printf("Phone number updated successfully from %s to %s.\n", old_phone_number, new_phone_number);
    } else {
        printf("Phone number %s not found.\n", old_phone_number);
    }
}

// Function to update a phone number using teacher_id and phone_number (Composite Key)
void update_phone_number_by_teacher_id_and_phone_number() {
    if (phone_head == NULL) {
        printf("No phone numbers found. Cannot update.\n");
        return;
    }

    int teacher_id;
    char old_phone_number[MAX_PHONE_LENGTH];
    char new_phone_number[MAX_PHONE_LENGTH];

    // Input teacher ID, old phone number, and new phone number
    printf("Enter Teacher ID: ");
    scanf("%d", &teacher_id);
    printf("Enter Old Phone Number: ");
    scanf("%s", old_phone_number);
    printf("Enter New Phone Number: ");
    scanf("%s", new_phone_number);

    // Check if the new phone number is unique
    if (!is_phone_number_unique(new_phone_number)) {
        printf("Error: Phone number %s already exists!\n", new_phone_number);
        return;
    }

    // Find the old phone number in the hash table
    TeacherPhone *phone = searchPhone(old_phone_number);
    if (phone != NULL && phone->teacher_id == teacher_id) {
        // Update the phone number in the linked list
        strcpy(phone->phone_number, new_phone_number);

        // Update the hash table
        deleteFromPhoneHashTable(old_phone_number);
        insertPhone(new_phone_number, phone);

        printf("Phone number for Teacher ID %d updated successfully from %s to %s.\n", teacher_id, old_phone_number, new_phone_number);
    } else {
        printf("Phone number %s for Teacher ID %d not found.\n", old_phone_number, teacher_id);
    }
}
void load_phone_numbers_from_file() {
    const char *file_path = "teacher_phonenumbers.dat";
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("No existing phone number data found.\n");
        return;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    Teacher_PhoneNumberFile *buffer = (Teacher_PhoneNumberFile *)malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    // Read the file into the buffer
    fread(buffer, 1, file_size, file);
    fclose(file);

    // Calculate the number of phone numbers in the file
    int num_phones = file_size / sizeof(Teacher_PhoneNumberFile);

    // Add each phone number from the buffer to the doubly linked list and hash table
    for (int i = 0; i < num_phones; i++) {
        TeacherPhone *new_phone = (TeacherPhone *)malloc(sizeof(TeacherPhone));
        if (new_phone == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }

        // Copy data from the buffer to the new phone number
        new_phone->teacher_id = buffer[i].teacher_id;
        strcpy(new_phone->phone_number, buffer[i].phone_number);
        new_phone->next = NULL;
        new_phone->prev = NULL;

        // Add the new phone number to the doubly linked list
        if (phone_head == NULL) {
            phone_head = new_phone;
            phone_tail = new_phone;
        } else {
            phone_tail->next = new_phone;
            new_phone->prev = phone_tail;
            phone_tail = new_phone;
        }

        // Add the new phone number to the hash table
        insertPhone(new_phone->phone_number, new_phone);
    }

    free(buffer); // Free the buffer
    printf("Phone numbers loaded from file.\n");
}
// Function to save teacher phone numbers to the file
void save_phone_numbers_to_file() {
    const char *file_path = "C:\\Projects\\DBMS_project\\teacher_phonenumbers.dat";
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    TeacherPhone *current = phone_head;
    while (current != NULL) {
        // Save only the data fields (excluding next and prev pointers)
        Teacher_PhoneNumberFile temp;
        temp.teacher_id = current->teacher_id;
        strcpy(temp.phone_number, current->phone_number);

        fwrite(&temp, sizeof(Teacher_PhoneNumberFile), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Phone numbers saved to file.\n");
}






void display() {
    if (head == NULL) {
        printf("No teachers found.\n");
        return;
    }

    printf("\nTeachers:\n");
    Teacher *current = head;
    while (current != NULL) {
        printf("ID: %d, Name: %s %s, Subject: %s\n",
               current->teacher_id,
               current->first_name,
               current->last_name,
               current->subject_expertise);
        current = current->next;
    }
}


