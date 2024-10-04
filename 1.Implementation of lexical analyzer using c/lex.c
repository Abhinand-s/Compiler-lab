#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Returns 'true' if the character is a DELIMITER.
bool isDelimiter(char ch)
{
    return (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
            ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
            ch == '[' || ch == ']' || ch == '{' || ch == '}');
}

// Returns 'true' if the character is an OPERATOR.
bool isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '>' || ch == '<' ||
            ch == '=');
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(const char* str)
{
    if (str[0] == '\0')
        return false;

    if (!(str[0] == '_' || (str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z')))
        return false;

    for (int i = 1; str[i] != '\0'; i++) {
        if (!(str[i] == '_' || (str[i] >= 'a' && str[i] <= 'z') ||
              (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')))
            return false;
    }

    return true;
}

// Returns 'true' if the string is a KEYWORD.
bool isKeyword(const char* str)
{
    static const char *keywords[] = {
        "if", "else", "while", "do", "break", "continue",
        "int", "double", "float", "return", "char", "case",
        "sizeof", "long", "short", "typedef", "switch",
        "unsigned", "void", "static", "struct", "goto"
    };

    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0)
            return true;
    }
    return false;
}

// Returns 'true' if the string is an INTEGER.
bool isInteger(const char* str)
{
    int len = strlen(str);
    if (len == 0)
        return false;

    if (str[0] == '-' && len > 1) {
        // Check digits after a negative sign
        for (int i = 1; i < len; i++) {
            if (str[i] < '0' || str[i] > '9')
                return false;
        }
        return true;
    }

    // Check all digits
    for (int i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

// Returns 'true' if the string is a REAL NUMBER.
bool isRealNumber(const char* str)
{
    int len = strlen(str);
    bool hasDecimal = false;
    if (len == 0)
        return false;

    if (str[0] == '-' && len > 1) {
        // Check after negative sign
        for (int i = 1; i < len; i++) {
            if (str[i] == '.') {
                if (hasDecimal)
                    return false; // More than one decimal point
                hasDecimal = true;
            } else if (str[i] < '0' || str[i] > '9') {
                return false;
            }
        }
        return hasDecimal;
    }

    // Check for decimal point and digits
    for (int i = 0; i < len; i++) {
        if (str[i] == '.') {
            if (hasDecimal)
                return false; // More than one decimal point
            hasDecimal = true;
        } else if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }
    return hasDecimal;
}

// Extracts the SUBSTRING.
char* subString(const char* str, int left, int right)
{
    int len = right - left + 1;
    char* subStr = (char*)malloc(len + 1);
    if (subStr) {
        strncpy(subStr, str + left, len);
        subStr[len] = '\0';
    }
    return subStr;
}

// Parsing the input STRING.
void parse(const char* str, int lineNumber)
{
    int left = 0, right = 0;
    int len = strlen(str);

    while (right <= len) {
        // Skip spaces and update left
        while (right < len && str[right] == ' ') {
            right++;
            left = right;
        }

        // Move to the next token
        while (right < len && !isDelimiter(str[right]) && !isOperator(str[right])) {
            right++;
        }

        if (left < right) {
            char* subStr = subString(str, left, right - 1);

            if (isKeyword(subStr))
                printf("Line %d: '%s' IS A KEYWORD\n", lineNumber, subStr);
            else if (isInteger(subStr))
                printf("Line %d: '%s' IS AN INTEGER\n", lineNumber, subStr);
            else if (isRealNumber(subStr))
                printf("Line %d: '%s' IS A REAL NUMBER\n", lineNumber, subStr);
            else if (validIdentifier(subStr))
                printf("Line %d: '%s' IS A VALID IDENTIFIER\n", lineNumber, subStr);
            else
                printf("Line %d: '%s' IS NOT A VALID IDENTIFIER\n", lineNumber, subStr);

            free(subStr);
        }

        if (right < len) {
            if (isOperator(str[right]))
                printf("Line %d: '%c' IS AN OPERATOR\n", lineNumber, str[right]);

            // Handle special delimiters explicitly
            if (str[right] == '{' || str[right] == '}' || str[right] == ',' || str[right] == ';') {
                printf("Line %d: '%c' IS A SPECIAL DELIMITER\n", lineNumber, str[right]);
            }

            // Move past the delimiter or operator
            right++;
            left = right;
        } else {
            // Handle the case when right reaches the end of the string
            left = right;
            right++;
        }
    }
}

// DRIVER FUNCTION
int main()
{
    char filename[100];
    FILE *file;
    char str[1000]; // Increased size for potentially large files
    int lineNumber = 1;

    // Get the filename from the user
    printf("Enter the filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove the newline character from fgets

    // Open the file
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read the file content line by line
    while (fgets(str, sizeof(str), file) != NULL) {
        // Remove newline character if present
        str[strcspn(str, "\n")] = '\0';
        parse(str, lineNumber); // calling the parse function with line number
        lineNumber++;
    }

    // Close the file
    fclose(file);

    return 0;
}

