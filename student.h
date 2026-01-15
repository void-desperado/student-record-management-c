#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define NAME_LEN 100
#define COURSE_LEN 64

typedef struct {
    int roll;
    char name[NAME_LEN];
    int age;
    char course[COURSE_LEN];
    float gpa;
} Student;

/* CRUD operations working on a binary file specified by filename.
   Return values:
     1 = success / found
     0 = not found / failure
    -1 = file error
*/

int add_student(const char *filename, const Student *s);
int view_all(const char *filename);
int find_student_by_roll(const char *filename, int roll, Student *out);
int update_student(const char *filename, int roll, const Student *newdata);
int delete_student(const char *filename, int roll);

#endif /* STUDENT_H */
