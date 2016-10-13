#include "8051.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8;
typedef unsigned int uint16;


void delay(uint16 i);

void Init();

void KeyPros();
void Fresh();

void onClick(const char c);
void Display(long num);
char* GetNum();

long opera(long a, long b, const char op);


#define GPIO_KEY P1
#define GPIO_DIG P0

#define LSA P2_2
#define LSB P2_3
#define LSC P2_4

__code char KEY_MAT[17] = {
	'7', '8', '9', '+',
	'4', '5', '6', '-',
	'1', '2', '3', '*',
	'C', '0', '=', '/',
};

__code uint8 smgduan[17] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
	0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71
};

static char dig[8];
static char Num[9];

static long a;
static long b;
static long *p;
static char op;

void main()
{
	Init();
	for (;;) {
		Fresh();
		KeyPros();
	}
}

void Init()
{
	p = &a;
	memset(Num, 0, 9);
	memset(dig, 0, 8);
	Display(0);
}

void KeyPros()
{
	uint8 a = 0;
	uint8 keyValue = 1;

	GPIO_KEY = 0x0f;
	if (GPIO_KEY != 0x0f) {
		delay(1000);
		if (GPIO_KEY != 0x0f) {

			GPIO_KEY = 0x0f;
			switch (GPIO_KEY) {
				case 0x07:
					keyValue = 0;
					break;
				case 0x0b:
					keyValue = 1;
					break;
				case 0x0d:
					keyValue = 2;
					break;
				case 0x0e:
					keyValue = 3;
					break;
				default:
					break;
			}

			GPIO_KEY = 0xf0;
			switch (GPIO_KEY) {
				case 0x70:
					keyValue += 0;
					break;
				case 0xb0:
					keyValue += 4;
					break;
				case 0xd0:
					keyValue += 8;
					break;
				case 0xe0:
					keyValue += 12;
					break;
				default:
					break;
			}

			while ((a < 50) && (GPIO_KEY != 0xf0)) {
				delay(1000);
				a++;
			}

			onClick(KEY_MAT[keyValue]);
		}
	}
}

void Fresh()
{
	uint8 i;
	for (i = 0; i < 8; i++) {
		switch (i) {
			case 0:
				LSC = 0; LSB = 0; LSA = 0;
				break;
			case 1:
				LSC = 0; LSB = 0; LSA = 1;
				break;
			case 2:
				LSC = 0; LSB = 1; LSA = 0;
				break;
			case 3:
				LSC = 0; LSB = 1; LSA = 1;
				break;
			case 4:
				LSC = 1; LSB = 0; LSA = 0;
				break;
			case 5:
				LSC = 1; LSB = 0; LSA = 1;
				break;
			case 6:
				LSC = 1; LSB = 1; LSA = 0;
				break;
			case 7:
				LSC = 1; LSB = 1; LSA = 1;
				break;
			default:
				break;
		}

		switch (dig[i]) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				GPIO_DIG = smgduan[dig[i] - 48];
				break;
			case '-':
				GPIO_DIG = 0x40; 	//0100 000 
				break;
			default:
				GPIO_DIG = 0x00;
				break;
		}
		delay(100);
		P0 = 0x00;
	}
}

void Display(long num)
{
	uint8 i;
	uint8 len;

	memset(Num, 0, 9);
	sprintf(Num, "%ld", num);
	for (i = 0; i < 8; i++) {
		dig[i] = -1;
	}
	len = strlen(Num);
	for (i = 0; i < len; i++) {
		dig[i] = Num[len - i - 1];
	}
}

char* GetNum()
{
	return Num;
}

void onClick(const char c)
{
	char *num = GetNum();
	uint8 len = strlen(num);

	switch (c) {
		case '0':
			if (len == 0) {
				break;
			}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			num[len] = c;
			Display(atol(num));
			break;
		case '+':
		case '-':
		case '*':
		case '/':
			*p = atol(num);
			p = &b;
			op = c;
			memset(num, 0, 9);
			break;
		case '=':
			*p = atol(num);
			a = opera(a, b, op);
			Display(a);
			break;
		case 'C':
			Init();
			break;
		default:
			break;
	}
}

long opera(long a, long b, const char op)
{
	long result;
	switch (op) {
		case '+':
			result = a + b;
			break;
		case '-':
			result = a - b;
			break;
		case '*':
			result = a * b;
			break;
		case '/':
			result = a / b;
			break;
		default:
			result = -1;
			break;
	}
	return result;
}

void delay(uint16 i)
{
	while(i--);
}
