// eya-attendance.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <filesystem>
#include "csv.h"
#include "person.h"

bool ValidDateFormat(std::string date)
{
    int d, m, y;
    std::istringstream is(date);
    char delimiter;
    if (is >> m >> delimiter >> d >> delimiter >> y) {
        struct tm t = { 0 };
        t.tm_mday = d;
        t.tm_mon = m - 1;
        t.tm_year = y - 1900;
        t.tm_isdst = -1;

        // normalize:
        time_t when = mktime(&t);
        const struct tm* norm = localtime(&when);
        // the actual date would be:
        // m = norm->tm_mon + 1;
        // d = norm->tm_mday;
        // y = norm->tm_year;
        // e.g. 29/02/2013 would become 01/03/2013

        // validate (is the normalized date still the same?):
        if (norm->tm_mday == d &&
            norm->tm_mon == m - 1 &&
            norm->tm_year == y - 1900 &&
            norm->tm_wday == 0)
        {
            // Valid date and Sunday
            return true;
        }
    }
    
    // Invalid date or not a Sunday
    return false;
}

bool IsValidCSV(const std::string& filePath)
{
    // Check if the file exists
    if (!std::filesystem::exists(filePath)) {
        std::cout << "File does not exist: " << filePath << std::endl;
        return false;
    }

    // Check if the file has a .csv extension
    if (std::filesystem::path(filePath).extension() != ".csv") {
        std::cout << "File is not a CSV file: " << filePath << std::endl;
        return false;
    }

    // Additional checks specific to CSV format can be added if necessary

    // If both checks pass, consider it a valid CSV file
    return true;
}

 bool GetHeaderLine(const std::string& filePath, std::string& headerLine)
{
    // Open the file for reading
    std::ifstream inFile;
    inFile.open(filePath);

    // Get the header row (first row to the newline)
    if (!inFile.good())
    {
        return false;
    }
    std::getline(inFile, headerLine, '\n');

    // Close the file
    inFile.close();

    return true;;
}

bool TokenizeHeaderRow(const std::string& headerLine, std::vector<std::string>& headers)
{
    // Tokenize the header line
    std::istringstream split(headerLine);

    // Push each field into the headers array
    for (std::string each; std::getline(split, each, ','); headers.push_back(each));

    // Make sure we have basic data, sizes, and known fields
    if (!headers.empty() && headers.size() > 3 && headers[0] == "first name" && headers[1] == "last name" && headers[2] == "percent")
    {
        // Erase all the headers we don't care about, keep the name, and sundays
        auto erased = std::erase_if(headers, [](std::string s) {
            if (s == "first name" || s == "last name" || /*s == "percent" ||*/ ValidDateFormat(s))
                return false;
            else
                return true;
            });
        //TODO: Look for duplicates

        return true;
    }
    else
    {
        return false;
    }
}

bool CreateClassRollVector(const std::string& filePath, const std::vector<std::string>& headers, const uint32_t actualNumHeaders, std::vector<person>& classRoll)
{
    try
    {
        // Create the csv reader with the file's path
        io::CSVReader<100> in(filePath);

        // Read the header row, this tells the csv parser all our headers .. since we're doing this dynamically we have to have 100 spots
        in.read_header(io::ignore_extra_column | io::ignore_missing_column,
            headers[0], headers[1], headers[2], headers[3], headers[4], headers[5], headers[6], headers[7], headers[8], headers[9], headers[10], headers[11], headers[12], headers[13], headers[14], headers[15], headers[16], headers[17], headers[18], headers[19], headers[20], headers[21], headers[22], headers[23], headers[24], headers[25], headers[26], headers[27], headers[28], headers[29], headers[30], headers[31], headers[32], headers[33], headers[34], headers[35], headers[36], headers[37], headers[38], headers[39], headers[40], headers[41], headers[42], headers[43], headers[44], headers[45], headers[46], headers[47], headers[48], headers[49], headers[50], headers[51], headers[52], headers[53], headers[54], headers[55], headers[56], headers[57], headers[58], headers[59], headers[60], headers[61], headers[62], headers[63], headers[64], headers[65], headers[66], headers[67], headers[68], headers[69], headers[70], headers[71], headers[72], headers[73], headers[74], headers[75], headers[76], headers[77], headers[78], headers[79], headers[80], headers[81], headers[82], headers[83], headers[84], headers[85], headers[86], headers[87], headers[88], headers[89], headers[90], headers[91], headers[92], headers[93], headers[94], headers[95], headers[96], headers[97], headers[98], headers[99]
        );

        std::vector<std::string> data;
        data.resize(100);

        while (in.read_row(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15], data[16], data[17], data[18], data[19], data[20], data[21], data[22], data[23], data[24], data[25], data[26], data[27], data[28], data[29], data[30], data[31], data[32], data[33], data[34], data[35], data[36], data[37], data[38], data[39], data[40], data[41], data[42], data[43], data[44], data[45], data[46], data[47], data[48], data[49], data[50], data[51], data[52], data[53], data[54], data[55], data[56], data[57], data[58], data[59], data[60], data[61], data[62], data[63], data[64], data[65], data[66], data[67], data[68], data[69], data[70], data[71], data[72], data[73], data[74], data[75], data[76], data[77], data[78], data[79], data[80], data[81], data[82], data[83], data[84], data[85], data[86], data[87], data[88], data[89], data[90], data[91], data[92], data[93], data[94], data[95], data[96], data[97], data[98], data[99]
        ))
        {
            person tmpPerson;
            tmpPerson.first_name = data[0];
            tmpPerson.last_name = data[1];
            //tmpPerson.percent = data[2];

            person::MemberType memberType{ person::MemberType::NA };
            for (uint32_t i = 2; i < actualNumHeaders; ++i)
            {
                person::AttendanceType attendanceType{ person::AttendanceType::NA };

                if (data[i] == "membership removed")
                {
                    attendanceType = person::AttendanceType::NA;
                    //memberType = person::MemberType::NA;
                }
                else if (data[i] == "attendance not taken")
                {
                    attendanceType = person::AttendanceType::NOT_TAKEN;
                }
                else if (data[i] == "attended as member")
                {
                    attendanceType = person::AttendanceType::PRESENT;
                    memberType = person::MemberType::MEMBER;
                }
                else if (data[i] == "attended as leader")
                {
                    attendanceType = person::AttendanceType::PRESENT;
                    memberType = person::MemberType::LEADER;
                }
                else if (data[i] == "attended as visitor")
                {
                    attendanceType = person::AttendanceType::VISITING;
                    memberType = person::MemberType::VISITOR;
                }
                else if (data[i] == "")
                {
                    attendanceType = person::AttendanceType::NOT_PRESENT;
                }

                tmpPerson.member_type = memberType;
                tmpPerson.attendance_list.push_back({ headers[i], attendanceType });
            }

            classRoll.emplace_back(tmpPerson);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool CountAbsentWeeks(std::vector<person>& classRoll)
{
    for (auto& member : classRoll)
    {
        uint32_t WeeksAbsent{ 99 };
        for (auto& Attendance : member.attendance_list)
        {
            if (Attendance.type == person::AttendanceType::PRESENT ||
                Attendance.type == person::AttendanceType::VISITING)
            {
                if (!member.seen)
                    member.seen = true;
                WeeksAbsent = 0;
            }
            else if (Attendance.type == person::AttendanceType::NOT_PRESENT)
            {
                if (member.seen)
                    WeeksAbsent++;
            }
            else if (Attendance.type == person::AttendanceType::NA)
            {
                member.seen = false;
                WeeksAbsent = 99;
            }
            Attendance.weeks_absent = WeeksAbsent;
        }
    }
    return true;
}

bool OutputDataToFile(const std::vector<std::string>& headers, const std::vector<person>& classRoll, const uint32_t actualNumHeaders)
{
    // Output to file
    std::ofstream outFile;
    outFile.open("report.csv");
    
    if (!outFile.good())
    {
        return false;
    }

    // Output the headers
    for (uint32_t i = 0; i < actualNumHeaders; ++i)
    {
        outFile << headers[i] << ",";

        if (i == 1)
            outFile << "member type,action,";

    }

    outFile << "\n";

    // For each member
    for (auto& member : classRoll)
    {
        // Output first/last name
        outFile << member.first_name << ",";
        outFile << member.last_name << ",";
        
        // Based on the LAST week's abscent count output a special action
        switch (member.member_type)
        {
        case person::MemberType::NA:
            outFile << "N/A";
            break;
        case person::MemberType::MEMBER:
            outFile << "Member";
            break;
        case person::MemberType::LEADER:
            outFile << "Leader";
            break;
        case person::MemberType::VISITOR:
            outFile << "Visitor";
            break;
        default:
            outFile << "";
        };

        outFile << ",";

        // Based on the LAST week's abscent count output a special action
        switch (member.attendance_list[member.attendance_list.size() - 1].weeks_absent)
        {
        case 2:
            outFile << "Post Card";
            break;
        case 3:
            outFile << "Text";
            break;
        case 4:
            outFile << "Phone Call";
            break;
        case 5:
            outFile << "Visit";
            break;
        default:
            outFile << "";
        };

        outFile << ",";

        // Output all the absent weeks, this should match the number of actual weeks..
        for (auto& Attendance : member.attendance_list)
        {
            outFile << Attendance.weeks_absent << ",";
        }

        outFile << "\n";
    }

    outFile.close();

    return true;
}

void PrintMessageAndWait(const std::string& msg)
{
    // Don't close the window immediately
    std::cout << msg << std::endl << std::endl;
    std::cout << "Press Enter to continue...";

    // Use std::getline to wait for the user to press Enter
    std::string dummy;
    std::getline(std::cin, dummy);
}

int main(int argc, char* argv[])
{
    // Arguments.. need to pass in a Planning Center attendance export (drag-drop works)
    if (argc == 1)
    {
        PrintMessageAndWait("Please include a valid Planning Center attendance .csv export");
        return -1;
    }
    if (argc > 2)
    {
        PrintMessageAndWait("Please include a valid Planning Center attendance .csv export");
        return -2;
    }

    // Basic validation of the input file
    const std::string inputFileString{ argv[1] };
    if (!IsValidCSV(inputFileString))
    {
        PrintMessageAndWait("Failed doing basic validation on input file\nPlease provide a valid Planning Center attendnace .csv export");
        return -3;
    }

    // Open the file and grab the header line
    std::string headerLine{};
    if (!GetHeaderLine(inputFileString, headerLine))
    {
        PrintMessageAndWait("Failed opening input file to grab header row");
        return -4;
    }
    
    // Tokenize the header row, do basic validation like only have sundays, and save the headers in a vector
    std::vector<std::string> headers;
    if (!TokenizeHeaderRow(headerLine, headers))
    {
        PrintMessageAndWait("Failed tokenizing the header row");
        return -5;
    }

    // Save the number of headers we have
    uint32_t actualNumHeaders{ static_cast<uint32_t>(headers.size()) };

    // Need to resize the headers vector to the number of columns we're going to be looking for
    headers.resize(100);

    // Using a csv reader (csv.h) create a class roll with the defined headers
    std::vector<person> classRoll;
    if (!CreateClassRollVector(inputFileString, headers, actualNumHeaders, classRoll))
    {
        PrintMessageAndWait("Failed to create a class roll, most likely due to the CSV parser throwing an exception");
        return -6;
    }

    // For each member count the number of absent weeks for each given date based on the roll, stores the data in the roll
    if (!CountAbsentWeeks(classRoll))
    {
        PrintMessageAndWait("Failed to count the number of absent weeks");
        return -7;
    }

    // We need to resize back to the actual number of headers for output
    // TODO: This is gross
    headers.resize(actualNumHeaders);

    // Output the data to a csv file
    if (!OutputDataToFile(headers, classRoll, actualNumHeaders))
    {
        PrintMessageAndWait("Failed creating an output report file");
        return -8;
    }

    // Don't close the window immediately
    PrintMessageAndWait("Report created successfuly");

    return 0;
}
