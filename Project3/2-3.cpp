#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <thread>
#include <mutex>

using namespace std;
using namespace chrono;
mutex print_mutex;
int pid;

// 공백을 제거하는 함수
void trim(std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    auto end = str.find_last_not_of(" \t\n\r\f\v");

    if (start == std::string::npos) {
        str = ""; // 문자열이 공백만 있는 경우
    }
    else {
        str = str.substr(start, end - start + 1);
    }
}

// 명령어를 파싱하는 함수
std::vector<std::string> parse(const std::string& command) {
    std::vector<std::string> tokens;
    std::stringstream ss(command);
    std::string item;

    while (std::getline(ss, item, ';')) {
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

/* ~체 알고리즘 */
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

// 명령어를 실행하는 함수
void exec(const std::vector<std::string>& commands) {
    for (const auto& command : commands) {
        std::stringstream ss(command);
        std::string token;
        std::vector<std::string> arr;

        while (ss >> token) {
            arr.push_back(token);
        }

        if ((arr.front()).front() == '&') {
            std::thread bgThread(background, arr);
            // join을 하면 독립 시행이 아니므로 detach한다.
            bgThread.detach();
        }
        else {
            foreground(arr);
        }
    }
}

int main() {
    std::string command = "&gcd 78696 19332 -p 12; sum 100000 -p 10 -n 2; echo ghi;";
    auto commands = parse(command);
    // commands 출력
    for (const auto& cmd : commands) {
        std::cout << cmd << std::endl;
    }
    exec(commands);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}