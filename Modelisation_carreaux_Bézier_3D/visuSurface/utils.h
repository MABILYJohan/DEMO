#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <point.h>

using namespace std;

class Utils
{
public:
    Utils();

    static int factorielle(int nb);
    static int randInt(int low, int high);

    static vector<vector<Point>> list_to_tab(std::vector<Point> liste, int n, int m, int begin);
    static std::vector<Point> tab_to_list(vector<vector<Point>> tab, int n, int m);
};

#endif // UTILS_H
