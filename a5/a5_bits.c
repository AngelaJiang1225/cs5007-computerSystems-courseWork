#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE (8) /* amount of bits */
#define ARRAY_SIZE(x) (x/8+(!!(x%8)))

void toggle_bit(char *array, int index);

void toggle_bit(char *array, int index) {
    array[index / 8] ^= 1 << (index % 8);
}
void displayBits(unsigned value) {
  unsigned c, displayMask = 1 << 15;
  printf("%3c%7u = ", value, value);
  for (c = 1; c <= 16; c++) {
  putchar(value & displayMask ? '1' : '0');
  value <<= 1;
  if (c % 8 == 0) {
    putchar(' ');
  }
  }
  putchar('\n');
}
// holding the packed characters
void packCharacters(char a, char b) {
  printf("Original chars:\n");
  displayBits(a);
  displayBits(b);
  unsigned c, displayMask = 1 << 15;
  int ca, cb;
  printf("After packing:\n");
  unsigned a2 = (unsigned)a << 8;
  for (c = 1; c <= 16; c++) {
    ca = ((a2 << c-1) & displayMask ? 1 : 0);
    cb = ((b << c-1) & displayMask ? 1 : 0);
    putchar(ca || cb ? '1' : '0');
    if (c % 8 == 0) {
      putchar(' ');
  }
  }
}
// unpackCharacters
void unpackCharacters(unsigned int packedInt) {
  unsigned displayMask1 = 1 << 15;
  unsigned displayMask2 = (unsigned)65280;
  printf("\nThe first unpacked char is:\n");
  printf("00000000 ");
  for (int c = 0;c <=7; c++) {
putchar((packedInt << c & displayMask1)
  && (displayMask2 << c & displayMask1) ? '1' : '0');
}
  printf("\nThe second unpacked char is:\n");
  displayMask2 >>= 8;
  for (int c = 1; c <= 16; c++) {
    putchar((packedInt & displayMask2) << c-1
  & displayMask1 ? '1' : '0');
if (c % 8 == 0) {
    putchar(' ');
  }
  }
  printf("\n");
}
//  calculate the result
int power2(int num, int pow) {
  for (int i = 0; i < pow; i++) {
    num = num * 2;
  }
  displayBits(num);
}
// read file
void readFile() {
  FILE *cfPtr;
  char *line, *tempRes;
  char buffer[1024];
  char delim[] = ",";
  if ((cfPtr = fopen("miles_graph.csv", "r")) == NULL) {
    printf("File could not be opened\n");
  } else {
    fseek(cfPtr, 0, SEEK_SET);
    char *result = NULL;
    while ((line = fgets(buffer, sizeof(buffer), cfPtr)) != NULL) {
  tempRes = strtok(line, delim);
      while (tempRes != NULL) {  // read every line
        printf("%s\n", tempRes);
        tempRes = strtok(NULL, delim);
}
  printf("\n");
  }
  fclose(cfPtr);
  }
}
int main(void) {
    /* initialize empty array with the right size */
    char x[ARRAY_SIZE(SIZE)] = { 0 };
    int i;
    toggle_bit(x, 3);
    toggle_bit(x, 4);
    toggle_bit(x, 6);
    toggle_bit(x, 7);
    char* out = "00011011";
    for (i = 0; i < 8; i++) {
      printf("%d\n", (out[i] >> i));
    }
    printf("\n\n");
    displayBits('B');
    displayBits('i');
    displayBits('k');
    packCharacters('B', 'i');
    unpackCharacters(17001);
    power2(3, 2);
    return 0;
}
