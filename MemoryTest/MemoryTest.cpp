#include "stdafx.h"
#include "CppUnitTest.h"

#include <Engine.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryTest
{		
	TEST_CLASS(DummyTest)
	{
	public:
		TEST_METHOD(PointerTest)
		{
			int* ptr = new int;
			Assert::IsNotNull(ptr);

			delete ptr;
			ptr = nullptr;
			Assert::IsNull(ptr);
		}

		TEST_METHOD(AdditionTest)
		{
			int a = 4;
			int b = 7;
			int c = a + b;
			Assert::AreEqual(c, 11);
		}
	};

	TEST_CLASS(BlockAllocatorTest)
	{
	public:
		TEST_METHOD(OneBlockTest)
		{
			BlockAllocator blockAllocator(16, 1);
			void* ptr = blockAllocator.Allocate();
			Assert::IsNotNull(ptr);
		}

		TEST_METHOD(TwoBlockTest)
		{
			BlockAllocator blockAllocator(16, 2);
			void* ptr1 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr1);
			void* ptr2 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr2);
			void* ptr3 = blockAllocator.Allocate();
			Assert::IsNull(ptr3);
		}

		TEST_METHOD(FreeTest)
		{
			BlockAllocator blockAllocator(16, 1);
			void* ptr1 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr1);
			void* ptr2 = blockAllocator.Allocate();
			Assert::IsNull(ptr2);
			blockAllocator.Free(ptr1);
			ptr2 = blockAllocator.Allocate();
			Assert::IsNotNull(ptr2);
			Assert::IsTrue(ptr1 == ptr2);
		}

		//TEST_METHOD(PtrInRangeCheck)
		//{
		//	BlockAllocator blockAllocator1(16, 1);
		//	BlockAllocator blockAllocator2(16, 1);
		//	void* ptr1 = blockAllocator2.Allocate();
		//	blockAllocator1.Free(ptr1);
		//	Assert::Fail();
		//}
	};

	TEST_CLASS(TypedAllocatorTest)
	{
	public:
		class Bar
		{
		public:
			static int counter;
			Bar() { counter = 0; } // use to test how many times constructor is called
			Bar(const Bar&) { counter = 1; }
			Bar(Bar&&) { counter = 2; }
		};

		class Foo
		{
		public:
			int a, b;
			Foo() : a(42), b(7) {}
			Foo(int i, int j) : a(i), b(j) {}
			Foo(Bar b) : a(12), b(34) {}
			~Foo() { a = 0xfeeefeee, b = 0xdeadbeef; }
		};

		TEST_METHOD(NewTest)
		{
			TypedAllocator<int> typedAllocator(1);
			int* ptr1 = typedAllocator.New();
			Assert::IsNotNull(ptr1);
		}

		TEST_METHOD(DeleteTest)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);

			typedAllocator.Delete(ptr);
			Assert::AreEqual((unsigned int)ptr->a, 0xfeeefeee);
			Assert::AreEqual((unsigned int)ptr->b, 0xdeadbeef);
		}

		TEST_METHOD(ConstructorDestructorTest)
		{
			class DummyClass
			{
			public:
				DummyClass() { size = 6; }
				~DummyClass() { length = 8.0f; }
			
				int size = 5;
				char letter = 'a';
				float length = 7.0f;
			};

			TypedAllocator<DummyClass> typedAllocator(1);
			DummyClass* ptr1 = typedAllocator.New();
			Assert::AreEqual(ptr1->size, 6);
			typedAllocator.Delete(ptr1);
			Assert::AreEqual(ptr1->length, 8.0f);
		}

		TEST_METHOD(ForwardingTest)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New(Bar());
			Assert::IsNotNull(ptr);
			Assert::AreEqual(Bar::counter, 2);
		}
	};

	TEST_CLASS(ArenaAllocatorTest)
	{
	public:
		TEST_METHOD(AllocateTest)
		{
			ArenaAllocator arenaAllocator(16);
			void* alloc = arenaAllocator.Allocate(4);
			Assert::IsNotNull(alloc);
		}
		TEST_METHOD(RewindTest)
		{
			ArenaAllocator arenaAllocator(16);
			void* alloc = arenaAllocator.Allocate(4);
			Assert::IsNotNull(alloc);
			void* alloc2 = arenaAllocator.Allocate(14);
			Assert::IsNotNull(alloc2);
		}
		TEST_METHOD(CapacityLimitTest)
		{
			ArenaAllocator arenaAllocator(16);
			void* alloc = arenaAllocator.Allocate(17);
			Assert::IsNull(alloc);
		}
	};

	int TypedAllocatorTest::Bar::counter = 0;

	TEST_CLASS(HandlePoolTest)
	{
	public:
		TEST_METHOD(RegisterTest)
		{
			HandlePool<int> handlePool(10);
			int* ptr1 = new int(5);

			Handle<int> handle1;
			Assert::IsFalse(handlePool.IsValid(handle1));

			handle1 = handlePool.Register(ptr1);
			Assert::IsTrue(handlePool.IsValid(handle1));
		}
		TEST_METHOD(ValidCheck)
		{
			HandlePool<int> handlePool(10);
			int* ptr1;
			*ptr1 = 5;

			Handle<int> handle1;
			handle1 = handlePool.Register(ptr1);
			Handle<int> handle2;

			Assert::IsFalse(handle2.IsValid());
			Assert::IsTrue(handle1.IsValid());
		}
		TEST_METHOD(UnRegisterTest)
		{
			HandlePool<int> handlePool(10);
			int* ptr1;
			*ptr1 = 5;

			Handle<int> handle1;
			handle1 = handlePool.Register(ptr1);

			Assert::IsTrue(handlePool.IsValid(handle1));
			handlePool.Unregister(handle1);
			Assert::IsFalse(handlePool.IsValid(handle1));
		}
	};
}