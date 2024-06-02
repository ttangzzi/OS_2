﻿#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <thread>
#include <mutex>
#include <windows.h>
#include <fstream>

using namespace std;
using namespace chrono;
mutex print_mutex;
int pid;

/* 공백을 제거하는 함수 */
void trim(string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    auto end = str.find_last_not_of(" \t\n\r\f\v");

    if (start == string::npos) {
        str = ""; // 문자열이 공백만 있는 경우
    }
    else {
        str = str.substr(start, end - start + 1);
    }
}

/* 명령어를 파싱하는 함수 (;) */
std::vector<string> parse(const std::string& command) {
    vector<string> tokens;
    stringstream ss(command);
    string item;

    while (getline(ss, item, ';')) {
        trim(item);
        if (!item.empty()) {
            tokens.push_back(item);
        }
    }

    return tokens;
}

/* 유클리드 호제법 */
int gcd(int a, int b)
{
    int c;
    while (b != 0)
    {
        c = a % b;
        a = b;
        b = c;
    }
    return a;
}

/* 에라토스테네스의 체 알고리즘 */
int Eratos(int n) {
    std::vector<bool> isPrime(n + 1, true);
    int count = 0;

    if (n >= 0) isPrime[0] = false;
    if (n >= 1) isPrime[1] = false;

    for (int i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // 소수 개수 세기
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) ++count;
    }

    return count;
}

/* sum 구현 */
int sum(int n)
{
    int sum = 0;
    for (int i = 0; i <= n; i++)
    {
        sum += i;
    }
    return sum % 1000000;
}

/* 명령어에 맞는 출력 */
void print_command(const std::vector<std::string>& arr) {
    if (arr[0] == "echo" || arr[0] == "&echo") {
        std::cout << arr[1] << "\n";
    }
    else if (arr[0] == "dummy" || arr[0] == "&dummy") {

    }
    else if (arr[0] == "gcd" || arr[0] == "&gcd") {
        int x = std::stoi(arr[1]);
        int y = std::stoi(arr[2]);
        std::cout << gcd(x, y) << "\n";
    }
    else if (arr[0] == "prime" || arr[0] == "&prime") {
        int x = std::stoi(arr[1]);
        std::cout << Eratos(x) << "\n";
    }
    else if (arr[0] == "sum" || arr[0] == "&sum") {
        int x = std::stoi(arr[1]);
        std::cout << sum(x) << "\n";
    }
    else {
        std::cout << "error\n";
    }
}

// Foreground 작업으로 실행될 함수
void foreground(const std::vector<std::string>& arr) {
    int p = 0, d = 200, n = 1;
    for (int i = 0; i < size(arr); i++) {
        // 예외처리 : i가 arr의 크기 -1 보다 작을때만 되도록 (i+1 때 오버플로 방지)
        if (i < size(arr) - 1) {
            if (arr[i] == "-p") {
                p = stoi(arr[i + 1]);
            }
            else if (arr[i] == "-d") {
                d = stoi(arr[i + 1]);
            }
            else if (arr[i] == "-n") {
                n = stoi(arr[i + 1]);
            }
        }
    }
    auto start = steady_clock::now(); // 시작 시간 기록
    auto end = start + seconds(d);   // 종료 시간 설정

    for (int j = 0; j < n; j++) {
        if (steady_clock::now() >= end) { // 현재 시간이 종료 시간보다 이전인 동안 반복
            break;
        }
        std::this_thread::sleep_for(seconds(p)); // p초 동안 대기
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            pid++;
            print_command(arr);
        }
    }
}

// Background 작업으로 실행될 함수
void background(const std::vector<std::string>& arr) {

    // 기본값 설정 > p = 0초마다 반복 / d = 200초 넘으면 종료 / n = 1개 생성
    int p = 0, d = 200, n = 1;

    for (int i = 0; i < size(arr); i++) {
        // 예외처리 : i가 arr의 크기 -1 보다 작을때만 되도록 (i+1 때 오버플로 방지)
        if (i < size(arr) - 1) {
            if (arr[i] == "-p") {
                p = stoi(arr[i + 1]);
            }
            else if (arr[i] == "-d") {
                d = stoi(arr[i + 1]);
            }
            else if (arr[i] == "-n") {
                n = stoi(arr[i + 1]);
            }
            else if ((arr[0] == "sum" || arr[0] == "&sum") && arr[i] == "-m") {

            }
        }
    }
    auto start = steady_clock::now(); // 시작 시간 기록
    auto end = start + seconds(d);   // 종료 시간 설정

    for (int j = 0; j < n; j++) {
        if (steady_clock::now() >= end) { // 현재 시간이 종료 시간보다 이전인 동안 반복
            break;
        }
        std::this_thread::sleep_for(seconds(p)); // p초 동안 대기

        /* 해당 부분만 lock을 해야 전체대기를 막을 수 있다 */
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            pid++;
            print_command(arr);
        }
    }
}

/* 명령어를 실행하는 함수 */
void exec(const vector<string>& commands) {

    for (const auto& command : commands) {
        std::stringstream ss(command);
        std::string token;
        std::vector<std::string> arr;


        // 명령어들을 동적배열에 할당한다.
        while (ss >> token) {
            arr.push_back(token);
        }

        if ((arr.front()).front() == '&') {
            std::thread bgThread(background, arr);
            // join을 하면 독립 시행이 아니므로 detach한다.
            bgThread.detach();
        }
    }


    for (const auto& command : commands) {
        std::stringstream ss(command);
        std::string token;
        std::vector<std::string> arr;


        // 명령어들을 동적배열에 할당한다.
        while (ss >> token) {
            arr.push_back(token);
        }

        if ((arr.front()).front() != '&') {
            foreground(arr);
        }   
    }
}

int main() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    string currentDirectory = string(buffer).substr(0, pos);
    //cout << "Current working directory: " << currentDirectory << endl;

    ifstream file(currentDirectory + "\\commands.txt");
    string command;
    vector<string> parseCommands;

    if (!file.is_open()) {
        cout << "File not found" << endl;
        return 1;
    }
    else {
        /* 한 줄 단위로 실행하기 */
        while (getline(file, command)) {
            parseCommands = parse(command);
            for (const auto& cmd : parseCommands) {
                std::cout << cmd << std::endl;
            }
            exec(parseCommands);
            std::this_thread::sleep_for(seconds(5));
        }
    }
    file.close();
    
    return 0;
}