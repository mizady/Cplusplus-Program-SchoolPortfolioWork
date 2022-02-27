import re
import string
import collections

# Reads a file, counts the occurences of each item and creates a dictionary entry for each. we then print the dictionary
def PrintFrequency():
    print(dict(GetFrequencyList()));

# Reads a file into a dictionary with each entry being the quantity of each item in the file, we then find the item the user wants and print it.
def CountItem(item):
    return GetFrequencyList().get(item, 0);

# Reads a file into a dictionary with each entry being the quantity of each item in the file, then print replacing the number with a character n times.
def WriteFrequencyFile():
    tempDict = dict(GetFrequencyList()).items
    with open("D:\\frequency.dat", "w") as outputfile:
        for item, quantity in tempDict():
            outputfile.write(item + " " + str(quantity) + ",")
    
# opens the input file and reads the data into a list
def GetFrequencyList():
    with open("D:\Input_File.txt") as itemFile:
        List = itemFile.read().splitlines();
    return collections.Counter(List);