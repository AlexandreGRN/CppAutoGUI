#pragma once
#include "Libraries.h"
#include "Scanner.h"
#include "ImageSource.h"
#include "Screenshot.h"
#include "Image.h"


/*
* This class is used to manage the thread stack only for the scanner object
* It allow to create a new thread to parse the screen more efficiently and add it to the thread stack
* All the coordinate found by the thread will be added to the coordinates list and retrivable by the GetCoordinates() function
*/
class ScannerThreadStackHandler
{
public:
	// Constructor
	ScannerThreadStackHandler();
	ScannerThreadStackHandler(uint maxThread);

	void WaitForAllThread();
	void AddNewThreadIfPossible(Scanner scanner, Image Haystack, Image Needle);
	inline std::list<coordinate2D> GetCoordinates() { return coordinates; }

private:
	void MakeNewThread(Scanner scanner, Image Haystack, Image Needle);
	void FinishSomeThread();

	std::vector<std::future<std::list<coordinate2D>>> results;
	std::list<coordinate2D> coordinates;
	uint maxThreads;
};
