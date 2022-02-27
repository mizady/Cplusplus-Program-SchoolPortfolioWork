#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>

// This csv parser comes from https://github.com/AriaFallah/csv-parser under MIT License
#include "parser.hpp"

using namespace std;

// Loop bit
static bool _shouldLoop = true;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();

	PyObject* my_function = PyObject_GetAttrString(my_module, procname);

	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}

	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

/// <summary>
/// Print the menu so the user knows the options.
/// </summary>
void PrintMenu()
{
	cout << endl << "1: Print grocery item frequency" << endl;
	cout << "2: Print frequency of specific item" << endl;
	cout << "3: Print grocery item histogram" << endl;
	cout << "4: Exit" << endl;
	cout << "Enter your selection as a number 1, 2, 3, or 4." << endl;
}

/// <summary>
/// Opens the csv file and then for each field it splits by the space character and replaces the number with that number of *'s
/// </summary>
void ProcessCsvFileToList()
{
	ifstream data("D:/frequency.dat");
	aria::csv::CsvParser parser(data);

	for (auto& row : parser)
	{
		for (auto& field : row) 
		{
			cout << field.substr(0, field.find(" ")) << " " << string(stoi(field.substr(field.find(" ") + 1)), '*') << endl;
		}
	}
	parser.end();
	data.close();
	
}

/// <summary>
/// Process user selection and get input depending on menu option selected
/// </summary>
/// <param name="input">Menu option selected</param>
/// <param name="shouldLoop">reference to main input loop bit</param>
void ProcessInput(char input, bool& shouldLoop)
{
	string item = "";

	switch (input)
	{
	case 1:
		CallProcedure("PrintFrequency");
		break;
	case 2:
		cout << "Enter item to count: " << endl;
		cin >> item;
		cout << callIntFunc("CountItem", item) << endl;
		break;
	case 3:
		CallProcedure("WriteFrequencyFile");
		ProcessCsvFileToList();
		break;
	case 4:
		shouldLoop = false;
		break;
	default:
		cout << "Invalid entry, please enter 1, 2, or 3." << endl;
		break;
	}
}

/// <summary>
/// Gets character input based on key state and whether the previous check had the same button (used in conjunction with thread sleep for a side effect of not having too many presses too quickly or "debounce")
/// </summary>
void GetUserInput()
{
	int input;
	cin >> input;
	ProcessInput(input, _shouldLoop);
}

int main()
{
	/// <summary>
	/// Loop until user wishes to exit.
	/// </summary>
	while (_shouldLoop)
	{
		PrintMenu();
		GetUserInput();
	}

	return 0;
}