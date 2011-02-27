/* string_extras.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * \file	string_extras.c
 * \author  Daniel Otte 
 * \date    2006-05-16
 * \license	GPLv3 or later
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

uint32_t stridentcnt(const char* a, const char* b){
	uint16_t i=0;
	for(;;){
		if(*a != *b || *b=='\0')
			return i;
		i++;
		a++;
		b++;
	}
}

uint16_t firstword_length(const char* s){
	uint16_t ret=0;
	while(isgraph((uint8_t)(*s++)))
		ret++;
	return ret; 
}

char* strstrip(char* str){
	if(!str)
		return str;
	char* endptr;
	while(*str && (*str==' ' || *str=='\t'))
		++str;
	endptr=str;
	while(*endptr)
		++endptr;
	do{
		--endptr;
	}while(*endptr==' ' || *endptr=='\t');
	endptr[1]='\0';
	return str;
}

void str_reverse(char* buffer){
	char *i, *j;
	char c;
	i=buffer;
	j=buffer + strlen(buffer)-1;
	while(i<j){
		c = *i;
		*i = *j;
		*j = c;
		++i;
		--j;
	}
}
char* ultoa (unsigned long value, char* buffer, uint8_t radix ){
	if((radix>36) || (radix<2) || (buffer==NULL)){
		return NULL;
	}
	unsigned length=(unsigned)-1;
	uint8_t x;
	char c;
	do{
		x = value%radix;
		value /= radix;
		if(x<10){
			c = x+ '0';
		}else{
			c = x+ 'a';
		}
		buffer[++length] = c;
	}while(value);
	buffer[length+1]='\0';
	unsigned idx=0;
	while(idx+1<length){
		c = buffer[idx];
		buffer[idx++] = buffer[length];
		buffer[length--] = c;
	}
	return buffer;
}

char* ulltoa(unsigned long long value, char* buffer, uint8_t radix){
	if((radix>36) || (radix<2) || (buffer==NULL)){
		return NULL;
	}
	unsigned length=(unsigned)-1;
	uint8_t x;
	char c;
	do{
		x = value%radix;
		value /= radix;
		if(x<10){
			c = x+ '0';
		}else{
			c = x+ 'a';
		}
		buffer[++length] = c;
	}while(value);
	buffer[length+1]='\0';
	unsigned idx=0;
	while(idx+1<length){
		c = buffer[idx];
		buffer[idx++] = buffer[length];
		buffer[length--] = c;
	}
	return buffer;
}

char* ustoa(unsigned short value, char* buffer, uint8_t radix){
	if((radix>36) || (radix<2) || (buffer==NULL)){
		return NULL;
	}
	unsigned length=(unsigned)-1;
	uint8_t x;
	char c;
	do{
		x = value%radix;
		value /= radix;
		if(x<10){
			c = x+ '0';
		}else{
			c = x+ 'a';
		}
		buffer[++length] = c;
	}while(value);
	buffer[length+1]='\0';
	unsigned idx=0;
	while(idx+1<length){
		c = buffer[idx];
		buffer[idx++] = buffer[length];
		buffer[length--] = c;
	}
	return buffer;
}
/*
void strlwr(char* s){
	while(*s){
		*s = tolower(*s);
		s++;
	}
}
*/
