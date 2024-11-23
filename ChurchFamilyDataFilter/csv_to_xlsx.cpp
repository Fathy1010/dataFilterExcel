#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "xlsxwriter.h"

// Define the required headers in the specified format
const std::vector<std::string> REQUIRED_HEADERS = {
    "Person Id", "First Name", "Last Name", "Middle Name", "Nickname", "Gender",
    "Marital Status", "Anniversary", "Engagement Date", "Birthdate", "Age", "Mobile Phone",
    "Email", "Deacon", "Rank", "Donor Bishop", "Father of Confession", "School", "Grade",
    "Address Line", "City", "State", "Postal Code", "Notes", "Group Name", "Family Id",
    "Family Role", "Update Date", "Sunday School Servant", "Guardian Full Name",
    "Guardian Phone Number", "Year Level Serving:", "Guardian Full Name", "WWCC number",
    "Guardian Phone Number", "Expiry date"
};

void csv_to_xlsx(const std::string& csv_file, const std::string& xlsx_file) {
    // Open the CSV file
    std::ifstream infile(csv_file);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << csv_file << std::endl;
        return;
    }

    // Create a new XLSX file
    lxw_workbook *workbook = workbook_new(xlsx_file.c_str());
    if (!workbook) {
        std::cerr << "Error: Could not create XLSX file " << xlsx_file << std::endl;
        return;
    }
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    // Write the header row
    for (size_t col = 0; col < REQUIRED_HEADERS.size(); col++) {
        worksheet_write_string(worksheet, 0, col, REQUIRED_HEADERS[col].c_str(), NULL);
    }

    // Read the CSV line by line and map it to the required headers
    std::string line;
    int row = 1;

    // Define the mapping for known CSV columns to the XLSX headers
    std::map<std::string, int> csv_to_header_map = {
        {"First Name", 1}, {"Last Name", 2}, {"Email", 12}, {"Mobile Phone", 11},
        {"Grade", 18}, {"Address Line", 19}, {"City", 20}, {"Postal Code", 22},
        {"Family Id", 26}, {"Family Role", 27}
    };

    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;

        // Read each cell in the CSV row
        std::vector<std::string> row_data(REQUIRED_HEADERS.size(), ""); // Initialize empty row
        while (std::getline(ss, cell, ',')) {
            if (csv_to_header_map.find(REQUIRED_HEADERS[col]) != csv_to_header_map.end()) {
                row_data[csv_to_header_map[REQUIRED_HEADERS[col]]] = cell;
            }
            col++;
        }

        // Write the mapped row data to the XLSX file
        for (size_t col = 0; col < row_data.size(); col++) {
            worksheet_write_string(worksheet, row, col, row_data[col].c_str(), NULL);
        }
        row++;
    }

    // Close the CSV file and the XLSX workbook
    infile.close();
    workbook_close(workbook);

    std::cout << "Converted " << csv_file << " to " << xlsx_file << " with required headers." << std::endl;
}

int main() {
    int num_files;
    std::cout << "Enter the number of CSV files to convert: ";
    std::cin >> num_files;

    for (int i = 0; i < num_files; i++) {
        std::string csv_file;
        std::cout << "Enter the path for CSV file " << (i + 1) << ": ";
        std::cin >> csv_file;

        // Generate the output XLSX file name
        std::string xlsx_file = csv_file.substr(0, csv_file.find_last_of('.')) + ".xlsx";

        // Convert the CSV to XLSX
        csv_to_xlsx(csv_file, xlsx_file);
    }

    return 0;
}
