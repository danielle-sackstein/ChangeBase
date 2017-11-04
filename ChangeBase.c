/**
 * question 3
 * When reading a value from a string according to a given base I used parseIntegerByBase.
 * When converting a value to a string using a given base I used convertToStringByBase.
 *
 * parseInteger:
 * 	In this method I accumulate the value of each digit
 * 	from the last integer to the first, multiplying each one first by the correct power of the
 * 	base.
 * 	If the number is n, the complexity is log(n) (because we iterate for each digit).
 * 	It is o(n) where n is the number of digits, but it is log (n) in the value of the number
 *
 *
 * convertToStringByBase
 *	In this method I calculate the digits from the least significant to the most significant as the
 *	remainder of a value which starts as the number is is divided by the base in each iteration
 *	If the number is n, the complexity is log(n) (because we iterate for each digit).
 * 	It is o(n) where n is the number of digits, but it is log (n) in the value of the number
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* internal errors */

#define OK                              0
#define NOT_A_NUMBER                    1
#define FAILED_TO_READ_INPUT            2

#define EXPECTED_ARG_COUNT              3
#define MAX_DIGITS_IN_ORIGINAL_NUMBER   6

/*
 * The maximum number of digits required for the new representation is 20
 * because the original number is max 999999 and the minimum newbase is 2
 * and 999999 in base 2 requires 20 digits
 * (1024 * 1024 is equal to 20 1's in base 2)
*/
#define MAX_DIGITS_IN_NEW_NUMBER        20
#define MAX_DIGITS_IN_BASE              2
#define MAX_CHARS_IN_INPUT                   \
            (MAX_DIGITS_IN_BASE + 1 +        \
            MAX_DIGITS_IN_BASE + 1 +         \
            MAX_DIGITS_IN_ORIGINAL_NUMBER + 1)

/**
 * Receives a string and turns it to a number in a specific base.
 * @param str the original string (a string of a number)
 * @param base the base which we want to represent the number with
 * @param pResult the number represented in the new base.
 * @return 0 if the program did not fail, and 1 otherwise.
 */
int parseIntegerByBase(char *str, int base, int *pResult) {
    *pResult = 0;

    size_t length = strlen(str);
    int power = 1;

    char *ptr = str + length - 1;

    for (size_t i = 0; i < length; i++) {

        int digit = *ptr - '0';

        if ((digit < 0) || (digit > (base - 1))) {
            return NOT_A_NUMBER;
        }

        *pResult += power * digit;
        power *= base;
        ptr--;
    }
    return OK;
}

// 932 / 4 : 233 0
// 233 / 4 : 58  1
// 58 / 4  : 14  2
// 14 / 4  : 3   2
// 3 / 4   : 0   3

/**
 * This method creates a string representation of a number according to a given base.
 * @param value the number
 * @param base the base that should be used to create the string represenation
 * @param outputString is a string which will contain the result.
 * @return a point to one of the characters in the outputString
 * This pointer points to the first character in the string representation.
 * A 0 is placed in the last character of outputString so the return value points to a valid string
 */
char *convertToStringByBase(
        int value,
        int base,
        char outputString[MAX_DIGITS_IN_NEW_NUMBER + 1]) {
    int startIndex = MAX_DIGITS_IN_NEW_NUMBER;
    outputString[startIndex] = 0;

    for (int ratio = value; ratio > 0 && startIndex > 0; startIndex--) {
        outputString[startIndex - 1] = '0' + ratio % base;
        ratio /= base;
    }

    return outputString + startIndex;
}

/**
 * Parses the arguments by parsing each integer.
 * @param argv a pointer to an array of chars
 * @return 0 if the program did not fail, and 1 otherwise.
 */
int parseArguments(
        char *argv[EXPECTED_ARG_COUNT],
        int *pNewBase,
        int *pOriginalValue) {
    int originalBase = 10;

    int error = parseIntegerByBase(argv[0], originalBase, &originalBase);
    if (error != OK) {
        return error;
    }

    error = parseIntegerByBase(argv[1], originalBase, pNewBase);
    if (error != OK) {
        return error;
    }

    return parseIntegerByBase(argv[2], originalBase, pOriginalValue);
}

/**
 * Parses the input string. Separates the numbers by " " by replacing it with Null.
 * @param inputString  pointer to char
 * @param argv
 */
void parseInputString(
        char inputString[MAX_CHARS_IN_INPUT],
        char *argv[EXPECTED_ARG_COUNT]) {
    char delimiters[] = " \n";

    argv[0] = strtok(inputString, delimiters);
    argv[1] = strtok(0, delimiters);
    argv[2] = strtok(0, delimiters);
}

/**
 *  This method receives an input from the user. A valid input is:
 *  base, " ", base, " ", number.
 * @param argv a pointer to an array of chars
 * @return 0 if the program did not fail, and 1 otherwise.
 */
int readInputArgs(
        char inputString[MAX_CHARS_IN_INPUT],
        char *argv[EXPECTED_ARG_COUNT]) {
    char *str = fgets(inputString, MAX_CHARS_IN_INPUT, stdin);
    if (str == 0) {
        return FAILED_TO_READ_INPUT;
    }

    parseInputString(inputString, argv);

    return OK;
}

/**
 * This is the main method of the program. It receives an input from the user:
 * An original base, a number and a new base.
 * @param argc the number of arguments
 * @param argv the users arguments.
 * @return true if the program did not fail, false otherwise.
 */
int main() {
    char *inputArgs[EXPECTED_ARG_COUNT];
    char inputString[MAX_CHARS_IN_INPUT];

    int error = readInputArgs(inputString, inputArgs);
    if (error != OK) {
        printf("Failed to read input");
        return EXIT_FAILURE;
    }

    int newBase;
    int originalValue;

    error = parseArguments(inputArgs, &newBase, &originalValue);
    if (error != OK) {
        fprintf(stderr, "invalid!!\n");
        return EXIT_FAILURE;
    }

    char outputString[MAX_DIGITS_IN_NEW_NUMBER + 1];

    char *result = convertToStringByBase(originalValue,
                                         newBase,
                                         outputString);

    printf("%s\n", result);

    return EXIT_SUCCESS;
}
