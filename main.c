#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

#define DB_FILE "students.dat"

static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void read_line(char *buf, size_t len) {
    if (!fgets(buf, (int)len, stdin)) {
        buf[0] = '\0';
        return;
    }
    size_t l = strlen(buf);
    if (l && buf[l-1] == '\n') buf[l-1] = '\0';
}

void input_student(Student *s) {
    printf("Enter roll (integer): ");
    while (scanf("%d", &s->roll) != 1) {
        printf("Invalid. Enter an integer for roll: ");
        clear_stdin();
    }
    clear_stdin();

    printf("Enter name: ");
    read_line(s->name, NAME_LEN);

    printf("Enter age: ");
    while (scanf("%d", &s->age) != 1) {
        printf("Invalid. Enter integer for age: ");
        clear_stdin();
    }
    clear_stdin();

    printf("Enter course: ");
    read_line(s->course, COURSE_LEN);

    printf("Enter GPA (float): ");
    while (scanf("%f", &s->gpa) != 1) {
        printf("Invalid. Enter a float for GPA: ");
        clear_stdin();
    }
    clear_stdin();
}

int main(void) {
    int choice;
    while (1) {
        printf("\nStudent Record Management\n");
        printf("1. Add student\n");
        printf("2. View all students\n");
        printf("3. Search by roll\n");
        printf("4. Update student\n");
        printf("5. Delete student\n");
        printf("0. Exit\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            continue;
        }
        clear_stdin();

        if (choice == 0) break;

        if (choice == 1) {
            Student s;
            input_student(&s);
            int r = add_student(DB_FILE, &s);
            if (r == 1) printf("Added.\n");
            else printf("Error adding (file error).\n");
        } else if (choice == 2) {
            view_all(DB_FILE);
        } else if (choice == 3) {
            int roll;
            printf("Enter roll to search: ");
            if (scanf("%d", &roll) != 1) { clear_stdin(); continue; }
            clear_stdin();
            Student s;
            int r = find_student_by_roll(DB_FILE, roll, &s);
            if (r == 1) {
                printf("Found: roll=%d, name=%s, age=%d, course=%s, gpa=%.2f\n",
                       s.roll, s.name, s.age, s.course, s.gpa);
            } else if (r == 0) {
                printf("Not found.\n");
            } else {
                printf("File error.\n");
            }
        } else if (choice == 4) {
            int roll;
            printf("Enter roll to update: ");
            if (scanf("%d", &roll) != 1) { clear_stdin(); continue; }
            clear_stdin();
            Student s;
            if (find_student_by_roll(DB_FILE, roll, &s) != 1) {
                printf("Record not found.\n");
            } else {
                printf("Enter new details for roll %d\n", roll);
                s.roll = roll; // keep same roll
                input_student(&s); // this asks for roll again; you can optionally skip input for roll
                int r = update_student(DB_FILE, roll, &s);
                printf(r==1 ? "Updated.\n" : "Update failed.\n");
            }
        } else if (choice == 5) {
            int roll;
            printf("Enter roll to delete: ");
            if (scanf("%d", &roll) != 1) { clear_stdin(); continue; }
            clear_stdin();
            int r = delete_student(DB_FILE, roll);
            if (r == 1) printf("Deleted.\n");
            else if (r == 0) printf("Not found or error.\n");
            else printf("File error.\n");
        } else {
            printf("Invalid choice.\n");
        }
    }

    printf("Goodbye.\n");
    return 0;
}
