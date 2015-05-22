/*
 Sami Badra
 
 May 7, 2015
 Quora Challenge (Schedule)
 FILE: schedule.cpp
 
 Challenege description may be found in this GitHub Repository,
 or by visiting www.quora.com/challenges
 
 Copyright (c) 2015 Sami Badra. All rights reserved.
 
 DISCLAIMER: Any unauthorized use, including but not limited to, copying or
 redistributing any chunk of the source code (or an entire file) will result in
 punishment by law. I, Sami Badra,own all rights to the files and their contents.
 
 This source code has already been submitted to Quora, so it is in your best
 interest NOT to copy ANY of the source code you see here. Quora checks for code
 similarities from previously submitted version.You will be noticed and punished.
 */

#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

int numTests;       //number of tests to run
int *order;         //optimal order of tests
int *timeNeeded;    //time needed for each test
float *passRate;    //probability of each test passing

/* Prompts user for input needed to simulate running tests */
void promptUserInput() {
    numTests=0;
    cout << "How many tests to run?" << endl;
    cin >> numTests;
    while (numTests > 100 || numTests < 1) {
        cout << "Enter a number between 1 - 100" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> numTests;
    }
    
    order = (int *)calloc(numTests, sizeof(int));
    timeNeeded = (int *)calloc(numTests, sizeof(int));
    passRate = (float *)calloc(numTests, sizeof(float));
    
    for (int i=0; i<numTests; i++) {        //get info for each test
        cout << "Test " << i+1 << " info? (time/ pass rate)" << endl;
        cin >> timeNeeded[i] >> passRate[i];
        while (timeNeeded[i] > 100 || timeNeeded[i] < 1 || passRate[i] > 1 || passRate[i] < 0) {
            cout << endl;
            if (timeNeeded[i] > 100 || timeNeeded[i] < 1)
                cout << "**Choose a time between 1-100" << endl;
            if (passRate[i] > 1 || passRate[i] < 0)
                cout << "**Choose a pass rate between 0-1" << endl;
            cout << "Re-enter Test " << i+1 << " info: (time/ pass rate)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> timeNeeded[i] >> passRate[i];
        }
    }
}

/* Determines optimal order by calculating which tests have higher rate of failing per second */
void determineOrder() {
    float failPerSec[numTests];        //chance of a test failing per second
    for (int i=0; i < numTests; i++)
        failPerSec[i] = (1-passRate[i])/((float)timeNeeded[i]);
    
    sort(failPerSec, failPerSec + numTests, greater<float>());   //sort array high to low
    for (int i=0; i < numTests; i++)        //loop through failPerSec[]
        for (int j=0; j < numTests; j++)    //loop to match failPerSec[i] with corresponding test#
            if (failPerSec[i] == (1-passRate[j])/((float)timeNeeded[j])) {
                order[i] = j;
                break;
            }
}

/* Determines the average expected time before 'getting “go or no go” feedback' */
float runSimulation() {
    int timeRan = 0;            //time already passed during simulation
    float timeExpected = 0;     //average expected time to receive feedback
    float chanceToRun = 1;      //% chance that a test will run (starts at 100%)
    for (int i=0; i < numTests; i++) {
        timeRan += timeNeeded[order[i]];
        timeExpected += timeRan * chanceToRun * (1-passRate[order[i]]);    //expected time for test 'i' to run
        chanceToRun -= (chanceToRun * (1 - passRate[order[i]]));  //decrement by the %chance of last test failing
    }
    timeExpected += timeRan * chanceToRun;    //expected time if all tests succeeded
    return timeExpected;
}

int main() {
    promptUserInput();
    determineOrder();
    float timeExpected = runSimulation();
    cout << endl << "Expected Time: " << timeExpected << endl;
    return 0;
}
