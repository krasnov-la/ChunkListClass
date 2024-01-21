#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../ChunkList/ChunkList.h"

using namespace fefu_laboratory_two;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChunkListUnitTest
{
	TEST_CLASS(Constructors)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			ChunkList<int, 12> list;


			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
		}

		TEST_METHOD(CopyConstructor)
		{
			ChunkList<int, 10> list1;
			for (int i = 0; i < 10; i++) list1.push_back(i);


			auto list2 = list1;

			
			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(InitializerListConstructor)
		{
			ChunkList<int, 10> list1 = { 1,2,3,4,5 };
			ChunkList<int, 10> list2;
			for (int i = 1; i <= 5; i++) {
				list2.push_back(i);
			}


			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(IteratorsConstructor)
		{
			std::vector<int> v = { 1,2,3,4,5,6,7 };
			auto first = v.begin();
			auto last = v.end();
			ChunkList<int, 10> list1(first, last);
			ChunkList<int, 10> list2 = { 1,2,3,4,5,6,7 };


			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(Assign)
		{
			ChunkList<int, 10> list1;
			ChunkList<int, 10> list2;
			for (int i = 0; i < 10; i++) list1.push_back(7);


			list2.assign(10, 7);


			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(AssignIterators)
		{
			std::vector<int> v = { 1,2,3,4,5,6,7 };
			auto first = v.begin();
			auto last = v.end();
			ChunkList<int, 10> list1;
			ChunkList<int, 10> list2;
			for (int i = 1; i <= 7; i++) list2.push_back(i);


			list1.assignIt(first, last);


			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(AssignInitList)
		{
			ChunkList<int, 10> list1;
			ChunkList<int, 10> list2 = { 1,2,3,4,5 };


			list1.assign({ 1,2,3,4,5 });


			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(AssignmentOperator)
		{
			ChunkList<int, 10> list1;
			ChunkList<int, 10> list2 = { 1,2,3,4,5 };


			list1 = { 1,2,3,4,5 };


			Assert::IsTrue(list1 == list2);
		}
	};

	TEST_CLASS(ElementAccess)
	{
	public:
		TEST_METHOD(At)
		{
			ChunkList<int, 10> list;
			for (int i = 0; i < 15; i++) list.push_back(i);
			

			for (int i = 0; i < 15; i++) Assert::AreEqual(i, list.at(i));
		}

		TEST_METHOD(Indexation)
		{
			ChunkList<int, 10> list;
			for (int i = 0; i < 15; i++) list.push_back(i);


			for (int i = 0; i < 15; i++) Assert::AreEqual(i, list.at(i));
		}

		TEST_METHOD(Front)
		{
			ChunkList<int, 10> list = {42, 1, 5, 7, 4, 1};
			

			Assert::IsTrue(list.front() == 42);
		}

		TEST_METHOD(Back)
		{
			ChunkList<int, 10> list = {1, 1, 5, 8, 0, 2, 5, 7, 42};


			Assert::IsTrue(list.back() == 42);
		}
	};

	TEST_CLASS(Iterator) {
		TEST_METHOD(IteratorsForeach) {
			ChunkList<int, 10> list = { 1, 4, 7, 8, 9, 13, 45 ,7, 9, 2 };
			int* arr = new int[10] { 1, 4, 7, 8, 9, 13, 45, 7, 9, 2 };

			int i = 0;
			for (auto e : list) Assert::IsTrue(arr[i++] == e);
		}

		TEST_METHOD(IteratorsCompare)
		{
			ChunkList<int, 10> list = { 1, 4, 7, 8, 9, 13, 45 ,7, 9, 2 };
			int* arr = new int[10] { 1, 4, 7, 8, 9, 13, 45, 7, 9, 2 };

			auto it1 = list.begin();
			auto it2 = list.begin();

			Assert::IsTrue(it1 == it2);
			Assert::IsTrue(it1 >= it2);
			Assert::IsTrue(it1 <= it2);
			
			it1++;
			Assert::IsTrue(it1 > it2);
			it2 += 7;
			Assert::IsTrue(it2 > it1);
		}
	};

	TEST_CLASS(Capacity) {
		TEST_METHOD(CapacityEmpty) {
			ChunkList<int, 11> list;


			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
			Assert::IsTrue(list.max_size() == 0);
		}

		TEST_METHOD(CapacityNonEmpty)
		{
			ChunkList<int, 4> list = {1, 2, 3, 4, 5, 6, 7};


			Assert::IsTrue(list.size() == 7);
			Assert::IsTrue(list.max_size() == 8);
		}
	};

	TEST_CLASS(Modifier) {
		TEST_METHOD(Emplace)
		{
			ChunkList<int, 10> list = { 1,2,3,4,5 };
			ChunkList<int, 10> expected = { 1,2,3,-1,-2,-3,-4,-5,4,5 };


			list.emplace(list.cbegin() + 3, -1, -2, -3, -4, -5);


			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(EmplaceBack)
		{
			ChunkList<int, 10> list = { 1,2,3,4,5 };
			ChunkList<int, 10> expected = { 1,2,3,4,5,-1,2 };


			list.emplace_back(-1, 2);
			
			
			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(EmplaceFront)
		{
			ChunkList<int, 10> list = { 1,2,3,4,5 };
			ChunkList<int, 10> expected = { -1,2,-3,1,2,3,4,5 };

			auto it2 = list.emplace_front(-1,2,-3);


			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(Insert1) {
			ChunkList<int, 10> list;
			ChunkList<int, 10> expected = { 100, 0, 1, 2 };

			for (int i = 0; i < 10; i++)
				list.push_back(i);

			list.clear();

			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
			Assert::IsTrue(list.max_size() == 0);

			for (int i = 0; i < 3; i++)
				list.push_back(i);

			auto it = list.cbegin();
			list.insert(it, 100);

			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(Insert2) {
			std::vector<int> vec = { -1,22,-333 };
			ChunkList<int, 10> list = { 1,2,3,4,5 };
			ChunkList<int, 10> expected = { -1,22,-333,1,2,3,4,5 };

			list.insert(list.cbegin(), vec.begin(), vec.end());

			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(Resize)
		{
			ChunkList<int, 8> list1 = { 1,2,3,4,5 };
			ChunkList<int, 8> list2 = { 1,2,3,4,5,-1,-1,-1,-1,-1};

			list1.resize(10, -1);

			Assert::IsTrue(list1 == list2);

			list1.resize(4);
			list2.resize(4);

			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(Erase) {
			ChunkList<int, 8> list = {1,2,3,4,5};
			ChunkList<int, 8> expected = { 1,2,5 };


			list.erase(list.cbegin() + 1, list.cbegin() + 3);


			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(PushBack) {
			ChunkList<int, 10> list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			Assert::IsTrue(list[0] == 0);
			Assert::IsTrue(list[10] == 10);
		}

		TEST_METHOD(PushFront) {
			ChunkList<int, 10> list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			list.push_front(42);
			Assert::IsTrue(list[0] == 42);
		}

		TEST_METHOD(PopBack) {
			ChunkList<int, 10> list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			list.pop_back();
			Assert::IsTrue(list.size() == 10);
		}

		TEST_METHOD(PopFront) {
			ChunkList<int, 10> list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			list.pop_front();
			Assert::IsTrue(list[0] == 1);
		}
	};

	TEST_CLASS(Comparision) {
		TEST_METHOD(Comparisions) {
			ChunkList<int, 10> list1 = { 1,2,3,4,5 };
			ChunkList<int, 10> list2 = { 1,2,3,4,5 };
			ChunkList<int, 10> list3 = { 1,2,6,4,5 };
			ChunkList<int, 10> list4 = { 1,2,3,4,5,1 };


			Assert::IsTrue(list1 == list2);
			Assert::IsTrue(list1 <= list2);
			Assert::IsTrue(list1 >= list2);
			Assert::IsTrue(list3 > list2);
			Assert::IsTrue(list4 > list2);
			Assert::IsTrue(list1 < list4);
			Assert::IsFalse(list2 > list3);
		}
	};
}
