//
// Created by RSgty on 25-12-24.
//
#include <iostream>
#include <cstring>
#include <utility>
#include <vector>

using namespace std;

int isSelected[26]{};
int Correspond[26];
int n;
int** AddTable;

typedef struct log{
    vector<char> Letters;
    vector<int> numbers;
    int data;
    struct log* next;
    struct log* parent;
} Logger;

Logger Origin;

string strA, strB, strO;

void Initialize() {
    cin >> n;
    memset(Correspond, -1, sizeof(Correspond));
    Origin.data = -1; Origin.parent = nullptr; Origin.next = nullptr;
    AddTable = new int*[n];
    for (int i = 0; i < n; i++) {
        AddTable[i] = new int[n];
        for (int j = 0; j < n; j++) {
            AddTable[i][j] = (i + j) % n;
        }
    }
}

Logger* AddLog(vector<char> newLetters, vector<int> newNumbers) {
    for (int i = 0; i < newLetters.size(); i++) {
        Correspond[newLetters[i]-'A'] = newNumbers[i];
        isSelected[newNumbers[i]] = 1;
    }

    Logger* temp = &Origin;
    Logger* current = &Origin;
    while (current) {
        current = current->next;
    }
    Logger* newLog = new Logger;
    current = newLog;
    current->Letters = std::move(newLetters);
    current->numbers = std::move(newNumbers);
    current->data = -1;
    current->parent = current;
    return newLog;
}

void DelLog(Logger* current) {
    if (current == nullptr) {
        return;
    }
    for (int i = 0; i < current->Letters.size(); i++) {
        isSelected[current->numbers[i]] = 0;
        Correspond[current->Letters[i]-'A'] = -1;
    }
    Logger* temp = current;
    Logger* par = current->parent;
    par->next = nullptr;
    delete temp;
}

bool Operation(int len, int extend){
    if (len == 0) {
        if (extend){return false;}
        return true;
    }
    char lA, lB, lO;
    lA = strA[len-1]; lB = strB[len-1]; lO = strO[len-1];
    bool isABIdentical = lA == lB;
    int AC = Correspond[lA-'A'] == -1, BC = Correspond[lB-'A'] == -1, OC = Correspond[lO-'A'] == -1;
    if (AC + BC == 0){
        if (OC) {
            int temp = (Correspond[lA-'A'] + Correspond[lB-'A'] + extend) % n;
            if (isSelected[temp]){return false;}
            Logger* logs1 = AddLog({lO}, {temp});
            int ex = (Correspond[lA-'A'] + Correspond[lB-'A'] + extend) / n;
            if (Operation(len-1, ex)) {
                delete logs1;
                return true;
            } else {
                DelLog(logs1);
            }
        } else {
            int temp = (Correspond[lA-'A'] + Correspond[lB-'A'] + extend) % n;
            if (temp != Correspond[lO-'A']){return false;}
            return (Operation(len-1, (Correspond[lA-'A'] + Correspond[lB-'A'] + extend) / n));
        }
    }
    else if (AC + BC == 1) {
        char moving = Correspond[lA-'A'] == -1? lA : lB;
        char unmoving = Correspond[lA-'A'] == -1? lB : lA;
        for (int i = n; i >= 0; i --) {
            if (!isSelected[i]) {
                vector<char> tempLetters = {moving};
                vector<int> tempNumbers = {i};
                Logger* logs1 = AddLog(tempLetters, tempNumbers);
                Logger* logs2 = nullptr;
                OC = Correspond[lO-'A'] == -1;
                if (OC) {
                    if (!isSelected[(i + Correspond[unmoving-'A'] + extend)%n]) {
                        logs2 = AddLog({lO}, {(i + Correspond[unmoving-'A'] + extend)%n});
                    } else {
                        DelLog(logs1);
                        continue;
                    }
                }
                else {if (Correspond[lO-'A'] != (i + Correspond[unmoving-'A'] + extend)%n){DelLog(logs1); continue;}}

                if (Operation(len-1, (i + Correspond[unmoving-'A'] + extend)/n)) {
                    if (logs2){delete logs2;}
                    delete logs1;
                    return true;
                } else {
                    DelLog(logs2);
                    DelLog(logs1);
                }
            }
        }
    }
    else if (AC + BC == 2) {
        for (int i = n; i >= 0; i --) {
            for (int j = n; j >= 0; j --) {
                if ((i != j ^ isABIdentical) && (!isSelected[i] && !isSelected[j])) {
                    vector<char> tempLetters = {lA, lB};
                    vector<int> tempNumbers = {i, j};
                    Logger* logs1 = AddLog(tempLetters, tempNumbers);
                    Logger* logs2 = nullptr;
                    OC = Correspond[lO-'A'] == -1;
                    if (OC) {
                        if (!isSelected[(i + j + extend)%n]) {
                            logs2 = AddLog({lO}, {(i + j + extend)%n});
                        } else {
                            DelLog(logs1);
                            continue;
                        }
                    }
                    else {if (Correspond[lO-'A'] != (i + j + extend)%n){DelLog(logs1); continue;}}


                    if (Operation(len-1, (i + j + extend)/n)) {
                        if (logs2){delete logs2;}
                        delete logs1;
                        return true;
                    } else {
                        DelLog(logs2);
                        DelLog(logs1);
                    }
                }
            }
        }
    }
    return false;
}

/*
int main(){
    Initialize();

    char ret = getchar();
    cin>>strA>>strB>>strO;

    Operation(strA.size(), 0);


    for (int i = 0; i < n; i++) {
        cout << Correspond[i] << ' ';
    }

    return 0;
}
*/