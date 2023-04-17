// BasisThreadExercises.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <future>
class Program {
public:
	void WorkThreadFunction(std::string threadName) {
		for (int i = 0; i < 5; i++) {
			std::cout << "Simple Thread from " << threadName << '\n';
		}
	}
};

class Console {
public:
	static void WriteLine(std::string input) {
		std::cout << input << '\n';
	}

	static void RepeatWriteLine(std::string input, int repetitions) {
		for (int i = 0; i < repetitions; i++)
		{
			WriteLine(input);
		}
	}
};

class TemperatureReader {
public:
	void Run() {
		char* toWrite = new char[128];
		int16_t readTemperature = ReadTemperature();

		if (readTemperature < 0 || readTemperature > 100) {
			m_strikes++;
		}

		sprintf_s(toWrite, 128, "%u", readTemperature);

		Console::WriteLine(toWrite);
	}

	std::int16_t ReadTemperature() {
		return (rand() % 140) - 20;
	}

	int GetStrikes() { return m_strikes; }

private:
	int m_strikes = 0;
};

int main()
{
	// Init random seed
	srand((unsigned)time(NULL));

	/**
	EXERCISE 0
	*/
	Program pg = Program();

	std::thread t1([&pg]() {pg.WorkThreadFunction("t1");}); // Lambda method
	std::thread t2(&Program::WorkThreadFunction, &pg, "t2"); // Function pointer and object passed as parameters
	t1.join();
	t2.join();

	/**
	EXERCISE 1
	*/
	std::thread t3(Console::RepeatWriteLine, "C++ trådning er nemt!", 5);
	t3.join();

	/**
	EXERCISE 2
	*/
	std::thread t4([=] {
		for (int i = 0; i < 5; i++) {
			Console::WriteLine("C++ trådning er nemt!");
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}});
	std::thread t5([=] {
		for (int i = 0; i < 5; i++) {
			Console::WriteLine("Også med flere tråde...");
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}});

	t4.join();
	t5.join();

	/**
	EXERCISE 3
	*/
	TemperatureReader* tr = new TemperatureReader();
	std::future<void> t6 = std::async([&tr]() {
		while (tr->GetStrikes() < 3) {
			tr->Run();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		});


	while (!(t6.wait_for(std::chrono::seconds(10)) == std::future_status::ready)) {
		std::cout << "Tjekker tråd igen om 10 sekunder\n";
	}
	std::cout << "Alarm-tråd termineret!\n";

	char c;
	std::cin >> c;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
