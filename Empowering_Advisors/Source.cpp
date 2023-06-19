//============================================================================
// Name        : Source.cpp
// Author      : Brandon Petersen
// Description : Project Two: Vector Sorting Course Information
//============================================================================

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// Forward declarations
double strToDouble(const std::string& str, char ch);

// Defining the structure to hold course information
struct Course {
    std::string courseID;
    std::string title;
    std::string prerequisite;
};

// Static methods used for testing
void displayCourse(const Course& course) {
    std::cout << course.courseID << ": " << course.title << " | " << course.prerequisite << std::endl;
}

// Prompting user for course information using the console
Course getCourse() {
    Course course;

    std::cout << "Enter courseID: ";
    std::cin.ignore();
    std::getline(std::cin, course.courseID);

    std::cout << "Enter title: ";
    std::getline(std::cin, course.title);

    std::cout << "Enter prerequisite: ";
    std::getline(std::cin, course.prerequisite);

    return course;
}

// Loading CSV file
std::vector<Course> loadCourses(const std::string& csvPath) {
    std::cout << "Loading CSV file: " << csvPath << std::endl;

    std::vector<Course> courses;

    std::ifstream file(csvPath);
    if (!file) {
        throw std::runtime_error("Failed to open the file: " + csvPath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string courseId, title, prerequisite;

        // Assuming CSV format: courseId,title,prerequisite
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        if (pos1 == std::string::npos || pos2 == std::string::npos) {
            throw std::runtime_error("Invalid CSV format in file: " + csvPath);
        }

        courseId = line.substr(0, pos1);
        title = line.substr(pos1 + 1, pos2 - pos1 - 1);
        prerequisite = line.substr(pos2 + 1);

        Course course{ courseId, title, prerequisite };
        courses.push_back(course);
    }

    file.close();

    return courses;
}

// Implementing quicksort functionality over course.title
int partition(std::vector<Course>& courses, int begin, int end) {
    int low = begin;
    int high = end;

    // Middle element being set as the pivot point for the quicksort
    int pivot = begin + (end - begin) / 2;

    bool done = false;
    while (!done) {
        // Increment low index while courses[low].title < courses[pivot].title
        while (courses.at(low).title.compare(courses.at(pivot).title) < 0) {
            ++low;
        }

        // Decrement high index while courses[pivot].title < courses[high].title
        while (courses.at(pivot).title.compare(courses.at(high).title) < 0) {
            --high;
        }

        if (low >= high) {
            done = true;
        }
        else {
            // Swap the high and low courses using built-in vector methods
            std::swap(courses.at(low), courses.at(high));
        }
    }

    return high;
}

// Performing a quicksort on course title
void quickSort(std::vector<Course>& courses, int begin, int end) {
    int mid = 0;

    if (begin >= end) {
        return;
    }

    mid = partition(courses, begin, end);
    quickSort(courses, begin, mid);
    quickSort(courses, mid + 1, end);
}

void printMenu() {
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Load Data Structure" << std::endl;
    std::cout << "2. Print Course List" << std::endl;
    std::cout << "3. Print Course" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void printCourseList(const std::vector<Course>& courses) {
    for (const auto& course : courses) {
        displayCourse(course);
    }
    std::cout << std::endl;
}

void searchCourse(const std::vector<Course>& courses) {
    std::string courseID;
    std::cout << "Enter course ID: ";
    std::cin >> courseID;

    bool courseFound = false;
    for (const auto& course : courses) {
        if (course.courseID == courseID) {
            displayCourse(course);
            courseFound = true;
            break;
        }
    }

    if (!courseFound) {
        std::cout << "Course ID " << courseID << " not found." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string csvPath;
    const int EXIT_CHOICE = 4;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "Course_Information.csv";
    }

    std::vector<Course> courseInformation;
    clock_t ticks;

    int choice = 0;
    while (choice != EXIT_CHOICE) {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            try {
                ticks = clock();

                courseInformation = loadCourses(csvPath);

                std::cout << courseInformation.size() << " course(s) loaded" << std::endl;

                ticks = clock() - ticks;
                std::cout << "time: " << ticks << " clock ticks" << std::endl;
                std::cout << "time: " << static_cast<double>(ticks) / CLOCKS_PER_SEC << " seconds" << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading courses: " << e.what() << std::endl;
            }
            break;

        case 2:
            printCourseList(courseInformation);
            break;

        case 3:
            try {
                ticks = clock();

                quickSort(courseInformation, 0, courseInformation.size() - 1);

                std::cout << courseInformation.size() << " course(s) read" << std::endl;

                searchCourse(courseInformation);

                ticks = clock() - ticks;
                std::cout << "time: " << ticks << " clock ticks" << std::endl;
                std::cout << "time: " << static_cast<double>(ticks) / CLOCKS_PER_SEC << " seconds" << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error sorting or searching courses: " << e.what() << std::endl;
            }
            break;
        }
    }

    std::cout << "Goodbye!" << std::endl;
    return 0;
}