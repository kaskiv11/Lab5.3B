#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab5.3B/Lab5.3B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLab53B
{
	TEST_CLASS(UnitTestLab53B)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			char g[6] = "g.dat";
			int t = binSearch(g, "1", 1, 1);
			Assert::AreEqual(t, -1); 
		}
	};
}
