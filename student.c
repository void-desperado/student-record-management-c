#include "student.h"
#include <stdlib.h>
#include <string.h>

int add_student(const char *filename, const Student *s) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) return -1;
    if (fwrite(s, sizeof(Student), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

int view_all(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        // no file yet is not an error; treat as empty.
        printf("No records found (file missing).\n");
        return 0;
    }

    Student s;
    int count = 0;
    printf("All students:\n");
    printf("---------------------------------------------------------------\n");
    printf("%-6s %-20s %-4s %-12s %-4s\n", "Roll", "Name", "Age", "Course", "GPA");
    printf("---------------------------------------------------------------\n");
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-6d %-20s %-4d %-12s %-4.2f\n",
               s.roll, s.name, s.age, s.course, s.gpa);
        count++;
    }
    fclose(fp);
    if (count == 0) printf("(no records)\n");
    return count;
}

int find_student_by_roll(const char *filename, int roll, Student *out) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            if (out) *out = s;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int update_student(const char *filename, int roll, const Student *newdata) {
    FILE *fp = fopen(filename, "r+b");
    if (!fp) return -1;
    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            // move back one record and overwrite
            if (fseek(fp, -((long)sizeof(Student)), SEEK_CUR) != 0) {
                fclose(fp);
                return 0;
            }
            if (fwrite(newdata, sizeof(Student), 1, fp) != 1) {
                fclose(fp);
                return 0;
            }
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0; // not found
}

int delete_student(const char *filename, int roll) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    FILE *tmp = fopen("students.tmp", "wb");
    if (!tmp) { fclose(fp); return -1; }

    Student s;
    int deleted = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            deleted = 1;
            continue; // skip writing this one
        }
        fwrite(&s, sizeof(Student), 1, tmp);
    }

    fclose(fp);
    fclose(tmp);

    // replace original file
    if (remove(filename) != 0) {
        // might fail if file doesn't exist but we already had it; treat failure as error
        return 0;
    }
    if (rename("students.tmp", filename) != 0) {
        return 0;
    }
    return deleted ? 1 : 0;
}
