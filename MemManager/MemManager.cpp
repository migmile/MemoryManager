// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// MemManager.cpp 
// Øá·Ò¨á Ò¨Ô¨¦é¨á  Ø ÒÞå·
// ë á· Ôå 2 - ëñ¦¨ÐÞ¨åãÞ ¢ÐÖÆ Ò·, ë Ô û Ð¨ Æ é¦Ö¬Ö - ãåáçÆåçá  MemChain

#include <iostream>
#include <list>
#include <deque>
#include <vector>
#include <malloc.h>
#include <stdlib.h>
#include <chrono>
#include "MManagerL.h"
#include "MManagerL1.h"
#include "MManagerD.h"
#include "MemManager_DL.h"
#include "testmem.h"

using std::cout;
using std::list;

const unsigned Len = 1024 * 100;
const unsigned Count = 5000;


int Test_newPlacement()
{
	std::vector<int *> pointers;
	pointers.reserve(Count);
	//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;
	//int * Mem = new int[Len * 10];

	for (int i = 0; i < Count; i++)
	{
		int * ptr = new int[Len / Count];//(int*)malloc(Len / Count * sizeof(int));
		WriteMem(ptr, Len / Count);
		pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";

	for (int i = 0; i < Count; i++)
	{
		ReadMem(pointers[i], Len / Count);
//		free(pointers[i]);
		//delete[] pointers[i];
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";

	//delete[] Mem;
	return 1;
}


int TestMM_DL()
{
	MemManager_DL mm(Len * sizeof(int)+24*Count);

	std::vector<int *> pointers;
	pointers.reserve(Count);
//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
		//if (ptr!=NULL) 
			WriteMem(ptr, Len / Count);
		/*else {
			cout << "Errrooorrr!!!!";
		}*/
		pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";

	for (int i = 0; i < Count; i++)
	{
		ReadMem(pointers[i], Len / Count);
		mm.free(pointers[i]);
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";

	return 1;
}

int TestMMList1()
{
	MemManagerL1 mm(Len * sizeof(int));

	std::vector<int *> pointers;
	pointers.reserve(Count);
//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
		WriteMem(ptr, Len / Count);
		pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";
	
	for (int i = 0; i < Count; i++)
	{
		ReadMem(pointers[i], Len / Count);
		mm.free(pointers[i]);
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;
}

int TestMMList()
{
	MemManagerL mm(Len * sizeof(int)+24*Count);

		std::vector<int *> pointers;
		pointers.reserve(Count);
//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
		WriteMem(ptr, Len / Count);
		pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";

	for (int i = 0; i < Count; i++)
	{
		ReadMem(pointers[i], Len / Count);
		mm.free(pointers[i]);
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;
	
}

int TestMMDeque()
{
	MemManagerD mm(Len * sizeof(int)+24*Count);

	std::vector<int *> pointers;
	pointers.reserve(Count);
//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
		WriteMem(ptr, Len / Count);
		pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";
	
	for (int i = 0; i < Count; i++)
	{
		ReadMem(pointers[i], Len / Count);
		mm.free(pointers[i]);
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;

}


int main()
{
	std::cout << "New Manager\n";
	Test_newPlacement();

	std::cout << "LIST1\n";
	TestMMList1();

	std::cout << "My List Manager\n";
	TestMM_DL();
		
	std::cout << "LIST\n";
	TestMMList();

	std::cout << "DEQUE\n";
	TestMMDeque();
	
	return 0;
}