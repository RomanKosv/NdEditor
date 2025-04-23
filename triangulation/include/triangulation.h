#ifndef TRIANGULATION_H
#define TRIANGULATION_H
#include <limits>
#include <optional>
#include <qfloat16.h>
#include <string>
#include <QDebug>
#include <vector>
#include <variant>
// #include "basic_loggers.h"
#include "../../QtQuickProject/triangles.h"
#include <QTextStream>
#include "logger.h"

namespace Triangulation{
template<typename Log>
struct QAddOverview{
    Log log;
    QTextStream& in;
    string prefix = "\n##################################\nOverview:\n";
    string postfix = "\n##################################\n";
    ~QAddOverview() {
        // in.clear();
        // in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        log << "\nInput overview (one line):" <<'\n';
        string overview = in.readLine().toStdString();
        log << prefix << overview.data() << postfix;
    }
};


static QTextStream sout(stdout);
static QTextStream sin(stdin);
static FileLog save_log{R"(..\..\..\logs\triangulation\)", FileLog::simple_time_path()};
static Holder<TwoLogs<TimePref<AddPrefix<FileLog&>>, decltype(sout)&>> LOG{{{{save_log, "\n"}}, sout}};

using std::get;

typedef float Scalar;
typedef GraphicsTriangles::vec3 Vec3d;

typedef struct {
    Vec3d axis;
    Scalar less_than;
} Halfspace3d;

typedef GraphicsTriangles::triangle Triangle;

Scalar x(Vec3d v);
Scalar y(Vec3d v);
Scalar z(Vec3d v);
std::string str(Vec3d v);

std::string str(Triangle tr);

const Scalar P_INF = std::numeric_limits<Scalar>::max();
const Scalar N_INF = -std::numeric_limits<Scalar>::max();

Scalar scalar_mult(Vec3d v1, Vec3d v2);

Vec3d vec_mult(Vec3d v1, Vec3d v2);

Scalar len2(Vec3d v);

Scalar len(Vec3d v);

Vec3d scale(Vec3d v, Scalar s);

Vec3d sum(Vec3d a, Vec3d b);

struct Segment1d {
    Scalar left, right;
    std::optional<Segment1d> cutMore(Scalar cut);
    std::optional<Segment1d> cutLess(Scalar cut);
};

struct Segment3d {
    Vec3d point, vec;
    Segment1d limits{N_INF, P_INF};
    std::optional<Segment3d> cutLess(Vec3d axis, Scalar cut);
    std::optional<Vec3d> getA();
    std::optional<Vec3d> getB();
    static std::optional<Segment3d> cut(std::optional<Segment3d> seg, Halfspace3d cut);
};

std::string str(std::optional<Segment3d> segment);

enum IntersectionError {
    Similar,
    Else
};

std::variant<Segment3d, IntersectionError> intersection(Halfspace3d& h1, Halfspace3d& h2);

std::vector<std::vector<std::optional<Segment3d>>> toPoints(std::vector<Halfspace3d>& cuts);

// template<bool checkSquare = false>
std::vector<Triangle> triangulate(std::vector<std::vector<std::optional<Segment3d>>> segments);

}
#endif // TRIANGULATION_H
