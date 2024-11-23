#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_LINE 1024
#define MAX_FAMILIES 1000
#define MAX_MEMBERS 10000

typedef struct {
    char family_uid[50];
    char address_line[100];
    char city[50];
    char postcode[10];
    char parent1_name[50];
    char parent1_phone[20];
    char parent2_name[50];
    char parent2_phone[20];
    char email[100];
    int family_size;
} Family;

typedef struct {
    char first_name[50];
    char last_name[50];
    char email[100];
    char address_line[100];
    char city[50];
    char postcode[10];
    char grade[10];
    char family_uid[50];
} Member;

Family families[MAX_FAMILIES];
Member members[MAX_MEMBERS];
int family_count = 0;
int member_count = 0;

void generate_uid(char *uid, int id) {
    sprintf(uid, "FAM%04d", id);
}

int find_family(const char *address_line, const char *city, const char *postcode) {
    for (int i = 0; i < family_count; i++) {
        if (strcmp(families[i].address_line, address_line) == 0 &&
            strcmp(families[i].city, city) == 0 &&
            strcmp(families[i].postcode, postcode) == 0) {
            return i;
        }
    }
    return -1;
}

void process_csv_row(const char *line) {
    char email[100], child_name[100], grade[10], address_line[100], city[50], postcode[10];
    char parent1_name[50], parent1_phone[20], parent2_name[50], parent2_phone[20];
    char first_name[50], last_name[50];

    printf("Processing line: %s\n", line);

    char line_copy[MAX_LINE];
    strncpy(line_copy, line, MAX_LINE - 1);
    line_copy[MAX_LINE - 1] = '\0';

    char *token = strtok(line_copy, ",");
    int col = 0;

    while (token) {
        // Remove surrounding quotes if present
        if (token[0] == '"') {
            memmove(token, token + 1, strlen(token));
            char* end = strchr(token, '"');
            if (end) *end = '\0';
        }

        switch (col) {
            case 1: strncpy(email, token, sizeof(email) - 1); email[sizeof(email) - 1] = '\0'; break;
            case 2: strncpy(child_name, token, sizeof(child_name) - 1); child_name[sizeof(child_name) - 1] = '\0'; break;
            case 3: strncpy(grade, token, sizeof(grade) - 1); grade[sizeof(grade) - 1] = '\0'; break;
            case 5: strncpy(address_line, token, sizeof(address_line) - 1); address_line[sizeof(address_line) - 1] = '\0'; break;
            case 6: strncpy(city, token, sizeof(city) - 1); city[sizeof(city) - 1] = '\0'; break;
            case 7: strncpy(postcode, token, sizeof(postcode) - 1); postcode[sizeof(postcode) - 1] = '\0'; break;
            case 8: strncpy(parent1_name, token, sizeof(parent1_name) - 1); parent1_name[sizeof(parent1_name) - 1] = '\0'; break;
            case 9: strncpy(parent1_phone, token, sizeof(parent1_phone) - 1); parent1_phone[sizeof(parent1_phone) - 1] = '\0'; break;
            case 10: strncpy(parent2_name, token, sizeof(parent2_name) - 1); parent2_name[sizeof(parent2_name) - 1] = '\0'; break;
            case 11: strncpy(parent2_phone, token, sizeof(parent2_phone) - 1); parent2_phone[sizeof(parent2_phone) - 1] = '\0'; break;
        }
        token = strtok(NULL, ",");
        col++;
    }

    char *space = strchr(child_name, ' ');
    if (space) {
        *space = '\0';
        strncpy(first_name, child_name, sizeof(first_name) - 1);
        first_name[sizeof(first_name) - 1] = '\0';
        strncpy(last_name, space + 1, sizeof(last_name) - 1);
        last_name[sizeof(last_name) - 1] = '\0';
    } else {
        strncpy(first_name, child_name, sizeof(first_name) - 1);
        first_name[sizeof(first_name) - 1] = '\0';
        last_name[0] = '\0';
    }

    int family_index = find_family(address_line, city, postcode);
    char family_uid[50];

    if (family_index == -1) {
        generate_uid(family_uid, family_count + 1);
        strncpy(families[family_count].family_uid, family_uid, sizeof(family_uid));
        strncpy(families[family_count].address_line, address_line, sizeof(address_line));
        strncpy(families[family_count].city, city, sizeof(city));
        strncpy(families[family_count].postcode, postcode, sizeof(postcode));
        strncpy(families[family_count].parent1_name, parent1_name, sizeof(parent1_name));
        strncpy(families[family_count].parent1_phone, parent1_phone, sizeof(parent1_phone));
        strncpy(families[family_count].parent2_name, parent2_name, sizeof(parent2_name));
        strncpy(families[family_count].parent2_phone, parent2_phone, sizeof(parent2_phone));
        strncpy(families[family_count].email, email, sizeof(email));
        families[family_count].family_size = 1;
        family_index = family_count;
        family_count++;
    } else {
        strncpy(family_uid, families[family_index].family_uid, sizeof(family_uid));
        families[family_index].family_size++;
    }

    strncpy(members[member_count].first_name, first_name, sizeof(first_name));
    strncpy(members[member_count].last_name, last_name, sizeof(last_name));
    strncpy(members[member_count].email, email, sizeof(email));
    strncpy(members[member_count].address_line, address_line, sizeof(address_line));
    strncpy(members[member_count].city, city, sizeof(city));
    strncpy(members[member_count].postcode, postcode, sizeof(postcode));
    strncpy(members[member_count].grade, grade, sizeof(grade));
    strncpy(members[member_count].family_uid, families[family_index].family_uid, sizeof(members[member_count].family_uid));
    member_count++;
}


void process_csv_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening CSV file");
        return;
    }

    char line[MAX_LINE];
    fgets(line, sizeof(line), file); // Skip header line

    while (fgets(line, sizeof(line), file)) {
        process_csv_row(line);
    }

    fclose(file);
}

void write_output(const char *filename) {
    if (member_count == 0) {
        printf("No data to write. Skipping output.\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening output file");
        return;
    }

    fprintf(file, "First Name,Last Name,Email,Phone Number,Address Line,City,Postcode,Grade,Family UID\n");
    for (int i = 0; i < member_count; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s\n",
                members[i].first_name,
                members[i].last_name,
                members[i].email,
                members[i].address_line,
                members[i].city,
                members[i].postcode,
                members[i].grade,
                members[i].family_uid);
    }
    fclose(file);
    printf("Output written to %s\n", filename);
}


int main() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (!dir) {
        perror("Error opening directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".csv") != NULL) {
            process_csv_file(entry->d_name);
        }
    }
    closedir(dir);

    write_output("formatted_output.csv");
    printf("Processed data saved to formatted_output.csv\n");
    return 0;
}
