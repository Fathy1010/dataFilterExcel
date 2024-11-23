#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 2048
#define MAX_MEMBERS 10000

typedef struct {
    char first_name[50];
    char last_name[50];
    char email[100];
    char phone[20];
    char address_line[100];
    char city[50];
    char postcode[10];
    char grade[10];
    char birthday[20]; // Added Birthday field
    char family_uid[10];
    char family_role[20];
} Member;

Member members[MAX_MEMBERS];
int member_count = 0;
int family_count = 1; // Start Family UID from 1

// Helper function to trim spaces and newlines
void trim_field(char *field) {
    char *end;
    // Trim leading spaces
    while (isspace((unsigned char)*field)) field++;
    // Trim trailing spaces and newlines
    end = field + strlen(field) - 1;
    while (end > field && (isspace((unsigned char)*end) || *end == '\n')) {
        *end = '\0';
        end--;
    }
}

// Helper function to split names into first and last
void split_name(const char *full_name, char *first_name, char *last_name) {
    const char *space = strchr(full_name, ' ');
    if (space) {
        strncpy(first_name, full_name, space - full_name); // First name before space
        first_name[space - full_name] = '\0';
        strncpy(last_name, space + 1, 49); // Last name after space
    } else {
        strncpy(first_name, full_name, 49); // Entire name as first name if no space
        last_name[0] = '\0';                // Empty last name
    }
}

// Add a member to the members array
void add_member(const char *first_name, const char *last_name, const char *email,
                const char *phone, const char *address_line, const char *city,
                const char *postcode, const char *grade, const char *birthday,
                const char *family_uid, const char *family_role) {
    if (member_count >= MAX_MEMBERS) {
        fprintf(stderr, "Error: Maximum member limit reached.\n");
        return;
    }

    strncpy(members[member_count].first_name, first_name, sizeof(members[member_count].first_name) - 1);
    strncpy(members[member_count].last_name, last_name, sizeof(members[member_count].last_name) - 1);
    strncpy(members[member_count].email, email, sizeof(members[member_count].email) - 1);
    strncpy(members[member_count].phone, phone, sizeof(members[member_count].phone) - 1);
    strncpy(members[member_count].address_line, address_line, sizeof(members[member_count].address_line) - 1);
    strncpy(members[member_count].city, city, sizeof(members[member_count].city) - 1);
    strncpy(members[member_count].postcode, postcode, sizeof(members[member_count].postcode) - 1);
    strncpy(members[member_count].grade, grade, sizeof(members[member_count].grade) - 1);
    strncpy(members[member_count].birthday, birthday, sizeof(members[member_count].birthday) - 1);
    strncpy(members[member_count].family_uid, family_uid, sizeof(members[member_count].family_uid) - 1);
    strncpy(members[member_count].family_role, family_role, sizeof(members[member_count].family_role) - 1);

    // Sanitize all fields to ensure no line breaks or unwanted spaces
    trim_field(members[member_count].first_name);
    trim_field(members[member_count].last_name);
    trim_field(members[member_count].email);
    trim_field(members[member_count].phone);
    trim_field(members[member_count].address_line);
    trim_field(members[member_count].city);
    trim_field(members[member_count].postcode);
    trim_field(members[member_count].grade);
    trim_field(members[member_count].birthday);
    trim_field(members[member_count].family_uid);
    trim_field(members[member_count].family_role);

    member_count++;
}

// Custom CSV parser to handle quoted fields and empty fields
void parse_csv_row(const char *line, char fields[][MAX_LINE], int *num_fields) {
    *num_fields = 0;
    int in_quotes = 0;
    const char *start = line;
    char field[MAX_LINE] = {0};
    char *out = field;

    for (const char *p = line; *p; p++) {
        if (*p == '"') {
            in_quotes = !in_quotes;
        } else if (*p == ',' && !in_quotes) {
            *out = '\0'; // Null-terminate field
            strncpy(fields[(*num_fields)++], field, MAX_LINE - 1);
            out = field; // Reset output buffer
        } else {
            *out++ = *p; // Add character to field
        }
    }

    // Add the last field
    if (out != field) {
        *out = '\0';
        strncpy(fields[(*num_fields)++], field, MAX_LINE - 1);
    }
}

// Process a single CSV line
void process_csv_row(const char *line) {
    char fields[20][MAX_LINE]; // To hold extracted fields
    int num_fields = 0;

    parse_csv_row(line, fields, &num_fields);

    if (num_fields < 10) {
        fprintf(stderr, "Skipping row: Invalid number of fields (%d).\n", num_fields);
        return;
    }

    char email[100], child_name[100], grade[10], address[100], city[50], postcode[10], birthday[20];
    char parent1_name[100], parent1_phone[20], parent2_name[100], parent2_phone[20];
    char child_first[50], child_last[50], parent1_first[50], parent1_last[50], parent2_first[50], parent2_last[50];
    char family_uid[10];

    // Extract fields
    strncpy(email, fields[0], 99);
    strncpy(child_name, fields[1], 99);
    strncpy(grade, fields[2], 9);
    strncpy(birthday, fields[3], 19); // Extract Birthday
    strncpy(address, fields[4], 99);
    strncpy(city, fields[5], 49);
    strncpy(postcode, fields[6], 9);
    strncpy(parent1_name, fields[7], 99);
    strncpy(parent1_phone, fields[8], 19);
    strncpy(parent2_name, fields[9], 99);
    strncpy(parent2_phone, fields[10], 19);

    // Set default phone numbers only for explicitly empty fields
    if (strcmp(parent1_phone, "") == 0) {
        strncpy(parent1_phone, "04", 19);
    }
    if (strcmp(parent2_phone, "") == 0) {
        strncpy(parent2_phone, "04", 19);
    }

    // Generate Family UID
    snprintf(family_uid, sizeof(family_uid), "%06d", family_count++);

    // Split child name and add to members
    split_name(child_name, child_first, child_last);
    add_member(child_first, child_last, email, "", address, city, postcode, grade, birthday, family_uid, "Child");

    // Split Parent 1 name and add to members
    split_name(parent1_name, parent1_first, parent1_last);
    add_member(parent1_first, parent1_last, email, parent1_phone, address, city, postcode, "", "", family_uid, "Spouse");

    // Only process Parent 2 if name is not empty
    if (strlen(parent2_name) > 0) {
        split_name(parent2_name, parent2_first, parent2_last);
        add_member(parent2_first, parent2_last, email, parent2_phone, address, city, postcode, "", "", family_uid, "Spouse");
    }
}

// Write output to CSV file
void write_output(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening output file");
        return;
    }

    // Write updated header
    fprintf(file, "Person Id,First Name,Last Name,Middle Name,Nickname,Gender,Marital Status,Anniversary,Engagement Date,Birthdate,Age,Mobile Phone,Email,Deacon,Rank,Donor Bishop,Father of Confession,School,Grade,Address Line,City,State,Postal Code,Notes,Group Name,Family Id,Family Role,Update Date,Sunday School Servant,Guardian Full Name,Guardian Phone Number,Year Level Serving:,Guardian Full Name,WWCC number,Guardian Phone Number,Expiry date\n");

    // Write member data with empty fields where applicable
    for (int i = 0; i < member_count; i++) {
        fprintf(file, ",%s,%s,,,,,,,,%s,%s,%s,,,,%s,%s,%s,,,,%s,,%s,,,,,,,,,\n",
                members[i].first_name,    // First Name
                members[i].last_name,     // Last Name
                members[i].birthday,      // Birthdate
                members[i].phone,         // Mobile Phone
                members[i].email,         // Email
                members[i].grade,         // Grade
                members[i].address_line,  // Address Line
                members[i].city,          // City
                members[i].postcode,      // Postal Code
                members[i].family_uid,    // Family ID
                members[i].family_role);  // Family Role
    }

    fclose(file);
    printf("Processed data saved to %s\n", filename);
}

int main() {
    char line[MAX_LINE];
    FILE *file = fopen("output.csv", "r");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    // Skip header line
    fgets(line, sizeof(line), file);

    // Process each line
    while (fgets(line, sizeof(line), file)) {
        process_csv_row(line);
    }
    fclose(file);

    // Write formatted output
    write_output("formatted_output.csv");
    return 0;
}

