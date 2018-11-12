// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// MemManager.cpp
// пример менеджера памяти
//

#include <iostream>
#include <list>
#include <vector>
#include <malloc.h>
#include <stdlib.h>
#include <chrono>


using std::cout;
using std::list;


class MemManager
{
	struct MemChain
	{
		unsigned shift;		// смещение от начала
		unsigned size;
		char free;      // занят - 0, свободен - 1
	};

	list <MemChain> List;
	char * Mem=NULL;
	unsigned totalSize=0;
public:
	MemManager(unsigned _size) {
		Mem = (char*) malloc(_size);
		if (Mem == NULL) throw "error allocating mem";
		totalSize = _size;
		MemChain ch;
		ch.shift = 0;
		ch.free = 1;
		ch.size = totalSize;
		List.push_front(ch);
	}

	~MemManager() {
		if (Mem!=NULL) free(Mem);
	}

	void * alloc(unsigned size) {
//		if (есть память)
		{
			auto ch=List.begin();
			for (; ch != List.end();ch++) if (ch->free == 1 && ch->size>=size) break;
			if (ch != List.end())
			{						// выделим из данного блока нужный кусок и вставим в список ссылку на оставшуюся свободную часть
				MemChain ch1;		// остаток
				ch1.shift = ch->shift+size;
				ch1.free = 1;
				ch1.size = ch->size - size;
				ch->size = size;
				ch->free = 0;
				auto ptr = ch;
				ptr++;
				List.insert(ptr,ch1);

				return (Mem + ch->shift);
			}
		}
		return NULL;
	}
	void  free(void*ptr) {
		if (ptr<Mem && ptr>Mem + totalSize) throw "wrong adr";
		auto ch = List.begin();
		for (; ch != List.end(); ch++) if (Mem + ch->shift == ptr) {
			ch->free = 1;
			break;
		}
	}
	unsigned maxSize() {};

	void GarbCol()
	{
		// сборщик мусора
		auto ch = List.begin(),pr=ch;
		for (int c = 0; ch != List.end(); ch++)
		{
			if (ch->free == 1)
			{
				c++;
				if (c == 2)	// предыдущий был тоже свободный
				{
					pr->size += ch->size;
					List.erase(ch);
					c = 1;
					ch = pr;
					continue;
				}
			}
			else c = 0;
			pr = ch;
		}
	};

	void Print(const char * str) {
		cout << str << "\n";
		for (auto ch = List.begin(); ch != List.end(); ch++) cout << ch->shift << "    " << ch->size << "    " << ch->free << "\n";
	}

};

const unsigned Len = 1024 * 100;
const unsigned Count = 5000;

int main()
{
	MemManager mm(Len*sizeof(int));
	std::vector<int *> pointers;
	pointers.reserve(Count);
	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1=std::chrono::high_resolution_clock::now(),p2,p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)malloc(Len / Count * sizeof(int));
		pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";
//	return 1;
	for (int i = 0; i < Count; i++)
	{
		free(pointers[i]);//mm.free(pointers[i]);
	}
	p3=std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;


	int * ptr1 = (int*)mm.alloc(5 * sizeof(int));
	int * ptr2=(int*)mm.alloc(10 * sizeof(int));
	mm.Print("first");
//	mm.free(ptr);
	mm.free(ptr1);
	mm.Print("free");
//	ptr=(int*)mm.alloc(5 * sizeof(int));
	mm.GarbCol();
	mm.Print("second");
	return 0;
}
