/*-----------------------------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class name here>
 *-----------------------------------------------------------------------------
 * Exercise Number: #exercise_number#
 * File:			lottery.c
 * Author(s):		Peter Bauer
 * Due Date:		#due#
 *-----------------------------------------------------------------------------
 * Description:
 * Implementation of a library for analyzing lottery tips.
 *-----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lottery.h"

#define UUID_LEN 36
#define MAX_TIP_LEN 17
#define MAX_LINE_LEN (UUID_LEN + 1 + MAX_TIP_LEN +1)


FILE* fd;
char seperator;
int drawing[TIP_SIZE];
bool isDrawingSet = false;

int get_number_of_lines(FILE* fd);
bool contains_digit(int* numbers, int number);

bool init_lottery(const char* csv_file, char csv_seperator){
  fd = fopen(csv_file , "r");
  seperator = csv_seperator;
  isDrawingSet = false;
  return fd != nullptr;
}

bool get_tip(int tip_number, int tip[TIP_SIZE]){
  if (tip_number < 0 || tip_number >= get_number_of_lines(fd)) return false;

  fseek(fd, 0, SEEK_SET);

  int current = 0;
  while (current < tip_number) {
    if (!feof(fd) && fgetc(fd) == '\n') {
      current++;
    }
  }

  fseek(fd, UUID_LEN + 1, SEEK_CUR);

  char tipString[MAX_TIP_LEN];
  fgets(tipString, MAX_TIP_LEN, fd);

  char delimeter[2] = {seperator, '\0'};
  char* ptr = strtok(tipString, delimeter);

  int pos = 0;
  while (ptr != nullptr) {
    int number = atoi(ptr);
    tip[pos] = number;
    ptr = strtok(NULL, delimeter);
    pos++;
  }
  return pos == TIP_SIZE;
}

bool set_drawing(int drawing_numbers[TIP_SIZE]){
  for (int i = 0; i < TIP_SIZE; i++)
     	if (drawing_numbers[i] <= 0 || drawing_numbers[i] > 45)
         	return false;

 	for (int i = 0; i < TIP_SIZE; i++)
     	drawing[i] = drawing_numbers[i];

 	isDrawingSet = true;

 	return true;

}

int get_tip_result(int tip_number){
 if (tip_number < 0 || tip_number >= get_number_of_lines(fd)) {
   return -2;
 }

 if (!isDrawingSet) {
   return -1;
 }

 int correctDigits = 0;
 int csv_tips[TIP_SIZE];
 get_tip(tip_number, csv_tips);

 for (int i = 0; i < TIP_SIZE; i++) {
   if (contains_digit(drawing, csv_tips[i])) {
     correctDigits++;
   }
 }
 return correctDigits;
}

int get_right_tips_count(int right_digits_count){
 if (!isDrawingSet || right_digits_count < 0 || right_digits_count > TIP_SIZE) {
   return -1;
 }
 int lineCount = get_number_of_lines(fd);
 int count = 0;

 for (int i = 0; i < lineCount; i++) {
   int tip_result = get_tip_result(i);
   if (tip_result == right_digits_count) {
     count++;
   }
 }
 return count;
}

int get_number_of_lines(FILE* fd)
{
  int lines = 1;
  long previousPos = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  while (!feof(fd)) {
    if (fgetc(fd) == '\n') {
      lines++;
    }
  }
  fseek(fd, previousPos, SEEK_SET);
  return lines;
}

bool contains_digit(int* numbers, int number)
{
  for (int i = 0; i < TIP_SIZE; i++)
	{
    	if (numbers[i] == number)
        	return true;
	}
	return false;

}
