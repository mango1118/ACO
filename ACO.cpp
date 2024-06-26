#include <iomanip>
#include "ACO.h"
#include "AntColony.h"

#include "Axis.h"

//---------- txt ----------//
void writeInFile(const string &fileName, const string &data) {
    ofstream outfile;
    // 使用 ios::app 模式打开文件，实现追加写入
    outfile.open(fileName, ios::app);
    // 检查文件是否成功打开
    if (!outfile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }
    // 写入数据并换行
    outfile << data << endl;
    // 关闭文件
    outfile.close();
}

void cleanFile(const string &fileName) {
    ofstream file(fileName, ofstream::out | ofstream::trunc);
    file.close();
//    cout << "文件内容已清空\n";
}

void writeTimeInFile(const std::string &fileName) {

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
    auto start = std::chrono::high_resolution_clock::now();
    cleanFile(THIS_TIME_RECORD_NAME);
    writeInFile(RECORD_NAME, "\n");
    writeTimeInFile(RECORD_NAME);
    writeTimeInFile(THIS_TIME_RECORD_NAME);
    string temp_write = "path select method: " + to_string(PATH_SELECTED_METHOD);
    writeInFile(RECORD_NAME, temp_write);
    AntColony antColony;
    antColony.init();
    antColony.run();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double seconds = duration.count();
    writeInFile(RECORD_NAME, "run time: " + to_string(seconds));
    writeInFile(THIS_TIME_RECORD_NAME, "run time: " + to_string(seconds));
    cout << seconds << endl;
    writeInFile(RECORD_NAME, "--------------- over ---------------");
    writeInFile(THIS_TIME_RECORD_NAME, "--------------- over ---------------");
    getch();

}
