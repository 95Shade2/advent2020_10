#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>    //sort
#include <map>
#include <iomanip>  //setprecision

using namespace std;

int Int(string num) {
    int num_i = 0;

    while (num.size() > 0) {
        num_i *= 10;
        num_i += num[0] - '0';
        num = num.substr(1);
    }

    return num_i;
}

vector<int> File_To_Int_Vec(string filename) {
    vector<int> lines_int;
    string line;
    ifstream file;

    file.open(filename.c_str());

    getline(file, line);
    while (file) {
        lines_int.push_back(Int(line));
        getline(file, line);
    }

    file.close();

    return lines_int;
}

void Get_Jolts(vector<int> adapters, int *j_1, int *j_3) {
    int a_1;
    int a_2;
    int diff;

    //initial adapter to the wall
    a_1 = adapters[0];
    if (a_1 == 1) {
        (*j_1)++;
    }
    else if (a_1 == 3) {
        (*j_3)++;
    }

    for (int a = 0; a < adapters.size()-1; a++) {
        a_1 = adapters[a];
        a_2 = adapters[a+1];
        diff = a_2 - a_1;

        if (diff == 1) {
            (*j_1)++;
        }
        else if (diff == 3) {
            (*j_3)++;
        }
    }

    //last adapter to the pc (always 3)
    (*j_3)++;

}

void Get_Connections(vector<int> adapters, map<int, vector<int>> *adapters_connections) {
    int a_1;
    int a_2;
    int diff;

    //the last adapter always connects to the pc, so it has no other connections
    for (int a = 0; a < adapters.size()-1; a++) {
        vector<int> connections;
        a_1 = adapters[a];

        for (int a2 = a+1; a2 < adapters.size(); a2++) {
            a_2 = adapters[a2];
            diff = a_2 - a_1;

            if (diff <= 3) {
                connections.push_back(a2);
            }
            else {
                break;
            }
        }

        (*adapters_connections)[a] = connections;
    }
}

double Calculate(vector<int> adapters, map<int, vector<int>> adapters_connections) {
    double ways;
    vector<int> connections;
    map<int, double> sub_counts;
    double sum;

    for (int a = adapters.size()-1; a >= 0; a--) {
        connections = adapters_connections[a];
        sum = 0;

        if (connections.size() > 0) {
            for (int c = 0; c < connections.size(); c++) {
                int connect = connections[c];
                sum += sub_counts[connect];
            }

            sub_counts[a] = sum;
        }
        else {
            sub_counts[a] = 1; //0 total sub connections (end of adapters)
        }
    }

    ways = sub_counts[0];

    return ways;
}

void Print(vector<int> adapters, map<int, vector<int>> adapters_connections) {
    for (int i = 0; i < adapters_connections.size(); i++) {
        vector<int> connections = adapters_connections[i];

        cout << adapters[i] << "\n\t";
        for (int a = 0; a < connections.size(); a++) {
            cout << adapters[connections[a]] << " ";
        }
        cout << endl << endl;
    }
}

vector<int> Push_Front(vector<int> adapters, int value) {
    vector<int> new_vec;

    new_vec.push_back(value);

    for (int a = 0; a < adapters.size(); a++) {
        new_vec.push_back(adapters[a]);
    }

    return new_vec;
}

int main()
{
    vector<int> adapters;
    int jolt_1 = 0;
    int jolt_3 = 0;
    map<int, vector<int>> adapters_connections; //index if adapter => list of adapters indexes it can connect to
    double ways;

    adapters = File_To_Int_Vec("adapters.txt");

    sort(adapters.begin(), adapters.end());

    Get_Jolts(adapters, &jolt_1, &jolt_3);

    cout << jolt_1 << " * " << jolt_3 << " = " << jolt_1 * jolt_3 << endl;

    //part 2

    //fix the start of the list, it doesnt include 0 - X
    adapters = Push_Front(adapters, 0);

    Get_Connections(adapters, &adapters_connections);

    //Print(adapters, adapters_connections);

    ways = Calculate(adapters, adapters_connections);

    cout << "There are " << fixed << setprecision(0) << ways << " different ways" << endl;

    return 0;
}
