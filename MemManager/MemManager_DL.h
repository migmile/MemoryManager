#pragma once
#include <iostream>
#include <stdlib.h>
#include <deque>
using std::deque;

class MemManager_DL
{
	struct MemChain
	{						// structure for memory block information
		void * addr;		// adress 
		unsigned size;
		char free;			// занят - 0, свободен - 1
//		MemChain *next;
	};
	unsigned HeadSize;
	
	deque<MemChain*> AllocatedList;		// List of allocated blockes
	deque<MemChain*> FreeList;			// list of free blockes
	char * Mem ;						// "heap" - placement of all blockes
	unsigned totalSize = 0;				// ?
/*	MemChain * FindFree(MemChain *ch)	
	{ // find first free
		for (; ch != NULL; ch = ch->next)
		{
			if (ch->free == 1) return ch;
		}
	}*/
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
//	ch->addr = 0;
	ch->free = 1;
	ch->size = totalSize - HeadSize;
	//List->prev=NULL;
//	ee->next = NULL;
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
//	if (NULL == ch) ch = List;
	for (; ch !=FreeList.end(); ch++)
	{
		if ((*ch)->size >= req_size) break;
	}
	if (ch != FreeList.end())
	{	// выделим из данного блока нужный кусок и вставим в список ссылку на оставшуюся свободную часть 
		MemChain * ch1;		// new block - describe остаток
		unsigned shift = req_size; // сдвиг нового блока relative to prev
		ch1 = *ch + shift;
		//ch1->next = ch->next;	// lists
		//ch->next = ch1;
		//ch1->shift = shift;
		//ch1->free = 1;
		ch1->size = (*ch)->size - req_size;
		(*ch)->size = size;
		//ch->free = 0;
		*ch=ch1;
		AllocatedList.push_back(*ch);
		return ch1 + HeadSize;
	}
	return NULL;
}

void  MemManager_DL::free(void * ptr)
{
	if (ptr<Mem && ptr>Mem + totalSize) throw "wrong adr";
	auto ch = AllocatedList.begin();
	for (; ch != AllocatedList.end(); ch ++) if ((*ch) + HeadSize == ptr) {
		FreeList.push_back(*ch);
		AllocatedList.erase(ch);
		break;
	}

}
unsigned maxSize() { return 1; };

void GarbCol()
{
	/*
	// сборщик мусора
	auto ch = List.begin(), pr = ch;
	for (int c = 0; ch != List.end(); ch++)
	{
		if ((*ch)->free == 1)
		{
			c++;
			if (c == 2)	// предыдущий был тоже свободный
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
	return;
	std::cout << str << "\n";
/*
	for (auto ch = List; ch != NULL; ch++)
		std::cout << ch->shift << "    "
		<< ch->size << "    "
		<< ch->free << "\n";
		*/
}