#pragma once
#include <iostream>
#include <stdlib.h>
#include <deque>
using std::deque;

class MemManager_DL
{
	struct MemChain
	{					// structure for memory block information
		void * addr;					// adress of data block
		unsigned size;
		char free;						// ����� - 0, �������� - 1
//		MemChain *next;
	};
	unsigned HeadSize;
	
	deque<MemChain*> AllocatedList;		// List of allocated blockes
	deque<MemChain*> FreeList;			// list of free blockes
	char * Mem ;						// "heap" - placement of all blockes
	unsigned totalSize = 0;				// ?
public:
	MemManager_DL(unsigned _size);
	~MemManager_DL();
	void * alloc(unsigned size);
	void free(void * ptr);
	void Print(const char * str);
};

// 
MemManager_DL::MemManager_DL(unsigned _size)
{
	HeadSize = sizeof MemChain;
	Mem = (char*)malloc(_size);
	if (Mem == NULL) throw "error allocating mem";
	totalSize = _size;
	MemChain *ch = (MemChain *)Mem;
	ch->addr = Mem + HeadSize;
	ch->free = 1;
	ch->size = totalSize - HeadSize;
	FreeList.push_back(ch);
}

MemManager_DL::~MemManager_DL()
{
	if (Mem != NULL) free(Mem);
	FreeList.clear();
	AllocatedList.clear();
}


void * MemManager_DL::alloc(unsigned size)
{
	unsigned req_size = size + HeadSize;
	auto ch = FreeList.begin();

	for (; ch !=FreeList.end(); ch++)
	{
		if ((*ch)->size >= req_size) break;
	}
	if (ch != FreeList.end())
	{	// ������� �� ������� ����� ������ ����� � ������� � ������ ������ �� ���������� ��������� ����� 
		MemChain * ch1;					// new block - describe �������
		unsigned shift = req_size;		// ����� ������ ����� relative to prev
		ch1 = (MemChain *)((char*)(*ch) + req_size);
		ch1->addr = (char*)(*ch)->addr + req_size;
		ch1->size = (*ch)->size - req_size;
		(*ch)->size = size;
		AllocatedList.push_back(*ch);
		*ch = ch1;
		return ch1 + HeadSize;
	}
	return NULL;
}

void  MemManager_DL::free(void * ptr)
{
	if (ptr<Mem && ptr>Mem + totalSize) throw "wrong adr";
	auto ch = AllocatedList.begin();
	for (; ch != AllocatedList.end(); ch ++) if ((*ch) + HeadSize == ptr) {
//		(*ch)->size = ((ch + 1) - ch)- HeadSize;
		FreeList.push_back(*ch);
		AllocatedList.erase(ch);
		break;
	}

}
unsigned maxSize() { return 1; };

void GarbCol()
{
	/*
	// ������� ������
	auto ch = List.begin(), pr = ch;
	for (int c = 0; ch != List.end(); ch++)
	{
		if ((*ch)->free == 1)
		{
			c++;
			if (c == 2)	// ���������� ��� ���� ���������
			{
				(*pr)->size += (*ch)->size;
				List.erase(ch);
				c = 1;
				ch = pr;
				continue;
			}
		}
		else c = 0;
		pr = ch;
	}*/
}

void MemManager_DL::Print(const char * str)
{
	std::cout << str << "\n";

	for (auto ch = FreeList.begin(); ch != FreeList.end(); ch++)
		std::cout	<< *ch << "    "
					<< (*ch)->addr << "    "
					<< (*ch)->size << "\n";

}