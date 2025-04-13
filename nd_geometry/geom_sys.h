#ifndef GEOM_SYS_H
#define GEOM_SYS_H
#include <vector>

//typedef int DimID;
namespace nd_geometry{
template<typename Dim, typename ObjIn, typename ObjOut=ObjIn>
class GeometrySystem{
public:
    virtual ObjOut intersect_of(ObjIn o1, ObjIn o2)=0;
    virtual ObjOut union_of(ObjIn o1, ObjIn o2)=0;
    virtual ObjOut project_parallel(ObjIn o, Dim d)=0;
    virtual ObjOut project_in(ObjIn o, std::vector<Dim> dims)=0;
    virtual ObjOut inversion_of(ObjIn o)=0;
    //virtual Obj copy_dim(Obj o, Dim newdim, Dim sourcedim)=0;
    //virtual Obj swap_dims(Obj o, Dim a, Dim b)=0;
};
template<typename T, typename CommonObjT, typename Dim>
concept GeomSys = requires (CommonObjT obj1, CommonObjT obj2, T sys, Dim d, std::vector<Dim> dims){
    CommonObjT(sys.intersect_of(obj1, obj2));
    CommonObjT(sys.project_parallel(obj1, d));
    CommonObjT(sys.project_in(obj1, dims));
};

//nx=new Dim;   вспомогательное измерение
//obj=new Obj;
//transform=new Obj(nx=2*x);
//depedence=intersect_of(obj,transform);    преобразовали из x в nx по правилу transform
//without_x=project_parallel(depedence,x);
//simmetry_x_nx=intersect(without_x, new Obj(x=nx));    указали x=nx
//newobj=project_parallel(simmetry_x_nx,nx);    nx нам больше не нужен, оставляем x
}
#endif // GEOM_SYS_H
