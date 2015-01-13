#include "stdafx.h"
#include <WinToolsLib\Signals\Signals.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(SignalsTests)
	{
	public:
		TEST_METHOD(SignalsTest)
		{
			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						value(0)
					{}

					int value;
					void Slot()
					{
						value = 10;
					}
				};

				MyStruct myStruct;
				WinToolsLib::Signals::Signal0<> signal;

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.Emit();
				Assert::AreEqual(10, myStruct.value);

				myStruct.Disable();
				myStruct.value = 0;
				signal.Emit();
				Assert::AreEqual(0, myStruct.value);

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.DisconnectAll();
				signal.Emit();
				Assert::AreEqual(0, myStruct.value);
			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						value1(0)
					{}

					int value1;
					void Slot(int val1)
					{
						value1 = val1;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal1<int> signal;

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.Emit(11);
				Assert::AreEqual(11, myStruct.value1);

				myStruct.Disable();
				myStruct.value1 = 0;
				signal.Emit(11);
				Assert::AreEqual(0, myStruct.value1);

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.DisconnectAll();
				signal.Emit(11);
				Assert::AreEqual(0, myStruct.value1);

			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						value1(0),
						value2(0)
					{}

					int value1;
					int value2;
					void Slot(int val1, int val2)
					{
						value1 = val1;
						value2 = val2;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal2<int, int> signal;

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.Emit(11, 12);
				Assert::AreEqual(11, myStruct.value1);
				Assert::AreEqual(12, myStruct.value2);

				myStruct.Disable();
				myStruct.value1 = 0;
				myStruct.value2 = 0;
				signal.Emit(11, 12);
				Assert::AreEqual(0, myStruct.value1);
				Assert::AreEqual(0, myStruct.value2);

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.DisconnectAll();
				signal.Emit(11, 12);
				Assert::AreEqual(0, myStruct.value1);
				Assert::AreEqual(0, myStruct.value2);
			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						value1(0),
						value2(0),
						value3(0)
					{}

					int value1;
					int value2;
					int value3;
					void Slot(int val1, int val2, int val3)
					{
						value1 = val1;
						value2 = val2;
						value3 = val3;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal3<int, int, int> signal;

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.Emit(11, 12, 13);
				Assert::AreEqual(11, myStruct.value1);
				Assert::AreEqual(12, myStruct.value2);
				Assert::AreEqual(13, myStruct.value3);

				myStruct.Disable();
				myStruct.value1 = 0;
				myStruct.value2 = 0;
				myStruct.value3 = 0;
				signal.Emit(11, 12,13);
				Assert::AreEqual(0, myStruct.value1);
				Assert::AreEqual(0, myStruct.value2);
				Assert::AreEqual(0, myStruct.value3);

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.DisconnectAll();
				signal.Emit(11, 12, 13);
				Assert::AreEqual(0, myStruct.value1);
				Assert::AreEqual(0, myStruct.value2);
				Assert::AreEqual(0, myStruct.value3);
			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						value1(0),
						value2(0),
						value3(0),
						value4(0)
					{}

					int value1;
					int value2;
					int value3;
					int value4;
					void Slot(int val1, int val2, int val3, int val4)
					{
						value1 = val1;
						value2 = val2;
						value3 = val3;
						value4 = val4;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal4<int, int, int, int> signal;

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(11, myStruct.value1);
				Assert::AreEqual(12, myStruct.value2);
				Assert::AreEqual(13, myStruct.value3);
				Assert::AreEqual(14, myStruct.value4);

				myStruct.Disable();
				myStruct.value1 = 0;
				myStruct.value2 = 0;
				myStruct.value3 = 0;
				myStruct.value4 = 0;
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(0, myStruct.value1);
				Assert::AreEqual(0, myStruct.value2);
				Assert::AreEqual(0, myStruct.value3);
				Assert::AreEqual(0, myStruct.value4);

				signal.Connect(&myStruct, &MyStruct::Slot);
				signal.DisconnectAll();
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(0, myStruct.value1);
				Assert::AreEqual(0, myStruct.value2);
				Assert::AreEqual(0, myStruct.value3);
				Assert::AreEqual(0, myStruct.value4);
			}();
		}

		TEST_METHOD(SignalsTestMultiSlot)
		{
			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						valueA(0),
						valueB(0)
					{}

					int valueA;
					int valueB;

					void SlotA()
					{
						valueA = 10;
					}

					void SlotB()
					{
						valueB = 10;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal0<> signal;

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.Emit();
				Assert::AreEqual(10, myStruct.valueA);
				Assert::AreEqual(10, myStruct.valueB);

				myStruct.Disable();
				myStruct.valueA = 0;
				myStruct.valueB = 0;
				signal.Emit();
				Assert::AreEqual(0, myStruct.valueA);
				Assert::AreEqual(0, myStruct.valueB);

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.DisconnectAll();
				signal.Emit();
				Assert::AreEqual(0, myStruct.valueA);
				Assert::AreEqual(0, myStruct.valueB);
			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						valueA1(0),
						valueB1(0)
					{}

					int valueA1;
					int valueB1;

					void SlotA(int val1)
					{
						valueA1 = val1;
					}

					void SlotB(int val1)
					{
						valueB1 = val1;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal1<int> signal;

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.Emit(11);
				Assert::AreEqual(11, myStruct.valueA1);
				Assert::AreEqual(11, myStruct.valueB1);

				myStruct.Disable();
				myStruct.valueA1 = 0;
				myStruct.valueB1 = 0;
				signal.Emit(11);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueB1);

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.DisconnectAll();
				signal.Emit(11);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueB1);
			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						valueA1(0),
						valueA2(0),
						valueB1(0),
						valueB2(0)
					{}

					int valueA1;
					int valueA2;
					int valueB1;
					int valueB2;

					void SlotA(int val1, int val2)
					{
						valueA1 = val1;
						valueA2 = val2;
					}

					void SlotB(int val1, int val2)
					{
						valueB1 = val1;
						valueB2 = val2;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal2<int, int> signal;

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.Emit(11, 12);
				Assert::AreEqual(11, myStruct.valueA1);
				Assert::AreEqual(12, myStruct.valueA2);
				Assert::AreEqual(11, myStruct.valueB1);
				Assert::AreEqual(12, myStruct.valueB2);

				myStruct.Disable();
				myStruct.valueA1 = 0;
				myStruct.valueA2 = 0;
				myStruct.valueB1 = 0;
				myStruct.valueB2 = 0;
				signal.Emit(11, 12);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueA2);
				Assert::AreEqual(0, myStruct.valueB1);
				Assert::AreEqual(0, myStruct.valueB2);

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.DisconnectAll();
				signal.Emit(11, 12);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueA2);
				Assert::AreEqual(0, myStruct.valueB1);
				Assert::AreEqual(0, myStruct.valueB2);
			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						valueA1(0),
						valueA2(0),
						valueA3(0),
						valueB1(0),
						valueB2(0),
						valueB3(0)
					{}

					int valueA1;
					int valueA2;
					int valueA3;
					int valueB1;
					int valueB2;
					int valueB3;

					void SlotA(int val1, int val2, int val3)
					{
						valueA1 = val1;
						valueA2 = val2;
						valueA3 = val3;
					}

					void SlotB(int val1, int val2, int val3)
					{
						valueB1 = val1;
						valueB2 = val2;
						valueB3 = val3;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal3<int, int, int> signal;

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.Emit(11, 12, 13);
				Assert::AreEqual(11, myStruct.valueA1);
				Assert::AreEqual(12, myStruct.valueA2);
				Assert::AreEqual(13, myStruct.valueA3);
				Assert::AreEqual(11, myStruct.valueB1);
				Assert::AreEqual(12, myStruct.valueB2);
				Assert::AreEqual(13, myStruct.valueB3);

				myStruct.Disable();
				myStruct.valueA1 = 0;
				myStruct.valueA2 = 0;
				myStruct.valueA3 = 0;
				myStruct.valueB1 = 0;
				myStruct.valueB2 = 0;
				myStruct.valueB3 = 0;
				signal.Emit(11, 12, 13);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueA2);
				Assert::AreEqual(0, myStruct.valueA3);
				Assert::AreEqual(0, myStruct.valueB1);
				Assert::AreEqual(0, myStruct.valueB2);
				Assert::AreEqual(0, myStruct.valueB3);

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.DisconnectAll();
				signal.Emit(11, 12, 13);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueA2);
				Assert::AreEqual(0, myStruct.valueA3);
				Assert::AreEqual(0, myStruct.valueB1);
				Assert::AreEqual(0, myStruct.valueB2);
				Assert::AreEqual(0, myStruct.valueB3);
			}();

			[]()
			{
				struct MyStruct : WinToolsLib::Signals::ClassTrack<>
				{
					MyStruct() :
						valueA1(0),
						valueA2(0),
						valueA3(0),
						valueA4(0),
						valueB1(0),
						valueB2(0),
						valueB3(0),
						valueB4(0)
					{}

					int valueA1;
					int valueA2;
					int valueA3;
					int valueA4;
					int valueB1;
					int valueB2;
					int valueB3;
					int valueB4;

					void SlotA(int val1, int val2, int val3, int val4)
					{
						valueA1 = val1;
						valueA2 = val2;
						valueA3 = val3;
						valueA4 = val4;
					}

					void SlotB(int val1, int val2, int val3, int val4)
					{
						valueB1 = val1;
						valueB2 = val2;
						valueB3 = val3;
						valueB4 = val4;
					}
				};
				MyStruct myStruct;
				WinToolsLib::Signals::Signal4<int, int, int, int> signal;

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(11, myStruct.valueA1);
				Assert::AreEqual(12, myStruct.valueA2);
				Assert::AreEqual(13, myStruct.valueA3);
				Assert::AreEqual(14, myStruct.valueA4);
				Assert::AreEqual(11, myStruct.valueB1);
				Assert::AreEqual(12, myStruct.valueB2);
				Assert::AreEqual(13, myStruct.valueB3);
				Assert::AreEqual(14, myStruct.valueB4);

				myStruct.Disable();
				myStruct.valueA1 = 0;
				myStruct.valueA2 = 0;
				myStruct.valueA3 = 0;
				myStruct.valueA4 = 0;
				myStruct.valueB1 = 0;
				myStruct.valueB2 = 0;
				myStruct.valueB3 = 0;
				myStruct.valueB4 = 0;
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueA2);
				Assert::AreEqual(0, myStruct.valueA3);
				Assert::AreEqual(0, myStruct.valueA4);
				Assert::AreEqual(0, myStruct.valueB1);
				Assert::AreEqual(0, myStruct.valueB2);
				Assert::AreEqual(0, myStruct.valueB3);
				Assert::AreEqual(0, myStruct.valueB4);

				signal.Connect(&myStruct, &MyStruct::SlotA);
				signal.Connect(&myStruct, &MyStruct::SlotB);
				signal.DisconnectAll();
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(0, myStruct.valueA1);
				Assert::AreEqual(0, myStruct.valueA2);
				Assert::AreEqual(0, myStruct.valueA3);
				Assert::AreEqual(0, myStruct.valueA4);
				Assert::AreEqual(0, myStruct.valueB1);
				Assert::AreEqual(0, myStruct.valueB2);
				Assert::AreEqual(0, myStruct.valueB3);
				Assert::AreEqual(0, myStruct.valueB4);
			}();
		}

		TEST_METHOD(SignalsTestAnonymousMultiSlot)
		{
			[]()
			{
				int valueA = 0;
				int valueB = 0;
				WinToolsLib::Signals::Signal0<> signal;

				auto trackA = signal.Connect([&valueA]()
				{
					valueA = 10;
				});
				auto trackB = signal.Connect([&valueB]()
				{
					valueB = 10;
				});
				signal.Emit();
				Assert::AreEqual(10, valueA);
				Assert::AreEqual(10, valueB);

				trackA->Disable();
				trackB->Disable();
				valueA = 0;
				valueB = 0;
				signal.Emit();
				Assert::AreEqual(0, valueA);
				Assert::AreEqual(0, valueB);

				trackA = signal.Connect([&valueA]()
				{
					valueA = 10;
				});
				trackB = signal.Connect([&valueB]()
				{
					valueB = 10;
				});
				signal.DisconnectAll();
				signal.Emit();
				Assert::AreEqual(0, valueA);
				Assert::AreEqual(0, valueB);
			}();

			[]()
			{
				int valueA1 = 0;
				int valueB1 = 0;
				WinToolsLib::Signals::Signal1<int> signal;

				auto trackA = signal.Connect([&valueA1](int val1)
				{
					valueA1 = val1;
				});
				auto trackB = signal.Connect([&valueB1](int val1)
				{
					valueB1 = val1;
				});
				signal.Emit(11);
				Assert::AreEqual(11, valueA1);
				Assert::AreEqual(11, valueB1);

				trackA->Disable();
				trackB->Disable();
				valueA1 = 0;
				valueB1 = 0;
				signal.Emit(11);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueB1);

				trackA = signal.Connect([&valueA1](int val1)
				{
					valueA1 = val1;
				});
				trackB = signal.Connect([&valueB1](int val1)
				{
					valueB1 = val1;
				});
				signal.DisconnectAll();
				signal.Emit(11);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueB1);
			}();

			[]()
			{
				int valueA1 = 0;
				int valueA2 = 0;
				int valueB1 = 0;
				int valueB2 = 0;
				WinToolsLib::Signals::Signal2<int, int> signal;

				auto trackA = signal.Connect([&valueA1, &valueA2](int val1, int val2)
				{
					valueA1 = val1;
					valueA2 = val2;
				});
				auto trackB = signal.Connect([&valueB1, &valueB2](int val1, int val2)
				{
					valueB1 = val1;
					valueB2 = val2;
				});
				signal.Emit(11, 12);
				Assert::AreEqual(11, valueA1);
				Assert::AreEqual(12, valueA2);
				Assert::AreEqual(11, valueB1);
				Assert::AreEqual(12, valueB2);

				trackA->Disable();
				trackB->Disable();
				valueA1 = 0;
				valueA2 = 0;
				valueB1 = 0;
				valueB2 = 0;
				signal.Emit(11, 12);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueA2);
				Assert::AreEqual(0, valueB1);
				Assert::AreEqual(0, valueB2);

				trackA = signal.Connect([&valueA1, &valueA2](int val1, int val2)
				{
					valueA1 = val1;
					valueA2 = val2;
				});
				trackB = signal.Connect([&valueB1, &valueB2](int val1, int val2)
				{
					valueB1 = val1;
					valueB2 = val2;
				});
				signal.DisconnectAll();
				signal.Emit(11, 12);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueA2);
				Assert::AreEqual(0, valueB1);
				Assert::AreEqual(0, valueB2);
			}();

			[]()
			{
				int valueA1 = 0;
				int valueA2 = 0;
				int valueA3 = 0;
				int valueB1 = 0;
				int valueB2 = 0;
				int valueB3 = 0;
				WinToolsLib::Signals::Signal3<int, int, int> signal;

				auto trackA = signal.Connect([&valueA1, &valueA2, &valueA3](int val1, int val2, int val3)
				{
					valueA1 = val1;
					valueA2 = val2;
					valueA3 = val3;
				});
				auto trackB = signal.Connect([&valueB1, &valueB2, &valueB3](int val1, int val2, int val3)
				{
					valueB1 = val1;
					valueB2 = val2;
					valueB3 = val3;
				});
				signal.Emit(11, 12, 13);
				Assert::AreEqual(11, valueA1);
				Assert::AreEqual(12, valueA2);
				Assert::AreEqual(13, valueA3);
				Assert::AreEqual(11, valueB1);
				Assert::AreEqual(12, valueB2);
				Assert::AreEqual(13, valueB3);

				trackA->Disable();
				trackB->Disable();
				valueA1 = 0;
				valueA2 = 0;
				valueA3 = 0;
				valueB1 = 0;
				valueB2 = 0;
				valueB3 = 0;
				signal.Emit(11, 12, 13);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueA2);
				Assert::AreEqual(0, valueA3);
				Assert::AreEqual(0, valueB1);
				Assert::AreEqual(0, valueB2);
				Assert::AreEqual(0, valueB3);

				trackA = signal.Connect([&valueA1, &valueA2, &valueA3](int val1, int val2, int val3)
				{
					valueA1 = val1;
					valueA2 = val2;
					valueA3 = val3;
				});
				trackB = signal.Connect([&valueB1, &valueB2, &valueB3](int val1, int val2, int val3)
				{
					valueB1 = val1;
					valueB2 = val2;
					valueB3 = val3;
				});
				signal.DisconnectAll();
				signal.Emit(11, 12, 13);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueA2);
				Assert::AreEqual(0, valueA3);
				Assert::AreEqual(0, valueB1);
				Assert::AreEqual(0, valueB2);
				Assert::AreEqual(0, valueB3);
			}();

			[]()
			{
				int valueA1 = 0;
				int valueA2 = 0;
				int valueA3 = 0;
				int valueA4 = 0;
				int valueB1 = 0;
				int valueB2 = 0;
				int valueB3 = 0;
				int valueB4 = 0;
				WinToolsLib::Signals::Signal4<int, int, int, int> signal;

				auto trackA = signal.Connect([&valueA1, &valueA2, &valueA3, &valueA4](int val1, int val2, int val3, int val4)
				{
					valueA1 = val1;
					valueA2 = val2;
					valueA3 = val3;
					valueA4 = val4;
				});
				auto trackB = signal.Connect([&valueB1, &valueB2, &valueB3, &valueB4](int val1, int val2, int val3, int val4)
				{
					valueB1 = val1;
					valueB2 = val2;
					valueB3 = val3;
					valueB4 = val4;
				});
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(11, valueA1);
				Assert::AreEqual(12, valueA2);
				Assert::AreEqual(13, valueA3);
				Assert::AreEqual(14, valueA4);
				Assert::AreEqual(11, valueB1);
				Assert::AreEqual(12, valueB2);
				Assert::AreEqual(13, valueB3);
				Assert::AreEqual(14, valueB4);

				trackA->Disable();
				trackB->Disable();
				valueA1 = 0;
				valueA2 = 0;
				valueA3 = 0;
				valueA4 = 0;
				valueB1 = 0;
				valueB2 = 0;
				valueB3 = 0;
				valueB4 = 0;
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueA2);
				Assert::AreEqual(0, valueA3);
				Assert::AreEqual(0, valueA4);
				Assert::AreEqual(0, valueB1);
				Assert::AreEqual(0, valueB2);
				Assert::AreEqual(0, valueB3);
				Assert::AreEqual(0, valueB4);

				trackA = signal.Connect([&valueA1, &valueA2, &valueA3, &valueA4](int val1, int val2, int val3, int val4)
				{
					valueA1 = val1;
					valueA2 = val2;
					valueA3 = val3;
					valueA4 = val4;
				});
				trackB = signal.Connect([&valueB1, &valueB2, &valueB3, &valueB4](int val1, int val2, int val3, int val4)
				{
					valueB1 = val1;
					valueB2 = val2;
					valueB3 = val3;
					valueB4 = val4;
				});
				signal.DisconnectAll();
				signal.Emit(11, 12, 13, 14);
				Assert::AreEqual(0, valueA1);
				Assert::AreEqual(0, valueA2);
				Assert::AreEqual(0, valueA3);
				Assert::AreEqual(0, valueA4);
				Assert::AreEqual(0, valueB1);
				Assert::AreEqual(0, valueB2);
				Assert::AreEqual(0, valueB3);
				Assert::AreEqual(0, valueB4);
			}();
		}
	};
}