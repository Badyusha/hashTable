#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <vector>
#include <fstream>
#include <vector>
#include <sstream>

#define arraySize 401
#define fi (float)((sqrt(5) - 1)/2)



HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // Получаем дескриптор консоли

using namespace std;

int elemCount = 0, plusElemCount = 0, negativeElemCount = 0;

int hashSize = 0, i = 0;

int checkNumber(int val) {
	SetConsoleTextAttribute(hStdOut, 5);
	while (!(cin >> val)) {
		cin.clear();
		cin.ignore(1024, '\n');
		SetConsoleTextAttribute(hStdOut, 7);
		cout << "\nInvalid value!\n";
		cout << "Try again: ";
		SetConsoleTextAttribute(hStdOut, 5);
	}
	SetConsoleTextAttribute(hStdOut, 7);
	return val;
}

int hashConvert(int key) {
	return (int)((float)13 * ((float(key * fi))));
}

struct List {
	int key;
	bool used = false;
	List* next;
};

List* createListNode(List* head, int randIndex) {

	List* newP, * previous, * current, * next = NULL;
	newP = new List;
	if (newP != NULL) {		 /*есть ли место ?*/
		newP->key = randIndex;
		newP->next = new List;
		newP->next = NULL;
		previous = NULL;
		current = head;
		while (current != NULL)
		{
			previous = current;
			current = current->next;
			if (current != NULL)
				next = current->next;
		}
		if (previous == NULL)
		{
			newP->next = head;
			head = newP;
		}
		else
		{
			previous->next = newP;
			newP->next = current;

		}
	}
	else
		cout << "\nNo enough memory!\n";
	return head;
}

List* createHash() {
	
	cout << "\nHow much elements would you like to add?\t";
	elemCount = checkNumber(elemCount);
	while (elemCount >= arraySize) {
		cout << "\nToo much elements!\nTry less: ";
		elemCount = checkNumber(elemCount);
	}

	List* array = new List[arraySize]; // создаю массив списков под макс кол-во элем.
	List* arrayCopy = array;
	for (int i = 0; i < arraySize; ++i) {
		array[i].next = new List;
		array[i].next = NULL;
	}
	for (int i = 0; i < elemCount; ++i) {
		int randIndex = rand() % 101 - 50;
		randIndex = hashConvert(randIndex);
		if (array[abs(randIndex)].used) {
			array[abs(randIndex)].next = createListNode(array[abs(randIndex)].next, randIndex);
			array[abs(randIndex)].next->used = true;
		}
		else {
			array[abs(randIndex)].key = randIndex;
			array[abs(randIndex)].used = true;
			array[abs(randIndex)].next = new List;
			array[abs(randIndex)].next = NULL;
		}
	}
	return arrayCopy;
}

void outputHashCollision(List* head) {
	while (head != NULL) {
		cout << " --> " << head->key;
		head = head->next;
	}
	cout << " --> NULL";
}

bool chechHashSize() {
	return elemCount == 0;
}

void deleteHash(List* hash, List* plus, List* negative) {
	if (chechHashSize()) return;

	for (int i = 0; i < arraySize; ++i) {
		if (hash[i].next != NULL) {
			List* current = hash[i].next;
			List* next = NULL;
			while (current != NULL) {
				next = current->next;
				delete current;
				current = next;
			}
			hash[i].next = NULL;
		}
		if (plusElemCount && negativeElemCount) {
			if (plus[i].next != NULL) {
				List* current = plus[i].next;
				List* next = NULL;
				while (current != NULL) {
					next = current->next;
					delete current;
					current = next;
				}
				plus[i].next = NULL;
			}
			if (negative[i].next != NULL) {
				List* current = negative[i].next;
				List* next = NULL;
				while (current != NULL) {
					next = current->next;
					delete current;
					current = next;
				}
				negative[i].next = NULL;
			}
		}
	}
	hash = plus = negative = NULL;
	elemCount = 0;
	plusElemCount = 0;
	negativeElemCount = 0;
}

List* reorganizeIntoPlusHash(List* copy) {
	List* head = copy;
	List* plusHash = new List[arraySize];

	for (int i = 0; i < arraySize; ++i) {
		plusHash[i].next = new List;
		plusHash[i].next = NULL;
	}

	for (int i = 0; i < arraySize; ++i) {
		if (head[i].next != NULL) {
			int attempt = 0;
			List anCopy = head[i]; // самый гениальный мув!!! типа создаем объект-копию
			//текущего элемента списка,чтобы этот самый элемент не перезаписывался
			while (abs(anCopy.key) < arraySize) { // здесь такое условие потому что нельзя написать head[i] == NULL :(
				if (anCopy.key > 0 && anCopy.key < arraySize) {
					if (!attempt) {
						plusHash[i].key = anCopy.key;
						plusHash[i].used = true;
						plusHash[i].next = new List;
						plusHash[i].next = NULL;
						++attempt;
					}
					else {
						plusHash[i].next = createListNode(plusHash[i].next, anCopy.key);
					}
				}
				if (anCopy.next != NULL) {
					anCopy = *anCopy.next;
				}
				else {
					break;
				}
			}
		}
		else if (head[i].key > 0 && head[i].key < arraySize) {
			plusHash[i].key = head[i].key;
			plusHash[i].used = true;
			plusHash[i].next = new List;
			plusHash[i].next = NULL;
		}
	}
	++plusElemCount;
	return plusHash;
}

List* reorganizeIntoNegativeHash(List* copy) {
	List* head = copy;
	List* negativeHash = new List[arraySize];

	for (int i = 0; i < arraySize; ++i) {
		negativeHash[i].next = new List;
		negativeHash[i].next = NULL;
	}

	for (int i = 0; i < arraySize; ++i) {
		if (head[i].next != NULL) {
			int attempt = 0;
			List anCopy = head[i]; // самый гениальный мув!!! типа создаем объект-копию
			//текущего элемента списка,чтобы этот самый элемент не перезаписывался
			while (abs(anCopy.key) < arraySize) { // здесь такое условие потому что нельзя написать head[i] == NULL :(
				if (anCopy.key < 0 && anCopy.key > -arraySize) {
					if (!attempt) {
						negativeHash[i].key = anCopy.key;
						negativeHash[i].used = true;
						negativeHash[i].next = new List;
						negativeHash[i].next = NULL;
						++attempt;
					}
					else {
						negativeHash[i].next = createListNode(negativeHash[i].next, anCopy.key);
					}
				}
				if (anCopy.next != NULL) {
					anCopy = *anCopy.next;
				}
				else {
					break;
				}
			}
		}
		else if (head[i].key < 0 && head[i].key > -arraySize) {
			negativeHash[i].key = head[i].key;
			negativeHash[i].used = true;
			negativeHash[i].next = new List;
			negativeHash[i].next = NULL;
		}
	}
	++negativeElemCount;
	return negativeHash;
}

void outputPositiveNegativeHash(List* a, List* b, List* c) {
	List* plusHash = a;
	List* negativeHash = b;
	List* arrayHash = c;

	SetConsoleTextAttribute(hStdOut, 6);
	cout << "\n\nMAIN hash:\n" << endl;
	SetConsoleTextAttribute(hStdOut, 7);

	for (int i = 0; i < abs(arraySize); ++i) {
		if (arrayHash[i].used) {
			cout << arrayHash[i].key;
			if (arrayHash[i].next != NULL) {
				outputHashCollision(arrayHash[i].next);
			}
			cout << endl;
		}
	}

	SetConsoleTextAttribute(hStdOut, 6);
	cout << "\nPositive hash:\n" << endl;
	SetConsoleTextAttribute(hStdOut, 7);

	for (int i = 0; i < abs(arraySize); ++i) {
		if (plusHash[i].used) {
			cout << plusHash[i].key;
			if (plusHash[i].next != NULL) {
				outputHashCollision(plusHash[i].next);
			}
			cout << endl;
		}
	}

	SetConsoleTextAttribute(hStdOut, 6);
	cout << "\n\nNegative hash:\n" << endl;
	SetConsoleTextAttribute(hStdOut, 7);

	for (int i = 0; i < abs(arraySize); ++i) {
		if (negativeHash[i].used) {
			cout << negativeHash[i].key;
			if (negativeHash[i].next != NULL) {
				outputHashCollision(negativeHash[i].next);
			}
			cout << endl;
		}
	}
}

void main() {

	system("chcp 1251");
	srand(time(NULL));
	system("cls");
	SetConsoleTextAttribute(hStdOut, 8);
	cout << "~ Chapter 15 ~" << endl;
	SetConsoleTextAttribute(hStdOut, 7);


	int* plusArray = NULL, * minusArray = NULL;
	List* arrayHash = NULL;
	List* plusHash = new List;
	List* negativeHash = new List;
	List* copyArrayHash = NULL;

	while (1){

		cout << "\nChoose menu item:\n"
		<< "1 - Create hash\n"
		<< "2 - Reorganize hash into +/- hash\n"
		<< "3 - Delete all hashes\n"
		<< "4 - Output main hash\n"
		<< "5 - Output +/- hashes\n"
		<< "0 - Exit\n" << endl;

		int menuChoice{};
		menuChoice = checkNumber(menuChoice);

		switch (menuChoice) {
		case 1: {
			deleteHash(arrayHash, plusHash, negativeHash);
			arrayHash = new List;
			arrayHash = createHash();
			cout << "\nCreated!\n";
			copyArrayHash = arrayHash;
			break;
		}
		case 2: {
			if (chechHashSize()) {
				cout << "\nThere is no main hash!!!\n";
				break;
			}

			plusHash = reorganizeIntoPlusHash(arrayHash);

			negativeHash = reorganizeIntoNegativeHash(arrayHash);


			cout << "\nReorganized\n";
			break;
		}
		case 3: 
			if (chechHashSize()) {
				cout << "\nThere is no main hash!!!\n";
				break;
			}
			deleteHash(arrayHash, plusHash, negativeHash);

			cout << "\nDeleted\n";
			break;
		case 4:
			if (chechHashSize()) {
				cout << "\nThere is no main hash!!!\n";
				break;
			}
			for (int i = 0; i < abs(arraySize); ++i) {
				if (arrayHash[i].used) {
					cout << arrayHash[i].key;
					if (arrayHash[i].next != NULL) {
						outputHashCollision(arrayHash[i].next);
					}
					cout << endl;
				}
			}
			system("pause");
			system("cls");
			break;
		case 5: {
			if (chechHashSize() || !plusElemCount || !negativeElemCount) {
				cout << "\nThere is no main hash!!!\n";
				break;
			}

			outputPositiveNegativeHash(plusHash, negativeHash, arrayHash);

			system("pause");
			system("cls");
			break;
		}
		case 0:
			deleteHash(arrayHash, plusHash, negativeHash);
			exit(0);
		default: cout << "\nInvalid value!\n" << endl;
			break;
		}

	}
}

