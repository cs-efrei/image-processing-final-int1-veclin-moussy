#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);
int cap(int number1, int number2, int ceiling);
void print_menu();
int clamp(int value);
#endif // UTILS_H
