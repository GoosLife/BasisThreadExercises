#include <iostream>
#include <thread> // Used for std::thread, which is used for creating threads in C++
#include <future> // Used for std::async and std::future, which is used for creating threads that can tell you when they are done

/// <summary>
/// Class used in exercise 0, rewritten from C# to C++
/// </summary>
class Program {
public:
	/// <summary>
	/// Method from exercise 0,  rewritten in  C++.
	/// The name is passed as a parameter because you can't name your threads in C++.
	/// I decided not to use the std::thread::get_id(), because the ids are not as easily identifiable (i.e. the first thread may get ID 78504, etc).
	/// </summary>
	/// <param name="threadName">The name of the thread currently calling this method.</param>
	void WorkThreadFunction(std::string threadName) {
		for (int i = 0; i < 5; i++) {
			std::cout << "Simple Thread from " << threadName << '\n';
		}
	}
};

/// <summary>
/// Console helper class, used to store methods that can be called from the threads, and to avoid having to write std::cout etc. every time.
/// The methods are static so they can be called without having to create an instance of the class.
/// </summary>
class Console {
public:
	// Write a string to the console, end with a newline
	static void WriteLine(std::string input) {
		std::cout << input << '\n';
	}

	// Write a string to the console multiple times
	static void RepeatWriteLine(std::string input, int repetitions) {
		for (int i = 0; i < repetitions; i++)
		{
			WriteLine(input);
		}
	}
};

/// <summary>
/// Class for simulating a temperature sensor, used in exercise 3.
/// </summary>
class TemperatureReader {
public:
	/// <summary>
	/// The method that is called from the thread in exercise 3.
	/// Simulates reading a temperature from a sensor, and prints it to the console.
	/// If the temperature is outside the range of 0-100, the strike counter is incremented.
	/// If the strike counter reaches 3, the thread is terminated.
	/// </summary>
	void Run() {
		char* toWrite = new char[128]; // Allocate memory for the string to be written to the console
		int16_t readTemperature = ReadTemperature(); // Read the temperature

		if (readTemperature < 0 || readTemperature > 100) { // If the temperature is outside the range of 0-100, increment the strike counter
			m_strikes++;
		}

		sprintf_s(toWrite, 128, "%u", readTemperature); // Convert the temperature to a string

		Console::WriteLine(toWrite); // Write the temperature to the console
	}

	/// <summary>
	/// Returns a simulated temperature reading.
	/// </summary>
	/// <returns></returns>
	std::int16_t ReadTemperature() {
		return (rand() % 140) - 20; // Return a random number between -20 and 120
	}

	int GetStrikes() { return m_strikes; } // Returns the strike counter

private:
	int m_strikes = 0; // Strike counter. Keeps track of how many times the temperature has been outside the range of 0-100
};

int main()
{
	// Init random seed
	// This is used in the TemperatureReader class to generate random numbers
	srand((unsigned)time(NULL));

	/**
	EXERCISE 0
	*/

	// Create two threads that write "Simple Thread from {threadName}" to the console 5 times each
	Program pg = Program();

	std::thread t1([&pg]() {pg.WorkThreadFunction("t1");}); // Lambda method
	std::thread t2(&Program::WorkThreadFunction, &pg, "t2"); // Function pointer and object passed as parameters

	// Wait for the threads to finish, then join them to the main thread so the program doesn't terminate before the threads are done
	t1.join();
	t2.join();

	/**
	EXERCISE 1
	*/

	// Writes "C++ trådning er nemt!" 5 times to the console using the RepeatWriteLine method from the Console class
	std::thread t3(Console::RepeatWriteLine, "C++ trådning er nemt!", 5);
	t3.join(); // Wait for the thread to finish

	/**
	EXERCISE 2
	*/
	
	// Create two threads that write "C++ trådning er nemt!" and "Også med flere tråde..." to the console, respectively, 5 times each, sleeping for 1 second between each write
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

	// Wait for the threads to finish, then join them to the main thread so the program doesn't terminate before the threads are done
	t4.join();
	t5.join();

	/**
	EXERCISE 3
	*/

	// Create a new TemperatureReader object, and create a new thread that calls the Run method of the TemperatureReader object every 2 seconds, until the strike counter reaches 3
	TemperatureReader* tr = new TemperatureReader();
	std::future<void> t6 = std::async([&tr]() {
		while (tr->GetStrikes() < 3) {
			tr->Run();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		});

	// Check if the thread is done every 10 seconds, and announce its termination when it is
	while (!(t6.wait_for(std::chrono::seconds(10)) == std::future_status::ready)) {
		std::cout << "Tjekker tråd igen om 10 sekunder\n";
	}
	std::cout << "Alarm-tråd termineret!\n";

	// Wait for user input before terminating the program
	char c;
	std::cin >> c;
}