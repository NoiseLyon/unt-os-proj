#include <iostream>
#include <iomanip>
#include <random>
#include <climits>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct process {
	int id;
    int cycles;
    int memory;
    int waitTime;
};

struct processor {
	vector<process> procList;
	int totalCycleCount;
	int clockSpeed;
};

vector<process> processList;
vector<processor> processorList;

void generateProcesses()
{
	double curProcCycle, curProcMemory;
	int numProcs = 100;
	int i = 0;
	ofstream outputFile;

	// Sets Up Normal Distribution.
	default_random_engine generator(time(0));
	normal_distribution<double> cycles(25000000.0, 10000000.0);
	normal_distribution<double> memory(5000000.0, 4000000.0);

	// Attempt to Open File
	outputFile.open("processes.txt", ios::trunc );
	if (	outputFile.is_open()	)
	{
		for(i = 0; i < numProcs; i++)
		{
			process currentProcess;
			currentProcess.id = (1000 + i);
			outputFile << int(currentProcess.id) << " ";

	        while(1)
	        {
				// Generate Number of Cycles and Check If Positive
				curProcCycle = cycles(generator);
	            if (curProcCycle > 10.0 && curProcCycle < 50000000.0)
				{

					outputFile << int(curProcCycle) << " ";
					currentProcess.cycles = curProcCycle;
					break;
				}
	        }

			while(1)
	        {
				// Generate Number of Cycles and Check If Positive and Within Bound
				curProcMemory = memory(generator);
	            if (curProcMemory > 250.0 && curProcMemory < 5000000.0)
				{
					outputFile << int(curProcMemory) << "\n";
					currentProcess.memory = curProcMemory;
					break;
				}
	        }
	        processList.push_back(currentProcess);
		}
	}

	outputFile.close();
	unlink("processes.txt");
}

//generates 5 processors with different clock speeds
void generateProcessors()
{
	int cntr;

	for(cntr = 0; cntr < 2; cntr++)
	{
		processor currentProcessor;
		currentProcessor.totalCycleCount = 0;
		currentProcessor.clockSpeed = 2000;
		processorList.push_back(currentProcessor);
	}

	for(cntr = 0; cntr < 2; cntr++)
	{
		processor currentProcessor;
		currentProcessor.totalCycleCount = 0;
		currentProcessor.clockSpeed = 3000;
		processorList.push_back(currentProcessor);
	}

	for(cntr = 0; cntr < 1; cntr++)
	{
		processor currentProcessor;
		currentProcessor.totalCycleCount = 0;
		currentProcessor.clockSpeed = 4000;
		processorList.push_back(currentProcessor);
	}
}

//schedules processes to processors
//operates by SJF
void scheduleProcesses()
{
	//while ready queue is not empty
	while(!processList.empty())
	{
		vector<process>::iterator processListIterator;
		vector<processor>::iterator processorListIterator;
		int processChosenIndex, processChosenCycle = 1000000000;
		int processorChosenIndex, processorChosenCycleCount = 1000000000;

		//find processor with least load by cycle count
		for(processorListIterator = processorList.begin(); processorListIterator != processorList.end(); ++processorListIterator)
		{
			if((*processorListIterator).totalCycleCount < processorChosenCycleCount)
			{
				processorChosenCycleCount = (*processorListIterator).totalCycleCount;
				processorChosenIndex = (processorListIterator - processorList.begin());
			}
		}
		
		//find shortest job
		for(processListIterator = processList.begin(); processListIterator != processList.end(); ++processListIterator)
		{
			if((*processListIterator).cycles < processChosenCycle)
			{
				processChosenCycle = (*processListIterator).cycles;
				processChosenIndex = (processListIterator - processList.begin());
			}
		}


		//add job to processor queue
		(processList.at(processChosenIndex)).waitTime = (processorList.at(processorChosenIndex)).totalCycleCount;
		(processorList.at(processorChosenIndex).procList).push_back(processList.at(processChosenIndex));
		(processorList.at(processorChosenIndex).totalCycleCount) += (processList.at(processChosenIndex)).cycles;


		//remove job from ready queue
		processList.erase(processList.begin()+processChosenIndex);
	}
}

//schedules processes to processors
//operates by SJF
void scheduleNewProcesses()
{
	//while ready queue is not empty
	while(!processList.empty())
	{
		vector<process>::iterator processListIterator;
		vector<processor>::iterator processorListIterator;
		int processChosenIndex, processChosenCycle = 1000000000;
		int processorChosenIndex, processorChosenCycleCount = 1000000000, processorChosenSpeed = 1;

		//find processor with least load by cycle count
		for(processorListIterator = processorList.begin(); processorListIterator != processorList.end(); ++processorListIterator)
		{
			if(((*processorListIterator).totalCycleCount / (*processorListIterator).clockSpeed) < (processorChosenCycleCount / processorChosenSpeed))
			{
				processorChosenCycleCount = (*processorListIterator).totalCycleCount;
				processorChosenSpeed = (*processorListIterator).clockSpeed;
				processorChosenIndex = (processorListIterator - processorList.begin());
			}
		}
		
		//check which processor has the lowest load
		if(processorChosenIndex > 3)
		{
			processChosenCycle = 1;
			
			//find longest job
			for(processListIterator = processList.begin(); processListIterator != processList.end(); ++processListIterator)
			{
				if((*processListIterator).cycles > processChosenCycle)
				{
					processChosenCycle = (*processListIterator).cycles;
					processChosenIndex = (processListIterator - processList.begin());
				}
			}
		}
		else
		{
			//find shortest job
			for(processListIterator = processList.begin(); processListIterator != processList.end(); ++processListIterator)
			{
				if((*processListIterator).cycles < processChosenCycle)
				{
					processChosenCycle = (*processListIterator).cycles;
					processChosenIndex = (processListIterator - processList.begin());
				}
			}
		}


		//add job to processor queue
		(processList.at(processChosenIndex)).waitTime = (processorList.at(processorChosenIndex)).totalCycleCount;
		(processorList.at(processorChosenIndex).procList).push_back(processList.at(processChosenIndex));
		(processorList.at(processorChosenIndex).totalCycleCount) += (processList.at(processChosenIndex)).cycles;


		//remove job from ready queue
		processList.erase(processList.begin()+processChosenIndex);
	}
}

//prints the process queues of each processor
void printProcessorQueues()
{
	vector<processor>::iterator processorListIterator;
	vector<process>::iterator processListIterator;
	unsigned long long int totalWaitTime = 0;	//used to hold total wait time

	cout << endl;

	//iterate through each processor
	for(processorListIterator = processorList.begin(); processorListIterator != processorList.end(); ++processorListIterator)
	{
		cout << "Current processor #: " << (processorListIterator - processorList.begin()) << endl << endl;
		cout << "Clock Speed: " << (*processorListIterator).clockSpeed << endl;
		cout << "Processor Total Cycle Count: " << (*processorListIterator).totalCycleCount << endl;
		cout << "Processor Turn Around Time: " << (*processorListIterator).totalCycleCount / (*processorListIterator).clockSpeed << " (Million Seconds)" << endl;
		cout << "Process List" << endl;
		cout << "ID               Cycles               Memory               Wait Time" << endl;

		//iterate through each processor process queue
		for(processListIterator = ((*processorListIterator).procList).begin(); processListIterator != ((*processorListIterator).procList).end(); ++processListIterator)
		{
			totalWaitTime += (*processListIterator).waitTime / (*processorListIterator).clockSpeed;	//add time to total wait time
			cout << setw(17) << setiosflags(ios::left) << (*processListIterator).id << setw(21) << (*processListIterator).cycles << setw(21) << (*processListIterator).memory << (*processListIterator).waitTime << endl;
		}

		cout << endl << endl;
	}

	cout << "Total Wait Time: " << totalWaitTime << " (Million Seconds)" << endl;
	cout << "Average Wait Time: " << (totalWaitTime / 100) << " (Million Seconds)" << endl;
}

//print menu of program
void printProcessorMenu()
{
	cout << "Scheduler Menu" << endl;
	cout << "1. Normal Shortest Job First" << endl;
	cout << "2. Modified Processor Schedule" << endl;
	cout << "Which Scheduling Mode would you like to use: ";
}

int main( int argc, char* argv[] )
{
	int choice;
	bool valid;

	generateProcesses();

	do
	{
		printProcessorMenu();

		if(!(std::cin >> choice))
		{
			std::cout << "PLEASE ENTER A VALID OPTION" << std::endl;
			valid = false;
		}
		
		else
		{
			if(choice > 0 && choice < 3)
				valid = true;
			
			else
			{
				std::cout << "PLEASE ENTER A VALID OPTION" << std::endl;
				valid = false;
			}

		}
		
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
	}while(!valid);

	generateProcessors();
	
	if(choice == 1)
	{
		scheduleProcesses();
		printProcessorQueues();
	}

	else
	{
		scheduleNewProcesses();
		printProcessorQueues();
	}

	return 0;        
}
