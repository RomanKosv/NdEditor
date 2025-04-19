#include "triangulation.h"
#include <vector>

using namespace Triangulation;

int main(int argc, char *argv[]) {
    std::vector<Halfspace3d> cube{{{1, 0, 0}, 1}, {{-1, 0, 0}, 1}, {{0, 1, 0}, 1}, {{0, -1, 0}, 1}, {{0 ,0 ,1}, 1}, {{0, 0, -1}, 1}};
    auto segs = toPoints(cube);
    for(auto v : segs) {
        for(auto s : v){
            LOG<<"[Segments set (form toPoints)]" << str(s) << " : ";
        }
        LOG<<"||||\n";
    }
    auto tr = triangulate/*<true>*/(segs);
    for(Triangle i : tr) {
        LOG << str(i) <<'\n';
    }
}
