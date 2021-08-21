#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int main()
{
    string DeadGhostsReturnFile("eyes_movement.txt");
    ifstream fin(DeadGhostsReturnFile);
    ofstream fout("DeadGhostsReturnFile.bin",ios::binary);
    if (!fin)
    {
        cerr << "Unable to open file " << DeadGhostsReturnFile << endl;
        exit(3);
    }
    unsigned temp, f1, f2, f3, f4;
cout << setfill('0');
int count = 0;
    while (fin >> f1 >> f2 >> f3 >> f4)
    {
        temp = (f1 << 24) + (f2 << 16) + (f3 << 8) + f4;
        cout << dec << ++count << ' ' << temp << "  " << sizeof(temp) << ' ' << setw(8) << hex << temp << endl;
       // if (f2 == 24) break;
        fout.write(reinterpret_cast<const char*>(&temp),sizeof(temp));
    }
    fout.close();
}
