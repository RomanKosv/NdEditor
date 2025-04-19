#ifndef TRIANGULATION_H
#define TRIANGULATION_H
#include <limits>
#include <optional>
#include <string>
#include <variant>
#include <tuple>
#include <cmath>
#include <vector>
#include "../logger/basic_loggers.h"
#include "../QtQuickProject/triangles.h"

namespace Triangulation{

static BaseLogSystem log_sys{R"(..\..\..\logs\triangulation\)"};
#define LOG log_sys.log
#define INP log_sys.input

using std::get;

typedef float Scalar;
typedef GraphicsTriangles::vec3 Vec3d;

typedef struct {
    Vec3d axis;
    Scalar less_than;
} Halfspace3d;

typedef GraphicsTriangles::triangle Triangle;

Scalar x(Vec3d v) {
    return v.a;
}
Scalar y(Vec3d v) {
    return v.b;
}
Scalar z(Vec3d v) {
    return v.c;
}
std::string str(Vec3d v) {
    std::string s = "(";
    s += std::to_string(x(v));
    s += ",";
    s += std::to_string(y(v));
    s += ",";
    s += std::to_string(z(v));
    s += ")";
    return s;
}

std::string str(Triangle tr) {
    return "{" + str(tr.a) + ";" + str(tr.b) + ";" + str(tr.c) + "}";
}

const Scalar P_INF = std::numeric_limits<Scalar>::max();
const Scalar N_INF = -std::numeric_limits<Scalar>::max();

Scalar scalar_mult(Vec3d v1, Vec3d v2) {
    return x(v1)*x(v2) + y(v1)*y(v2) + z(v1)*z(v2);
}
Vec3d vec_mult(Vec3d v1, Vec3d v2) {
    return Vec3d(y(v1)*z(v2) - z(v1)*y(v2), x(v1)*z(v2) - z(v1)*x(v2), x(v1)*y(v2) - y(v1)*x(v2));
}
Scalar len2(Vec3d v) {
    return scalar_mult(v, v);
}
Scalar len(Vec3d v) {
    return std::sqrt(len2(v));
}
Vec3d scale(Vec3d v, Scalar s) {
    return Vec3d(x(v)*s, y(v)*s, z(v)*s);
}
Vec3d sum(Vec3d a, Vec3d b) {
    return Vec3d(x(a)+x(b), y(a)+y(b), z(a)+z(b));
}

struct Segment1d {
    Scalar left, right;
    std::optional<Segment1d> cutMore(Scalar cut) {
        if (cut>right) {
            LOG << "[cutMore]segment deleted" << cut << "," << left << '\n';
            return std::nullopt;
        }
        else {
            LOG << cut << "," << left << "," << std::max(left, cut) << "...\n";
            return Segment1d{std::max(left, cut), right};
        }
    }
    std::optional<Segment1d> cutLess(Scalar cut) {
        if (cut < left) return std::nullopt;
        else return Segment1d{left, std::min(right, cut)};
    }
};

struct Segment3d {
    Vec3d point, vec;
    Segment1d limits{N_INF, P_INF};
    std::optional<Segment3d> cutLess(Vec3d axis, Scalar cut) {
        Scalar cos = scalar_mult(axis, vec);
        if (cos == 0) {
            LOG <<" cos = 0\n";
            if (scalar_mult(axis, point) > cut) return std::nullopt;
            else return *this;
        }
        else {
            LOG << "cos is" <<cos << '\n';
            Scalar point_c = scalar_mult(axis, point);
            Scalar point_d = cut - point_c;
            LOG <<"point _d is"<< point_d <<'\n';
            Scalar cut_on_line = point_d / cos;
            std::optional<Segment1d> newlim;
            if (cos > 0) {
                newlim = limits.cutLess(cut_on_line);
            }
            else {
                newlim = limits.cutMore(cut_on_line);
                LOG << "cut more call finished\n";
            }
            if(newlim.has_value()) return Segment3d{point, vec, newlim.value()};
            else return std::nullopt;
        }
    }
    std::optional<Vec3d> getA() {
        if (limits.left != N_INF) return sum(point, scale(vec, limits.left));
        else return std::nullopt;
    }
    std::optional<Vec3d> getB() {
        if (limits.right != P_INF) return sum(point, scale(vec, limits.right));
        else return std::nullopt;
    }
    static std::optional<Segment3d> cut(std::optional<Segment3d> seg, Halfspace3d cut) {
        if (seg.has_value()) return seg.value().cutLess(cut.axis, cut.less_than);
        else return std::nullopt;
    }
};

std::string str(std::optional<Segment3d> segment) {
    if (segment.has_value()){
        auto seg = segment.value();
        std::string s = "";
        if (seg.getA().has_value()) {
            s+=str(seg.getA().value());
        }
        else s+="so long";
        s+=" to ";
        if (seg.getB().has_value()) {
            s+=str(seg.getB().value());
        }
        else s+="so long";
        s+="(vec - "+ str(seg.vec) +")";
        return s;
    }
    else return "void segment";
}

enum IntersectionError {
    Similar,
    Else
};

std::variant<Segment3d, IntersectionError> intersection(Halfspace3d& h1, Halfspace3d& h2) {
    Vec3d axis1 = h1.axis;
    Scalar c1 = h1.less_than;
    Vec3d axis2 = h2.axis;
    Scalar c2 = h2.less_than;
    Vec3d vec1 = scale(axis1, c1 / scalar_mult(axis1, axis1));
    Vec3d vec2 = scale(axis2, c2 / scalar_mult(axis2, axis2));
    if (scalar_mult(axis1, axis2)*scalar_mult(axis1, axis2) == len2(axis1)*len2(axis2)) {
        if (scalar_mult(axis1, axis2) > 0 && len2(vec1) == len2(vec2)) return Similar;
        else return Else;
    }
    else {
        Vec3d vec_1_without_2 = sum(vec1, scale(axis2, -scalar_mult(vec1, axis2)/scalar_mult(axis1, axis1)));
        return Segment3d{sum(vec_1_without_2, vec2), vec_mult(axis1, axis2)};
    }
}

std::vector<std::vector<std::optional<Segment3d>>> toPoints(std::vector<Halfspace3d>& cuts) {
    std::vector<Halfspace3d> planes{};
    std::vector<std::vector<std::optional<Segment3d>>> segments{};
    planes.push_back(cuts[0]);
    for(size_t pl_i = 1; pl_i < cuts.size(); pl_i++) {
        planes.push_back(cuts[pl_i]);
        segments.push_back(std::vector<std::optional<Segment3d>>{});
        for(size_t cutter_i = 0; cutter_i < planes.size() - 1; cutter_i++) {
            auto cut_rez = intersection(planes[cutter_i], cuts[pl_i]);
            if (cut_rez.index() == 1)  {
                if (get<IntersectionError>(cut_rez) == Similar) {
                    planes.pop_back();
                    segments.pop_back();
                    LOG<<"del"<<pl_i<<","<<cutter_i<<"\n";
                    break;
                }
                else {
                    segments.back().push_back(std::nullopt);
                }
            }
            else {
                segments.back().push_back(get<Segment3d>(cut_rez));
                LOG<<"add"<<pl_i<<","<<cutter_i<<"\n";
            }
        }
    }
    for(size_t pl_i = 0; pl_i < segments.size(); pl_i++) {
        for(size_t pl2_i = 0; pl2_i < planes.size(); pl2_i++) {
            if (pl2_i != pl_i+1)
                for(size_t i = 0; i < segments[pl_i].size(); i++) {
                    if (i!= pl2_i){
                        segments[pl_i][i] = Segment3d::cut(segments[pl_i][i], planes[pl2_i]);
                        LOG <<"segment" <<pl_i <<"::" <<i <<str(segments[pl_i][i]) <<" cut by" <<pl2_i <<'\n';
                    }
                }
        }
    }
    return segments;
}

// template<bool checkSquare = false>
std::vector<Triangle> triangulate(std::vector<std::vector<std::optional<Segment3d>>> segments) {
    std::vector<Triangle> triangles{};
    for(size_t i = 0; i < segments.size()+1; i++) {
        std::vector<std::tuple<Vec3d, Vec3d>> points{};
        if (i>0) {
            for(auto opt_seg : segments[i-1]) {
                if (opt_seg.has_value()) {
                    Segment3d seg = opt_seg.value();
                    assert(seg.getA().has_value()&&seg.getB().has_value());
                    points.push_back({seg.getA().value(), seg.getB().value()});
                }
            }
        }
        for(size_t j = i; j<segments.size(); j++) {
            if (segments[j][i].has_value()) {
                Segment3d seg = segments[j][i].value();
                assert(seg.getA().has_value()&&seg.getB().has_value());
                points.push_back({seg.getA().value(), seg.getB().value()});
            }
        }
        for(int p = 1;p<points.size(); p++){
            if (/*!checkSquare || !(std::get<0>(points[0]) == std::get<0>(points[p]) || std::get<0>(points[0]) == std::get<1>(points[p]) || std::get<0>(points[p]) == std::get<1>(points[p]))*/ true) {
                LOG << "[Triangle by plane"<<i << "]"
                    <<str(std::get<0>(points[0]))
                    <<str(std::get<0>(points[p]))
                    <<str(std::get<1>(points[p])) <<'\n';
                triangles.push_back({
                                     std::get<0>(points[0]),
                                     std::get<0>(points[p]),
                                     std::get<1>(points[p])});
            }
        }
        // std::vector<Vec3d> points{};
        // if (i>0) {
        //     for(auto opt_seg : segments[i-1]) {
        //         if (opt_seg.has_value()) {
        //             Segment3d seg = opt_seg.value();
        //             if(seg.getA().has_value()) points.push_back(seg.getA().value());
        //             if(seg.getB().has_value()) points.push_back(seg.getB().value());
        //         }
        //     }
        // }
        // for(size_t j = i; j<segments.size(); j++) {
        //     if (segments[j][i].has_value()) {
        //         Segment3d seg = segments[j][i].value();
        //         if(seg.getA().has_value()) points.push_back(seg.getA().value());
        //         if(seg.getB().has_value()) points.push_back(seg.getB().value());
        //     }
        // }
        // for(size_t p = 1; p < points.size()-1; p++) {
        //     if(!checkSquare || !(points[0] == points[p] || points[0] == points[p+1] || points[p] == points[p+1]))
        //         triangles.push_back({points[0], points[p], points[p+1]});
        // }
    }
    return triangles;
}
}
#endif // TRIANGULATION_H
