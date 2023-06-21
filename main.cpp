#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <cctype>
#include <chrono>
using namespace std;
// to upper case function to convert the string to upper case
string toUpperCase(const string &str)
{
    string result = str;
    for (char &c : result)
    {
        c = toupper(c);
    }
    return result;
}
// to lower case  function to convert the string to lower case
string toLowerCase(const string &str)
{
    string result = str;
    for (char &c : result)
    {
        c = tolower(c);
    }
    return result;
}
// substring function to get the substring from a string with a given delimiter and position of the substring;
string subStringer(const string &str, char delim, int pos)
{
    int start = 0;
    int end = str.find(delim);
    vector<string> tokens;
    while (end != string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    tokens.push_back(str.substr(start));
    if (pos >= 0 && pos < tokens.size())
    {
        return tokens[pos];
    }
    else
    {
        return ""; // Return an empty string if pos is out of range
    }
}

// Check if the given item ID already exists in the file
bool isItemIdExists(const string &item_id)
{
    ifstream inputFile("items.csv");
    if (inputFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            string existingItemId = subStringer(line, ',', 0);
            if (existingItemId == item_id)
            {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
    }
    return false;
}

// Check if the given item name already exists in the file
bool isItemNameExists(const string &item_name)
{
    ifstream inputFile("items.csv");
    if (inputFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            string existingItemName = subStringer(line, ',', 1);
            if (existingItemName == item_name)
            {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
    }
    return false;
}

// Add item function
void addItem(string item_id, string item_name, string quantity, string registration_date)
{
    // Check if the item ID or item name already exists
    if (isItemIdExists(item_id))
    {
        cout << "Item ID already exists." << endl;
        return;
    }

    if (isItemNameExists(item_name))
    {
        cout << "Item name already exists." << endl;
        return;
    }
    // Check if the quantity is a number
    try
    {
        int quantityInt = stoi(quantity);
    }
    catch (const std::exception &e)
    {
        cout << "Invalid quantity." << endl;
        return;
    }
    // check if id is a number
    try
    {
        int idInt = stoi(item_id);
    }
    catch (const std::exception &e)
    {
        cout << "Invalid id, please enter a number" << endl;
        return;
    }
    // Check if item_name is a string (contains alphabetic characters only)
    for (char c : item_name)
    {
        if (!isalpha(c))
        {
            cout << "Invalid item name. Please enter a string." << endl;
            return;
        }
    }
    int quantityInt = stoi(quantity);
    if (quantityInt < 0)
    {
        cout << "Invalid quantity. Please enter a non-negative integer." << endl;
        return;
    }

    // Validate registration date using regex
    regex dateRegex("\\d{4}-\\d{2}-\\d{2}");
    if (!regex_match(registration_date, dateRegex))
    {
        cout << "Invalid registration date format. Correct format: YYYY-MM-DD." << endl;
        return;
    }

    // Parse the registration date components
    int year = stoi(subStringer(registration_date, '-', 0));
    int month = stoi(subStringer(registration_date, '-', 1));
    int day = stoi(subStringer(registration_date, '-', 2));

    // Get the current date
    time_t currentTime = time(nullptr);
    tm *currentTm = localtime(&currentTime);

    // Check if entered year, month, or day is not greater than the current year, month, or day
    if (year > currentTm->tm_year + 1900)
    {
        cout << "Invalid year entered." << endl;
        return;
    }
    if (year == currentTm->tm_year + 1900 && month > currentTm->tm_mon + 1)
    {
        cout << "Invalid month entered." << endl;
        return;
    }
    if (year == currentTm->tm_year + 1900 && month == currentTm->tm_mon + 1 && day > currentTm->tm_mday)
    {
        cout << "Invalid day entered." << endl;
        return;
    }

    // Open the file
    ofstream MyFile("items.csv", ios::app);
    // Check if the file is open
    if (MyFile.is_open())
    {
        // Write to the file
        MyFile << item_id << "," << item_name << "," << quantity << "," << registration_date << endl;
        // Close the file
        MyFile.close();
        // Return the success message
        cout << "Item added successfully" << endl;
    }
    else
    {
        // Return the error message
        cout << "Unable to open the file" << endl;
    }
}

bool compareItemNames(const string &item1, const string &item2)
{
    string itemName1 = subStringer(item1, ',', 1);
    string itemName2 = subStringer(item2, ',', 1);
    transform(itemName1.begin(), itemName1.end(), itemName1.begin(), ::tolower);
    transform(itemName2.begin(), itemName2.end(), itemName2.begin(), ::tolower);
    return itemName1 < itemName2;
}

void listItems()
{
    ifstream inputFile("items.csv");
    if (inputFile.is_open())
    {
        string line;
        vector<string> items;
        while (getline(inputFile, line))
        {
            items.push_back(line);
        }

        sort(items.begin(), items.end(), compareItemNames);

        for (const auto &item : items)
        {
            string itemId = subStringer(item, ',', 0);
            string itemName = subStringer(item, ',', 1);
            string quantity = subStringer(item, ',', 2);
            string regDate = subStringer(item, ',', 3);

            cout << "Item ID:" << itemId << "\t Item Name:" << itemName
                 << "\t Quantity :" << quantity << "\t Reg Date :" << regDate << endl;
        }

        inputFile.close();
    }
    else
    {
        cout << "Unable to open the file" << endl;
    }
}

int main()
{
    // command variable to store the user input
    string command;

    vector<string> tokens;

    // Main program loop
    while (true)
    {
        // Print console prompt
        cout << "console>";

        // Read user input
        getline(cin, command);
        // if command is help we are going to print the help menu to the console window
        if (toLowerCase(command) == "help")
        {
            cout << "--------------------------------------------------------\n"
                 << "*              Command syntaxes                            *\n"
                 << "------------------------------------------------------------*\n"
                 << "               itemadd <item_id> <item_name> <quantity> <registration_date>\n"
                 << "               itemlist\n";
        }
        // if command is itemadd
        else if (subStringer(toLowerCase(command), ' ', 0) == "itemadd")
        {
            // get item_id,item_name,quantity,registration_date
            string item_id = subStringer(command, ' ', 1);
            string item_name = subStringer(command, ' ', 2);
            string quantity = subStringer(command, ' ', 3);
            string registration_date = subStringer(command, ' ', 4);
            if (subStringer(command, ' ', 5) != "" || item_id == "" || item_name == "" || quantity == "" || registration_date == "")
            {
                cout << "invalid command, format must be itemadd <item_id> <item_name> <quantity> <registration_date>" << endl;
                continue;
            }
            addItem(item_id, item_name, quantity, registration_date);
        }

        // if commad is itemlist
        else if (toLowerCase(command) == "itemslist")
        {
            listItems();
        }
        // Exit program if command is "exit"
        else if (toLowerCase(command) == "exit")
        {
            cout << "Exiting Inventory System." << endl;
            break;
        }
        else
        {
            cout << "Invalid command enterd" << endl;
        }
    }
    return 0;
}