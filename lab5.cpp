#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int exit_flag = 0;

void interrupt(*old9)(...);
void interrupt new9(...);

void indicator(int mask);

void main() {
	old9 = getvect(0x09); // ��������� ��������� �� ������ ����������
	setvect(0x09, new9); // ������ ��� �� �����
	while (!exit_flag) {
		indicator(4); // ���. ��������� Caps Lock
		delay(600);
		indicator(0); // ����. ��� ����������
		delay(600);
		system("cls");
		printf("Press ESC to exit program");
		printf("\nPress any key: ");

	}
	indicator(0); // ����. ��� ����������
	setvect(0x09, old9); // ���������� ������ ����������
}

void interrupt new9(...) {
	char buf[5];
	unsigned char c = inp(0x60); // ��������� ����-��� �� ����� ����������
	if (c == 0x01) exit_flag = 1; // ���� ��� ESC ������������� ���� ���������� ���������
	if (c != 0xFA && !exit_flag) { // ����� ������� ����-��� � ����������������� �����
		itoa(c, buf, 16);
		cputs("0x");
		cputs(buf);
		cputs(" ");
	}
	(*old9)();
}

void indicator(int mask) {
	// ����� ������������ ����� ���������� ��� 0xED - ��� ������� ���������� ������������ ����������
	if (mask != 0xED) indicator(0xED);
	int i = 0;
	// ���� �������������, ��� ���������� ������� ������ ���������� ���������� ����� (��������� ����� ���������)
	while ((inp(0x64) & 2) != 0); // 1 ��� ������ ���� 0
// ���������� ����� � ���� ����������
	do {
		i++;
		outp(0x60, mask);
	} while (inp(0x60) == 0xFE && i < 3); // ��������� ��� �������� 0xFA - �������, 0xFE - ���������� ��������� ��������
	if (i == 3) {
		cputs("\nError: 0xFE - can't send mask\n");
		exit_flag = 1; // ��������� ����� ���������� ���������
	}
}