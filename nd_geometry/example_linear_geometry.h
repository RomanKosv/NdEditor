#ifndef EXAMPLE_LINEAR_GEOMETRY_H
#define EXAMPLE_LINEAR_GEOMETRY_H
#include <set>
#include <unordered_set>
#include <vector>
#include <memory>

#include <Eigen/Sparse>

#include "geom_sys.h"

/*
template<typename Figure>
class GSFigure{
    virtual void do_intersect(Figure)=0;
    virtual void do
};
*/
using Eigen::SparseVector;
using nd_geometry::GeometrySystem;

namespace ExamplePolyredronGeometry
{
#define MAX_DIM 500000
typedef double Scalar;
typedef SparseVector<Scalar> Vec;
class HalfSpace{
public:
    Vec vec;
    bool is_strong;
    struct Hash{
        std::size_t operator() (const HalfSpace& h) const noexcept{
            return h.vec.sum()+h.is_strong;
        }
    };
    bool operator ==(const HalfSpace &h) const{
        return this->vec.isApprox(h.vec) && this->is_strong==h.is_strong;
    }
};
class Dim{
public:
    const int index;
    bool operator <(const Dim &d) const{
        return this->index<d.index;
    }
};
class Polyhedron{
public:
    std::unordered_set<HalfSpace,HalfSpace::Hash> faces{};
    struct Hash{
        std::size_t operator() (const Polyhedron &p) const noexcept{
            return p.faces.size();
        }
    };
    bool operator ==(const Polyhedron &p) const{
        return p.faces==this->faces;
    }
};
class Group{
public:
    std::unordered_set<Polyhedron,Polyhedron::Hash> components{};
};
static Polyhedron project_polyhedron(Polyhedron &p, Dim d, Scalar epsilon=0.001){
    Polyhedron res;
    std::vector<HalfSpace> positive;
    std::vector<HalfSpace> negative;
    for(auto h : p.faces){
        if(h.vec.coeff(d.index)==0){
            res.faces.insert(h);
        }else if(h.vec.coeff(d.index)>0){
            positive.push_back(h);
        }else{
            negative.push_back(h);
        }
    }
    for(auto i:positive){
        for(auto j:negative){
            HalfSpace h;
            h.is_strong=i.is_strong||j.is_strong;
            h.vec=i.vec/i.vec.coeff(d.index)-j.vec/j.vec.coeff(d.index);
            assert(h.vec.coeff(d.index)<epsilon);
            h.vec.coeffRef(d.index)=0;
            res.faces.insert(h);
        }
    }
    return res;
}
static Polyhedron project_polyhedron_in(Polyhedron &p, std::set<Dim> space){
    for(auto h:p.faces){
        for(Vec::InnerIterator it(h.vec); it; ++it){//непонятно как этот код должен работать
            Dim d{it.index()};
            if(space.count(d)>0){
                auto proj=project_polyhedron(p,d);
                return project_polyhedron_in(proj,space);
            }
        }
    }
    return p;
}
static Polyhedron intersect_polyhedrones(Polyhedron a, Polyhedron b){
    Polyhedron res;
    for(auto i:a.faces){
        res.faces.insert(i);
    }
    for(auto i:b.faces){
        res.faces.insert(i);
    }
    return res;
}
class GS:GeometrySystem<Dim, std::shared_ptr<Group>>{
    typedef std::shared_ptr<Group> Obj;
public:
    Obj intersect_of(Obj o1, Obj o2) override{
        Obj res{new Group};
        for(auto i:o1->components){
            for(auto j:o2->components){
                Polyhedron p=intersect_polyhedrones(i,j);
                res->components.insert(p);
            }
        }
        return res;
    }
    Obj union_of(Obj o1, Obj o2) override{
        Obj res{new Group};
        for(auto i:o1->components){
            res->components.insert(i);
        }
        for(auto i:o2->components){
            res->components.insert(i);
        }
        return res;
    };
    Obj project_parallel(Obj o, Dim d) override{
        Obj res{new Group};
        for(auto i:o->components){
            res->components.insert(project_polyhedron(i,d));
        }
        return res;
    };
    Obj project_in(Obj o, std::set<Dim> space) override{
        Obj res{new Group};
        for(auto i:o->components){
            res->components.insert(project_polyhedron_in(i,space));
        }
        return res;
    }
};
/*
struct A{};
struct B{
public:
    explicit operator A(){
        return A{};
    }
};
*/
/*
static_assert(
    GeomSys<GS,std::shared_ptr<Group>,Dim>
    );
*/
};

#endif // EXAMPLE_LINEAR_GEOMETRY_H
