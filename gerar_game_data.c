#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define BOARD_SIZE 25
#define BOMB_COUNT 5


static int contains(int values[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (values[i] == value) {
            return 1;
        }
    }
    return 0;
}


static void sort_bombs(int bombs[]) {
    for (int i = 0; i < BOMB_COUNT - 1; i++) {
        for (int j = i + 1; j < BOMB_COUNT; j++) {
            if (bombs[j] < bombs[i]) {
                int temp = bombs[i];
                bombs[i] = bombs[j];
                bombs[j] = temp;
            }
        }
    }
}


static void generate_bombs(int bombs[]) {
    int count = 0;


    while (count < BOMB_COUNT) {
        int position = rand() % BOARD_SIZE;


        if (!contains(bombs, count, position)) {
            bombs[count] = position;
            count++;
        }
    }


    sort_bombs(bombs);
}
