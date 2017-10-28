#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OK                  0
#define NOT_A_NUMBER        1

struct Input
{
	int newBase;
	int originalValue;
};

struct Output
{
	int startIndex;
	int result[6];
};

struct Input gInput;
struct Output gOutput;

int parseInteger(char *str, int length, int base, int *pResult)
{
	/*we are assuming that the length of the number must be positive*/

	*pResult = 0;

	int power = 1;

	char *ptr = str + length - 1;

	for (int i = 0; i < length; i++) {
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

int changeBase(int value, int base, int result[6], int *pStartIndex)
{
	for (int i = 0; i < 6; i++) {
		result[i] = 0;
	}
	*pStartIndex = 5;

	int smallestPowerAbove = 1;
	int power = 0;

	for (power = 0; power < 6; power++) {
		if (smallestPowerAbove > value) {
			break;
		}
		smallestPowerAbove *= base;
	}

	if (power == 7) {
		return -1;
	}

	if (power == 0) {
		return 0;
	}

	power--;

	int smallestPowerBelow = smallestPowerAbove / base;
	int remain = value;
	int place = 5 - power;
	*pStartIndex = place;

	while (place < 6) {
		int digit = remain / smallestPowerBelow;
		result[place] = digit;

		remain = remain % smallestPowerBelow;
		smallestPowerBelow /= base;
		place++;
	}

	return OK;
}

int getIntegerArgument(char *str, int base, int *pResult)
{
	*pResult = 0;

	int length = strlen(str);

	int error = parseInteger(str, length, base, pResult);

	/*we are assuming that the length of the number must be positive*/
	if (error == NOT_A_NUMBER) {
		fprintf(stderr, "invalid!!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int readInput(int argc, char *argv[])
{
	int originalBase = 0;

	int error = getIntegerArgument(argv[1], 10, &originalBase);
	if (error != 0) {
		return error;
	}

	error = getIntegerArgument(argv[2], 10, &gInput.newBase);
	if (error != 0) {
		return error;
	}

	error = getIntegerArgument(argv[3], originalBase, &gInput.originalValue);
	if (error != 0) {
		return error;
	}

	return 0;
}

void printOutput(int startIndex, int *result)
{
	for (int i = startIndex; i < 6; i++) {
		printf("%d", result[i]);
	}

	printf("\n");
}

int Test()
{
	gInput.newBase = 2;
	gInput.originalValue = 63;

	struct Output expectedOutput;

	expectedOutput.startIndex = 0;
	expectedOutput.result[0] = 1;
	expectedOutput.result[1] = 1;
	expectedOutput.result[2] = 1;
	expectedOutput.result[3] = 1;
	expectedOutput.result[4] = 1;
	expectedOutput.result[5] = 1;

	int error = changeBase(
			gInput.originalValue,
			gInput.newBase,
			gOutput.result,
			&gOutput.startIndex);

	if (error != 0) {
		return -1;
	}

	if (expectedOutput.startIndex != gOutput.startIndex) {
		return -1;
	}

	for (int i = 0; i < 6; i++) {
		if (expectedOutput.result[i] != gOutput.result[i]) {
			return -1;
		}
	}

	return 0;
}

int Run(int argc, char *argv[])
{
	int error = readInput(argc, argv);
	if (error != 0) {
		return error;
	}

	error = changeBase(
			gInput.originalValue,
			gInput.newBase,
			gOutput.result,
			&gOutput.startIndex);

	if (error != 0) {
		return error;
	}

	printOutput(gOutput.startIndex, gOutput.result);

	return 0;
}

int main(int argc, char *argv[])
{
//	if (Test() == 0) {
//		printf("PASSED\n");
//	} else {
//		printf("FAILED\n");
//	}
//	return 0;

	return Run(argc, argv);
}
