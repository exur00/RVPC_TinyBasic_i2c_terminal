/**
 * This file is part of the RVPC project 
 * https://github.com/OLIMEX/RVPC
 * https://www.olimex.com/Products/Retro-Computers/RVPC/open-source-hardware
 * 
 * Copyright (c) 2024 Olimex Ltd
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "print.h"
#include "i2c_own.h"

#define I2C_ADDR 0x37 // should be imported from gpu/terminal settings
#define VGA_NUM_COLS 80 // should be imported from gpu/terminal settings

uint8_t send_buffer[10];

void cursor_down() {
	print("\n");
}

void cursor_back() {
	send_buffer[0] = 0x1;
	send_buffer[1] = 0x2; // get from header instead
	I2C_SendBytes(I2C_ADDR, send_buffer, 2);
}

void write(char c) {
	I2C_SendByte(I2C_ADDR, c);
}

void print(const char *str) {
	I2C_SendCommandAndBytes(I2C_ADDR, 0x0, str, strlen(str));
}

void println(const char *str) {
	print(str);
	cursor_down();
}

void printfstr(const char* format, ...) {
	va_list argptr;
	char tmp[VGA_NUM_COLS + 2];

	va_start(argptr, format);
	vsnprintf(tmp, sizeof(tmp), format, argptr);
	va_end(argptr);

	print(tmp);
}

void printfln(const char* format, ...) {
	va_list argptr;
	char tmp[VGA_NUM_COLS + 2];

	va_start(argptr, format);
	vsnprintf(tmp, sizeof(tmp), format, argptr);
	va_end(argptr);

	println(tmp);
}

void cls() {
	send_buffer[0] = 0x1; // replace with control command macro
	send_buffer[1] = 0x0; // replace with CLS macro
	I2C_SendBytes(I2C_ADDR, send_buffer, 2);
}

void set_color(uint8_t color) {
	send_buffer[0] = 0x2;
	send_buffer[1] = color;
	I2C_SendBytes(I2C_ADDR, send_buffer, 2);
}
