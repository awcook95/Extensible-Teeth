#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <string>
#include <map>
#include <algorithm>

using namespace std;

/* Function that reads the input file for the sizes and contents of the two arrays and ensures that the two arrays are the same size */
void importTeeth(string readFile, vector<int> &vect1, vector<int> &vect2) {
    string input1, input2, input3;

    ifstream rdFile(readFile);

    getline(rdFile, input1);
    getline(rdFile, input2);
    getline(rdFile, input3);

    stringstream iss1(input2);
    stringstream iss2(input3);

    int num;
    while(iss1 >> num)
        vect1.push_back(num);

    while(iss2 >> num)
        vect2.push_back(num);
 
    if(vect1.size() > vect2.size()) {
        int min = min_element(vect2.begin(), vect2.end()) - vect2.begin();
        while(vect1.size() != vect2.size()) {
            vect2.insert(vect2.begin()+min, vect2[min]);
        }
    } 
    else if (vect1.size() < vect2.size()) {
        int min = min_element(vect1.begin(), vect1.end()) - vect1.begin();
        while(vect1.size() != vect2.size()) {
            vect1.insert(vect1.begin()+min, vect1[min]); 
        }
    }

    rdFile.close();
}

/* function that writes the expanded arrays as well as the max height of the two sets of teeth into the output file */
void exportTeeth(vector<int> sol1, vector<int> sol2, string writeFile, int max) {
    std::ofstream wtFile(writeFile);
    
    wtFile << max << '\n';

    for(int i = 0; i < sol1.size(); i++) {
        wtFile << sol1[i] << " " << sol2[i] << endl;
    }

    wtFile.close();
}

/* Function takes in the altered 2d vector and traces the path with the lowest height that was calculated by the teeth algorithm. This will eventually be passed the memoized structure instead of the original input vector */
vector<vector<int>> findPath(vector<vector<int>> input, int i, int j, int max, vector<int> &sol1, vector<int> &sol2, map<int, int> m, map<int, int> n) {
    if(i == input.size() - 1 || j == input[0].size() - 1) {
        if(i == input.size() - 1 && j == input[0].size() - 1) {
            sol1.push_back(m[i]);
            sol2.push_back(n[j]);
            return input;
        }
        else if(i == input.size() - 1) {
            if(input[i][j + 1] <= max) {
                sol1.push_back(m[i]);
                sol2.push_back(n[j]);
                return findPath(input, i, j + 1, max, sol1, sol2, m, n);
            }
        }
        else if(j == input[0].size() - 1) {
            if(input[i + 1][j] <= max) {
                sol1.push_back(m[i]);
                sol2.push_back(n[j]);
                return findPath(input, i + 1, j, max, sol1, sol2, m, n);
            } 
        }
    }
    if(input[i + 1][j + 1] <= max) {
        sol1.push_back(m[i]);
        sol2.push_back(n[j]);
        return findPath(input, i + 1, j + 1, max, sol1, sol2, m , n);
    }
    else if(input[i + 1][j] <= max) {
        sol1.push_back(m[i]);
        sol2.push_back(n[j]);
        return findPath(input, i + 1, j, max, sol1, sol2, m, n);
    }
    else if(input[i][j + 1] <= max) {
        sol1.push_back(m[i]);
        sol2.push_back(n[j]);
        return findPath(input, i, j + 1, max, sol1, sol2, m, n);
    }

    return input;
}

/* Function that traverses all possible paths, filling the memoized data structure with the best path, and returns the minimum height of the two sets of teeth */
int alignTeeth(vector<vector<int>> &input, int i, int j, vector<vector<int>> &memVec) {
    if(memVec[i][j] != -1){
        input[i][j] = memVec[i][j];
    }
    else if (i == input.size() - 1 && j == input.size() - 1){
        memVec[i][j] = input[i][j];
    }
    else if(i + 1 > input.size() - 1) {
        input[i][j] = max(input[i][j], alignTeeth(input, i, j + 1, memVec));
        memVec[i][j] = input[i][j];
    }
    else if(j + 1 > input.size() - 1) {
        input[i][j] = max(input[i][j], alignTeeth(input, i + 1, j, memVec));
        memVec[i][j] = input[i][j];  
    }
    else {
        int o1 = max(input[i][j], alignTeeth(input, i + 1, j + 1, memVec));
        int o2 = max(input[i][j], alignTeeth(input, i, j + 1, memVec));
        int o3 = max(input[i][j], alignTeeth(input, i + 1, j, memVec));

        input[i][j] = min(o1, min(o2, o3));
        memVec[i][j] = input[i][j];
    }
    return input[i][j];
}

int main () {
    vector<vector<int>> input, memVec;
    vector<int> vect1, vect2, sol1, sol2, temp;
    map<int, int> m, n;

    importTeeth("input.txt", vect1, vect2);

    for(int i = 0; i < vect1.size(); i++) {
        input.push_back(temp);
        memVec.push_back(temp);
        for(int j = 0; j < vect2.size(); j++) {
            m[i] = vect1[i];
            n[j] = vect2[j];
            input[i].push_back(vect1[i] + vect2[j]);
            memVec[i].push_back(-1);
        }
    }

    int max = alignTeeth(input, 0, 0, memVec);

    vector<vector<int>> ret = findPath(input, 0, 0, max, sol1, sol2, m , n);

    exportTeeth(sol1, sol2, "output.txt", max);
}
