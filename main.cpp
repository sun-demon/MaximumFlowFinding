#include "dinic.h"
#include <cmath>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct FileNotFoundException : runtime_error {
    explicit FileNotFoundException(const string & path)
            : runtime_error("File '" + path + "' does not found: can't be reading or writing") {}
};
struct MatrixNotFoundException : runtime_error {
    explicit MatrixNotFoundException(const string & arg)
            : runtime_error("Matrix not found exception: " + arg) {}
};
struct NotMatrixException : runtime_error {
    explicit NotMatrixException(const string & arg)
            : runtime_error("Not matrix exception: " + arg) {}
};


vector<vector<int>> readMatrix(const string & path) {
    ifstream fin(path);
    if (!fin)
        throw FileNotFoundException(path);
    vector<int> line;
    int buffer;
    while (fin >> buffer)
        line.push_back(buffer);
    fin.close();
    if (line.empty())
        throw MatrixNotFoundException("no numbers found");
    int n = int(round(sqrt(line.size())));
    if (line.size() != n * n)
        throw NotMatrixException("count of numbers must be a square of an integer");
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            matrix[i][j] = line[i * n + j];
    return matrix;
}

string getCurrentDateTime(const string& s) {
    time_t now = time(nullptr);
    struct tm timeStruct{};
    char  buf[80];
    timeStruct = *localtime(&now);
    if(s == "now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &timeStruct);
    else if(s == "date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &timeStruct);
    return buf;
}

void log(const char * filePath, const string& logMsg) {
    string now = getCurrentDateTime("now");
    ofstream ofs(filePath, std::ios_base::out | std::ios_base::app );
    if (!ofs.is_open())
        ofs = ofstream("errors.txt", std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}

void pressAnyKeyForContinue() noexcept {
    cout << "Press any key for continue..." << flush;
    getch();
}

int main(int argc, char * argv[]) {
    if (argc != 4) {
        cerr << "[Utility format]: BasicGraphAlgorithms input.txt output.txt error.txt";
        return 1;
    }

    try {
        auto matrix = readMatrix(argv[1]);
        ofstream fout(argv[2]);
        if (!fout.is_open())
            throw FileNotFoundException(argv[2]);
        int n = int(matrix.size());
        cout << "Enter source of flow (from 1 to " << n << "): " << flush;
        int from;
        cin >> from;
        if (from < 1 || from > n)
            throw range_error("source must be in range from 1 to " + to_string(n) + ", not " + to_string(from));
        --from;
        fout << "Source vertex: " << from + 1 << "\n";
        for (int to = 0; to < n; ++to) {
            if (to == from)
                continue;
            Dinic dinic(n, from, to);
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    if (i != j && matrix[i][j] != 0)
                        dinic.add_edge(i, j, matrix[i][j]);
            auto flow = dinic.flow();
            cout << "To: " << (to + 1) << ", flow: " << ((flow == dinic.flow_inf) ? "INF" : to_string(flow)) << "\n";
            fout << "Destination vertex: " << (to + 1) << ", flow: " << ((flow == dinic.flow_inf) ? "INF" : to_string(flow)) << "\n";
        }
        fout.close();
        pressAnyKeyForContinue();
    } catch (runtime_error & exc) {
        log(argv[3], exc.what());
        cout << exc.what() << endl;
        pressAnyKeyForContinue();
    }
    return 0;
}
