#include "ScannerThreadStackHandler.h"
// Make a new ThreadStack
// A thread stack object is used to manage actions across multiple threads
ScannerThreadStackHandler::ScannerThreadStackHandler()
{
	maxThreads = std::thread::hardware_concurrency();
}
ScannerThreadStackHandler::ScannerThreadStackHandler(uint maxThread)
{
	maxThreads = maxThread;
}

/*
* Wait for all the thread to finish and add the result to the coordinates list
*/
void ScannerThreadStackHandler::WaitForAllThread()
{
	for (auto& result : results)
	{
		result.wait();
		for (auto& coordinate : result.get())
		{
			coordinates.push_back(coordinate);
		}
	}
}

/*
* Add a new thread to the thread stack if the number of existing thread is less than the maximum number of thread allowed
* IN -------------------------------------------------------------
* Scanner scanner object: the scanner object used to scan the screen
* Image Haystack object:  the screen or the main image (the haystack)
* Image Needle object:    the target image (the needle)
*/
void ScannerThreadStackHandler::AddNewThreadIfPossible(autoGUI::Scanner scanner, Image Haystack, Image Needle)
{
	while (results.size() >= maxThreads)
	{
		ScannerThreadStackHandler::FinishSomeThread();
	}
	ScannerThreadStackHandler::MakeNewThread(scanner, Haystack, Needle);
}



/*
* CReate a new thread and add it to the thread stack
* IN -------------------------------------------------------------
* Scanner scanner object: the scanner object used to scan the screen
* Image Haystack object:  the screen or the main image (the haystack)
* Image Needle object:    the target image (the needle)
*/
void ScannerThreadStackHandler::MakeNewThread(autoGUI::Scanner scanner, Image Haystack, Image Needle)
{
	std::cout << "Adding new thread: " << results.size() << std::endl;
	//results.push_back(std::async(&Scanner::locateOnScreen, &scanner, Haystack, Needle));
}

/*
* Wait for some thread to finish. If thread came to its end: add the result to the coordinates list
*/
void ScannerThreadStackHandler::FinishSomeThread()
{
	int counter = 0;
	for (auto& thread : results)
	{
		if (thread.valid())
		{
			for (auto& coordinate : thread.get())
			{
				coordinates.push_back(coordinate);
				results.erase(results.begin() + counter);
			}
		}
		counter++;
	}
}