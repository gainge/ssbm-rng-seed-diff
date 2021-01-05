//
//  main.cpp
//  ssbm-seed-tool
//
//  Created by Gibson Ainge on 1/4/21.
//

#include <iostream>
#include <algorithm>

using namespace std;

// Implementation of LCG by Savestate
void rng_adv(uint32_t *seed) {
    *seed = ((*seed * 214013) + 2531011);
}




bool isOffset(string str) {
    return str[0] == '-' || str[0] == '+';
}

bool isQuit(string str) {
    return str[0] == 'x' || str[0] == 'X';
}

bool charInRange(char c, char low, char high) {
    return c >= low && c <= high;
}


bool isValidHex(string str) {
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        
        if (isspace(c)) continue;
        
        if (!charInRange(c, '0', '9') &&
            !charInRange(c, 'A', 'F') &&
            !charInRange(c, 'a', 'f') &&
            c != 'x' && c != 'X' &&
            c != '-' && c != '+')
        {
            return false;
        }
    }
    
    return true;
}

string removeSpaces(string str) {
    int i = 0;
    int j = 0;
    
    while (j < str.length()) {
        if (str[j] != ' ') {
            // Copy over char
            str[i] = str[j];
            i++;
        }
        j++;
    }
    
    // Null terminate string
    str.erase(i);
    
    return str;
}

string getUserHex(string prompt) {
    string input;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Remove spaces
        input = removeSpaces(input);
        
        // Validate input
        if (input.length() == 0 || !isValidHex(input)) {
            printf("\n\n!---- Please Input a Valid Hex Number (x to quit) ----!\n\n");
        } else {
            break;
        }
    }
    
    return input;
}


uint32_t calcSeedOffset(uint32_t seed, int offset) {
    if (offset >= 0) {
        // Standard seed advance
        for (int i = 0; i < offset; i++) {
            rng_adv(&seed);
        }
        
        return seed;
    } else {
        // Gotta count from the bottom, yikes
        cout << "(this will take a while)" << endl;
        uint32_t trailSeed = seed;
        
        // Search for the offset using a delayed pointer
        for (long i = 0; i < 0x100000000 - abs(offset); i++) {
            rng_adv(&trailSeed);
        }
        
        return trailSeed;
    }
}

int findSeedDifference(uint32_t start, uint32_t target) {
    if (start == target) return 0;
    
    uint32_t seedFromStart = start;
    uint32_t seedFromTarget = target;
    uint32_t rollCount = 0;
    
    do {
        rng_adv(&seedFromStart);
        rng_adv(&seedFromTarget);
        rollCount++;
        
    } while (seedFromStart != target && seedFromTarget != start);
    
    // Check for standard advancement or wraparound
    if (seedFromTarget == start) {
        return -1 * rollCount;
    }
    
    return rollCount;
}


int main(int argc, const char * argv[]) {
    cout << "=======================" << endl;
    cout << "SSBM RNG Seed Diff Calc" << endl;
    cout << "=======================" << endl;
    
    string userInput;
    
    while (true) {
        cout << endl;
        cout << "Please supply seeds (x to quit)" << endl;
        userInput = getUserHex("Please input first seed: ");
        
        // Check flags
        if (isQuit(userInput)) break;
        
        // Convert user input as hex
        uint32_t firstSeed = stoul(userInput, 0, 16);
        
        // Grab second seed
        userInput = getUserHex("Please input second seed or offset: ");
        
        if (isQuit(userInput)) break;
        
        // Check for offset
        if (isOffset(userInput)) {
            // Parse input as a base 10 offset
            int offset = stoi(userInput);
            
            cout << "\nFinding seed..." << endl;
            
            uint32_t offsetSeed = calcSeedOffset(firstSeed, offset);
            
            cout << "\n0x" << hex << firstSeed << dec << (offset < 0 ? " - " : " + ") << abs(offset) << " => 0x" << hex << offsetSeed << dec << endl;
            
        } else {
            // Perform standard seed to seed diff calc
            uint32_t secondSeed = stoul(userInput, 0, 16);
            
            cout << "\n0x" << hex << firstSeed << " => 0x" << hex << secondSeed << dec << endl;
            
            int diff = findSeedDifference(firstSeed, secondSeed);
            cout << diff << endl;
        }
        
        cout << "========================================" << endl;
    }
    
    cout << "Goodbye!" << endl;
    
    return 0;
}
