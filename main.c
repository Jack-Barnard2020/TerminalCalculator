#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS 0
#define FAIL 1

// Function declarations
void help();                      
double readCalcAns();             
void writeCalcAns(double);        
void displayAns(double, char, double); 
int isValidNumber(const char*);   
void clearAns();                  
void printLastAns();              // Prints last saved answer

int main(int argc, char *argv[]) {
    // Help option
    if (argc == 2 && strcmp(argv[1], "help") == 0) {
        help();
        return SUCCESS;
    }

    // Clear saved answer
    if (argc == 2 && strcmp(argv[1], "clear") == 0) {
        clearAns();
        return SUCCESS;
    }

    // Print last saved answer
    if (argc == 2 && strcmp(argv[1], "ans") == 0) {
        printLastAns();
        return SUCCESS;
    }

    // Check for correct format
    if (argc != 4) {
        printf("Incorrect format. Type 'calc help' for usage instructions.\n");
        return FAIL;
    }

    double num1, num2, ans;
    char opp = argv[2][0];

    // Parse first number
    if (strcmp(argv[1], "ans") == 0) {
        num1 = readCalcAns();
    } else if (isValidNumber(argv[1])) {
        num1 = atof(argv[1]);
    } else {
        printf("Error: Invalid first number '%s'.\n", argv[1]);
        return FAIL;
    }

    // Parse second number
    if (strcmp(argv[3], "ans") == 0) {
        num2 = readCalcAns();
    } else if (isValidNumber(argv[3])) {
        num2 = atof(argv[3]);
    } else {
        printf("Error: Invalid second number '%s'.\n", argv[3]);
        return FAIL;
    }

    // Perform calculation
    switch (opp) {
        case '+': ans = num1 + num2; break;
        case '-': ans = num1 - num2; break;
        case 'x':
        case '*': ans = num1 * num2; break;
        case '/':
            if (num2 == 0) {
                printf("Error: Division by zero is undefined.\n");
                return FAIL;
            }
            ans = num1 / num2;
            break;
        default:
            printf("Error: Invalid operation '%c'. Supported operators: +, -, x, *, /\n", opp);
            return FAIL;
    }

    // Display and save result
    displayAns(num1, opp, num2);
    writeCalcAns(ans);

    return SUCCESS;
}

// Prints detailed help message
void help() {
    printf("Welcome to the Enhanced Simple Calculator!\n");
    printf("Supported operations:\n");
    printf("  Addition       : +\n");
    printf("  Subtraction    : -\n");
    printf("  Multiplication : x or *\n");
    printf("  Division       : /\n\n");
    printf("Special commands:\n");
    printf("  help           : Show this help message\n");
    printf("  clear          : Clear the saved answer\n");
    printf("  ans            : Print the last saved answer\n\n");
    printf("Usage:\n");
    printf("  calc <num1> <operator> <num2>\n");
    printf("  - num1, num2 can be numbers or 'ans' to use previous result.\n\n");
    printf("Examples:\n");
    printf("  calc 5 + 3       -> 5 + 3 = 8\n");
    printf("  calc ans x 2     -> previous answer * 2\n");
    printf("  calc 10 / ans    -> 10 / previous answer\n");
    printf("  calc ans         -> print last answer\n");
}

// Displays the calculation result clearly
void displayAns(double num1, char opp, double num2) {
    double result = (opp == '+') ? num1 + num2 :
                    (opp == '-') ? num1 - num2 :
                    (opp == 'x' || opp == '*') ? num1 * num2 :
                    (opp == '/' && num2 != 0) ? num1 / num2 : 0;
    printf("%.3lf %c %.3lf = %lf\n", num1, opp, num2, result);
}

// Reads previous answer from file
double readCalcAns() {
    FILE *file = fopen("CalcAns.txt", "r");
    if (!file) {
        printf("No previous answer found. Defaulting to 0.\n");
        return 0.0;
    }
    double value;
    if (fscanf(file, "%lf", &value) != 1)
        value = 0.0;
    fclose(file);
    return value;
}

// Writes current answer to file
void writeCalcAns(double ans) {
    FILE *file = fopen("CalcAns.txt", "w");
    if (!file) {
        printf("Error: Unable to write to file.\n");
        return;
    }
    fprintf(file, "%lf", ans);
    fclose(file);
}

// Checks if string is a valid number (integer or decimal)
int isValidNumber(const char* str) {
    int dotSeen = 0;
    if (*str == '-' || *str == '+') str++; // optional sign
    if (!*str) return 0; // empty string after sign

    while (*str) {
        if (*str == '.') {
            if (dotSeen) return 0; // multiple dots invalid
            dotSeen = 1;
        } else if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

// Clears saved answer
void clearAns() {
    FILE *file = fopen("CalcAns.txt", "w");
    if (!file) {
        printf("Error: Unable to clear answer file.\n");
        return;
    }
    fclose(file);
    printf("Saved answer cleared successfully.\n");
}

// Prints last saved answer
void printLastAns() {
    double ans = readCalcAns();
    printf("Last saved answer: %lf\n", ans);
}
