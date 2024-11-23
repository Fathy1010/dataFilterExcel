# README: Processing CSV Data to Generate Formatted Output

## Overview
This program processes an input CSV file (`output.csv`) containing family and child data and generates a formatted CSV output file (`formatted_output.csv`) with the desired structure and headers.

### Features
- Handles missing fields (e.g., phone numbers, empty parent entries).
- Includes additional columns like `Birthdate` in the output.
- Generates a unique `Family UID` for each family.
- Ensures consistent formatting of the output with empty fields for missing data.
- Supports configurable headers as per your requirements.

---

## Dependencies

### Required Software
1. **GCC Compiler**:
   - Install GCC to compile and run the program.
   - On macOS, you can install GCC using Xcode:
     ```bash
     xcode-select --install
     ```
   - Alternatively, install GCC via Homebrew:
     ```bash
     brew install gcc
     ```

2. **Text Editor**:
   - Use any text editor or IDE to modify the code if needed (e.g., Visual Studio Code, nano, Vim).

---

## Steps to Set Up

### Step 1: Clone or Copy the Project
1. Copy the source code (`processFamilyData.c`) into a folder on your system.

### Step 2: Prepare the Input File
1. Ensure the input file (`output.csv`) is present in the same folder as the program.
2. The input file should follow this structure:
   ```csv
   "Email address","Child Name (First Name, Last Name)","Child’s Grade (in 2024)","Child Date of Birth","Street address (house number and street name)",City,Postcode,"Parent 1 Name","Parent 1 Phone","Parent 2 Name","Parent 2 Phone"

### Step 3: Compile the Program
1. Open a terminal in the folder where the source code is saved.
2. Compile the program using GCC:
   ```bash
   gcc -o processFamilyData processFamilyData.c

Step 4: Run the Program
Execute the compiled program:
bash
./processFamilyData
The program will:
Read the output.csv file.
Generate a formatted CSV file named formatted_output.csv.