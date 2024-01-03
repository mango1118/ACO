#include <iomanip>
#include "ACO.h"
#include "AntColony.h"

//---------- txt ----------//
void WriteInFile(const string &fileName, const string &data) {
    ofstream outfile;
    outfile.open(fileName);
    outfile << data << endl;
    outfile.close();
}

void WriteTimeInFile(const std::string &fileName) {
    // 获取当前时间
    std::time_t currentTime = std::time(nullptr);
    std::tm *localTime = std::localtime(&currentTime);
    // 打开文件并追加写入
    std::ofstream outputFile(fileName, std::ios::app);
    if (outputFile.is_open()) {
        // 写入当前时间到文件
        outputFile << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << std::endl;
        // 关闭文件
        outputFile.close();
        std::cout << "Current time has been written to " << fileName << std::endl;
    } else {
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
}

int main() {
    WriteTimeInFile(RECORD_NAME);
    AntColony antColony;
    antColony.init();
    antColony.run();
}
