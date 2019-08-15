#define A5_H
char get_bit(char *array, int index);

char get_bit(char *array, int index) {
    return 1 & (array[index / 8] >> (index % 8));
}
void toggle_bit(char *array, int index);

void toggle_bit(char *array, int index) {
    array[index / 8] ^= 1 << (index % 8);
}
