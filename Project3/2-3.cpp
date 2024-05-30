#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <thread>
#include <mutex>

std::mutex print_mutex;
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
    std::lock_guard<std::mutex> lock(print_mutex);
    pid++;
    print_command(arr);
}

// Background 작업으로 실행될 함수
void background(const std::vector<std::string>& arr) {
    std::lock_guard<std::mutex> lock(print_mutex);
    pid++;
    print_command(arr);
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

        // BG와 FG 구분하기, BG일때 &를 붙이므로 명령어 평가를 위해 제거해줌
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
    std::string command = "gcd 78696 19332 ; sum 100000; echo ghi -p 2;";
    auto commands = parse(command);
    // commands 출력
    for (const auto& cmd : commands) {
        std::cout << cmd << std::endl;
    }
    exec(commands);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}