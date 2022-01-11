#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;
/*
* Class: timeReport
* Main class to create Time Tracking Report
* The main idea is: We are parsing all file and find
* Name and Date of every line - that will be our key in map data
* Value will be INT of Hours, every time we found a key in InputFileLines
* we add hours from this line to Tota Hours in map data
* We have separatorCSV, which is ';' as default,
* numberOfProcessedEntires to keep number of processed lines, which is 1 as default,
* becouse first line of file is column names
* We have posOfName, posOfMonth and posOfHOurs, if we want to change form of
* input data, we need to point out where will be this columns
* Also, we have a getMonthName, just easily way to convert number of month
* to name of month in fastest way, using map
*/
class timeReport {
    char separatorCSV = ';';
    map<vector<string>, int> data;
    int numberOfProcessedEntries = 1;
    int posOfName = 0, posOfMonth = 6, posOfHours = 7; 

    string getMonthName(int numberOfMonth) {
        const map<int, string> months
        {
            { 1, "January"},
            { 2, "February"},
            { 3, "March"},
            { 4, "April"},
            { 5, "May"},
            { 6, "June"},
            { 7, "July"},
            { 8, "August"},
            { 9, "September"},
            { 10, "October"},
            { 11, "November"},
            { 12, "December"}
        };
        const auto it = months.find(numberOfMonth);
        if(it != months.cend()) return it->second;
        else {
            throw "Date format error!";
            return "Error";
        }
    }

public:
    timeReport() = default;

    timeReport(char separatorCSV) {
        this->separatorCSV = separatorCSV;
    }
    /*
    * Function: parse
    * Function that parse throught out the all line of file,
    * and save info in data map.
    * line - line of file
    * lineStream - stream of line to have a opportunite to iterate all columns
    * column - column of line
    * counter - counter for index of each column to find Name, Date and Hours
    * tempData - vector to temporarily save Name and Date in output format of the line.
    */
    void parse(string filePath) {
        ifstream inputFile(filePath);
        
        if (!inputFile.is_open()) throw "InputFile reading error!";
        else {
            string line;
            getline(inputFile, line);

            while (getline(inputFile, line)) {
                stringstream lineStream;
                lineStream << line;
                string column;
                int counter = 0;
                vector<string> tempData;

                while (getline(lineStream, column, separatorCSV)) {
                    if (counter == posOfName) {
                        tempData.push_back(column);
                    }
                    if (counter == posOfMonth) {
                        string currentMonth = getMonthName(stoi(column.substr(5, 2)));
                        string currentYear = column.substr(0, 4);
                        tempData.push_back(currentMonth);
                        tempData.push_back(currentYear);
                    }
                    if (counter == posOfHours) {
                        data[tempData] += stoi(column);
                    }
                    counter++;
                }
                numberOfProcessedEntries++;
                tempData.clear();
                lineStream.clear();
            }
        }
        inputFile.close();
    }
    /*
    * Function: getNumberOfProcessedEntries
    * We have an variable that count every line with data in file
    * Return: numberOfProcessedEntries
    */
    int getNumberOfProcessedEntries() {
        return numberOfProcessedEntries;
    }
    /*
    * Function: getTimeReportInConsole
    * Create a TimeReport from data map, and output it in Console,
    * iterating over all elements of map.
    */
    void getTimeReportInConsole() {
        cout << "Name" << separatorCSV << "Month" << separatorCSV << "Total hours" << endl;
        for (auto it = data.cbegin(); it != data.cend(); ++it) {
            cout << it->first[0] << separatorCSV << it->first[1] << " " << it->first[2] << separatorCSV << it->second << endl;
        }
    }
    /*
    * Function: getTimeReportInFile
    * Create a TimeReport from data map, and save it to file in resultPath
    */
    void getTimeReportInFile(string resultPath) {
        ofstream outFile(resultPath);
        if (!outFile.is_open()) throw "The OutFile path does not exist, or the OutFile could not be created!";
        else {
            outFile << "Name" << separatorCSV << "Month" << separatorCSV << "Total hours" << endl;
            for (auto it = data.cbegin(); it != data.cend(); ++it) {
                outFile << it->first[0] << separatorCSV << it->first[1] << " " << it->first[2] << separatorCSV << it->second << endl;
            }
        }
        outFile.close();
    }
};

int main()
{
    /*
    *   An example of using the class   
    *   We have the input in console, try-catch block for handling exceptions
    *   that occur in timeReport class
    *   Also, we have simple measurer of parsing time
    */

    cout << "Enter a InputFilePath: ";
    string inputFilePath;
    cin >> inputFilePath;
    cout << "Enter a separator (Type 'D' to set separator as default): ";
    char separatorCVS;
    cin >> separatorCVS;
    if (separatorCVS == 'D') separatorCVS = ';';
    cout << "Enter a OutFilePath (Type 'no' if you dont want to have a result in file): ";
    string outputFilePath;
    cin >> outputFilePath;
    cout << "\n";

    try {
        clock_t start = clock();

        timeReport file(separatorCVS);
        file.parse(inputFilePath);
        file.getTimeReportInConsole();

        clock_t end = clock();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;

        cout << "\nNumber of processed enteries: " << file.getNumberOfProcessedEntries() << endl;
        cout << "Parsing time: " << seconds << " seconds" << endl << endl;

        if (outputFilePath != "no") file.getTimeReportInFile(outputFilePath);
    }
    catch (const char* exception) {
        cerr << "Error: " << exception << '\n';
    }

    system("pause");
    return 0;
}