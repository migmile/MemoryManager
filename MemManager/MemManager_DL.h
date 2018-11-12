#pragma once
#include <iostream>
#include <stdlib.h>

class MemManager_DL
{
	struct MemChain
	{
		unsigned shift;		// смещение от начала 
		unsigned size;
		char free;      // занят - 0, свободен - 1
//		MemChain *prev;
		MemChain *next;
	};
	unsigned HeadSize;
	
	MemChain * List;
	MemChain * LastFree;
	char * Mem ;
	unsigned totalSize = 0;
	MemChain * FindLastFree(MemChain *ch)
	{ // найти первый свободный
		for (; ch != NULL; ch = ch->next)
		{
			if (ch->free == 1) return ch;
		}
	}
public:
	MemManager_DL(unsigned _size);
	~MemManager_DL();
	void * alloc(unsigned size);
	void free(void * ptr);
	void Print(const char * str);
};

// попытка сделать свой список вместо list

MemManager_DL::MemManager_DL(unsigned _size)
{
	HeadSize = sizeof MemChain;
	Mem = (char*)malloc(_size);
	if (Mem == NULL) throw "error allocating mem";
	totalSize = _size;
	List = (MemChain *)Mem;
	List->shift = 0;
	List->free = 1;
	List->size = totalSize - HeadSize;
	//List->prev=NULL;
	List->next = NULL;
	LastFree = List;
}

MemManager_DL::~MemManager_DL()
{
	if (Mem != NULL) free(Mem);
}


void * MemManager_DL::alloc(unsigned size)
{
	unsigned req_size = size + HeadSize;
	auto ch = LastFree;
	if (NULL == ch) ch = List;
	for (; ch != NULL; ch = ch->next)
	{
		if (ch->free == 1 && ch->size >= req_size) break;
	}
	if (ch != NULL)
	{	// выделим из данного блока нужный кусок и вставим в список ссылку на оставшуюся свободную часть 
		MemChain * ch1;		// new block - describe остаток
		unsigned shift = ch->shift + req_size; // сдвиг нового блока 
		ch1 = (MemChain *)(Mem + shift);
		ch1->next = ch->next;	// lists
		ch->next = ch1;
		ch1->shift = shift;
		ch1->free = 1;
		ch1->size = ch->size - req_size;
		ch->size = size;
		ch->free = 0;
		LastFree = FindLastFree(ch1);
		return ch1 + HeadSize;
	}
	return NULL;
}

void  MemManager_DL::free(void * ptr)
{
	if (ptr<Mem && ptr>Mem + totalSize) throw "wrong adr";
	auto ch = List;
	for (; ch != NULL; ch = ch->next) if (ch + HeadSize == ptr) {
		ch->free = 1;
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
	for (auto ch = List; ch != NULL; ch++)
		std::cout << ch->shift << "    "
		<< ch->size << "    "
		<< ch->free << "\n";
}