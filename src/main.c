#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MAX_OBJECTS 4
// *50ms
#define FALL_INTERVAL_TICKS 5

int lifes = 3;
int points = 0;
int personX = 9;
int personY = 21;

typedef struct {
    int x;
    int y;
    char type;  
} Falling;

Falling objects[MAX_OBJECTS];
int dropCounter = 0;

void printKey(int ch) {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 22);
    printf("Key code :");
    screenGotoxy(34, 23);
    printf("            ");
    screenGotoxy((ch == 27 ? 36 : 39), 23);
    printf("%d ", ch);
    while (keyhit()) {
        printf("%d ", readch());
    }
}

void printHead(void) {
    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(2, 5);
    printf("-----------------------------------------------------------------------------");
    screenGotoxy(4, 2);
    printf("Pontos: %d ", points);
    screenGotoxy(4, 3);
    printf("Vidas: %d ", lifes);
}

void printPerson(void) {
    screenSetColor(GREEN, DARKGRAY);
    screenGotoxy(personX + 1, personY);
    printf("O");
    screenGotoxy(personX + 1, personY + 1);
    printf("I");
    screenGotoxy(personX + 1, personY + 2);
    printf("A");
    screenGotoxy(personX, personY + 1);
    printf("/");
    screenGotoxy(personX + 2, personY + 1);
    printf("\\");
}

void cleanPerson(void) {
    screenGotoxy(personX + 1, personY);
    printf(" ");
    screenGotoxy(personX + 1, personY + 1);
    printf(" ");
    screenGotoxy(personX + 1, personY + 2);
    printf(" ");
    screenGotoxy(personX, personY + 1);
    printf(" ");
    screenGotoxy(personX + 2, personY + 1);
    printf(" ");
}

void initObjects(void) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        objects[i].y = MINY + 2;
        objects[i].x = rand() % (MAXX - MINX - 1) + MINX + 1;
        objects[i].type = (rand() % 2 == 0 ? 'W' : 'X');
    }
}

int main(void) {
    int ch = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    srand((unsigned)time(NULL));
    initObjects();

    printHead();
    printPerson();

   
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (objects[i].type == 'W') screenSetColor(GREEN, DARKGRAY);
        else screenSetColor(RED, DARKGRAY);
        screenGotoxy(objects[i].x, objects[i].y);
        printf("%c", objects[i].type);
    }
    screenUpdate();

    while (ch != 10 && lifes > 0) {
       
        if (keyhit()) {
            ch = readch();
            // printKey(ch);
            cleanPerson();
            if (ch == 97) personX--;
            else if (ch == 100) personX++;
            printPerson();
            printHead();
            screenUpdate();
        }

     
        if (timerTimeOver()) {
            dropCounter++;
            if (dropCounter >= FALL_INTERVAL_TICKS) {
                dropCounter = 0;
                for (int i = 0; i < MAX_OBJECTS; i++) {
                    screenGotoxy(objects[i].x, objects[i].y);
                    printf(" ");

                    objects[i].y++;

                   
                    if (objects[i].y == personY + 1 &&
                        objects[i].x >= personX && objects[i].x <= personX + 2) {
                        if (objects[i].type == 'W') points++;
                        else lifes--;
                        printHead();
                        
                        objects[i].y = MINY + 2;
                        objects[i].x = rand() % (MAXX - MINX - 1) + MINX + 1;
                        objects[i].type = (rand() % 2 == 0 ? 'W' : 'X');
                    }
                   
                    else if (objects[i].y >= MAXY - 1) {
                        objects[i].y = MINY + 2;
                        objects[i].x = rand() % (MAXX - MINX - 1) + MINX + 1;
                        objects[i].type = (rand() % 2 == 0 ? 'W' : 'X');
                    }

                    // Draw new
                    if (objects[i].type == 'W') screenSetColor(GREEN, DARKGRAY);
                    else screenSetColor(RED, DARKGRAY);
                    screenGotoxy(objects[i].x, objects[i].y);
                    printf("%c", objects[i].type);
                }
                screenUpdate();
            }
        }
    }


    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
