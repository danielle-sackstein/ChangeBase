#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* internal errors */

#define OK                              0
#define NOT_A_NUMBER                    1
#define FAILED_TO_READ_INPUT            2


#define EXPECTED_ARG_COUNT              3
#define MAX_DIGITS_IN_ORIGINAL_NUMBER   6
#define MAX_DIGITS_IN_BASE              2
#define MAX_CHARS_IN_INPUT            \
            MAX_DIGITS_IN_BASE + 1 + \
            MAX_DIGITS_IN_BASE + 1 + \
            MAX_DIGITS_IN_ORIGINAL_NUMBER + 1

struct Input
{
	int newBase;
	int originalValue;
};

struct Output
{
	int startIndex;
	long double result[MAX_DIGITS_IN_ORIGINAL_NUMBER];
};

struct Input gInput;
struct Output gOutput;

int parseInteger(char *str, int base, int *pResult)
{
	*pResult = 0;

	size_t length = strlen (str);
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

void getDigitsInNewBase(int value, int base, long double *result, int *pStartIndex)
{
	int startIndex = MAX_DIGITS_IN_ORIGINAL_NUMBER;

	for (int ratio = value; ratio > 0; startIndex--) {
		result[startIndex - 1] = ratio % base;
		ratio /= base;
	}

	*pStartIndex = startIndex;
}

int parseArguments(char **argv)
{
	int originalBase = 10;

	int error = parseInteger(argv[0], 10, &originalBase);
	if (error != OK) {
		return error;
	}

	error = parseInteger(argv[1], 10, &gInput.newBase);
	if (error != OK) {
		return error;
	}

	return parseInteger(argv[2], originalBase, &gInput.originalValue);
}

/**
 * Parses the input string. Separates the numbers by " " by replacing it with Null.
 * @param inputString  pointer to char
 * @param argv
 */
void parseInputString(char *inputString, char *argv[EXPECTED_ARG_COUNT])
{
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
int readInputArgs(char **argv)
{
	char inputString[MAX_CHARS_IN_INPUT];

	char *str = fgets(inputString, MAX_CHARS_IN_INPUT, stdin);
	if (str == 0) {
		return FAILED_TO_READ_INPUT;
	}

	parseInputString(inputString, argv);

	return OK;
}

void printOutput(int startIndex, long double *result)
{
	for (int i = startIndex; i < 6; i++) {
		printf("%llu", (unsigned long long)result[i]);
	}
	printf("\n");
}

/**
 * This is the main method of the program. It receives an input from the user:
 * An original base, a number and a new base.
 * @param argc the number of arguments
 * @param argv the users arguments.
 * @return true if the program did not fail, false otherwise.
 */
int main(int argc, char *argv[])
{
	char *inputArgs[EXPECTED_ARG_COUNT];

	int error = readInputArgs(inputArgs);
	if (error != OK) {
		printf("Failed to read input");
		return EXIT_FAILURE;
	}

	error = parseArguments(inputArgs);
	if (error != OK) {
		fprintf(stderr, "invalid!!\n");
		return EXIT_FAILURE;
	}

	getDigitsInNewBase(gInput.originalValue,
					   gInput.newBase,
					   gOutput.result,
					   &gOutput.startIndex);

	printOutput(gOutput.startIndex, gOutput.result);

	return EXIT_SUCCESS;
}
