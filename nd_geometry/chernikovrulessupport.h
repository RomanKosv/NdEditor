#ifndef CHERNIKOVRULESSUPPORT_H
#define CHERNIKOVRULESSUPPORT_H
#include "../array_utilites.h"
#include "linear_geometry_std_structures.h"
#include "../linear_algebra/linear_algebra_utilites.h"
#include <unordered_set>
#include <tuple>
#include <iostream>
using namespace nd_geometry;
using namespace array_utilites;
using std::unordered_set;
using std::tuple;
using std::cout;
namespace nd_parser_realisation_1{
template<Ordable Coef, typename Expr> requires ArifmetycOp<Coef>
class ChernikovRulesSupport
{
public:
    static tuple<vector<Expr>, vector<unordered_set<int>>, int> chernikov_step(
        vector<Expr> p, Expr dim, vector<unordered_set<int>> parents, int s,
        LinearAlgebra<Expr,Coef>& la, LinearOrder<Coef>& order){
        cout<<"iteration "<<s<<"\n";
        std::vector<Expr> negative;
        std::vector<Expr> neutral;
        std::vector<Expr> positive;
        vector<unordered_set<int>> negative_parents;
        vector<unordered_set<int>> neutral_parents;
        vector<unordered_set<int>> positive_parents;
        for(int i=0; i<p.size(); i++){
            Expr& e=p.at(i);
            CompRes comp=linear_algebra_utilites::dot_collyniaryty_to_0(
                la, order, e, dim);
            switch (comp) {
            case CompRes::Less:
                negative.push_back(e);
                negative_parents.push_back(parents.at(i));
                break;
            case CompRes::Eq:
                neutral.push_back(e);
                neutral_parents.push_back(parents.at(i));
                break;
            default:
                positive.push_back(e);
                positive_parents.push_back(parents.at(i));
                break;
            }
        }
        cout<<"first rule: \n";
        for(int i=0; i<negative.size(); i++){
            for(int j=0; j<positive.size(); j++){
                unordered_set<int> unit_parents=negative_parents.at(i);
                unit_parents.insert(positive_parents.at(j).begin(),positive_parents.at(j).end());
                Expr negative_vec=negative.at(i);
                Expr positive_vec=positive.at(j);
                auto newexpr=linear_algebra_utilites::annihilate_inequalites_component(la,order,negative_vec,positive_vec,dim);
                if(unit_parents.size()<=s+1){//первое правило черникова
                    //assert(false);
                    neutral.push_back(newexpr);
                    neutral_parents.push_back(unit_parents);
                }else{
                    cout<<"eliminate "<<newexpr<<" ";
                    cout<<"negative "<<negative_vec<<" ";
                    cout<<"negative parents "<<"("<<negative_parents.at(i).size()<<")";
                    for(int t:negative_parents.at(i)){
                        cout<<t<<", ";
                    }
                    cout<<"\n";
                    cout<<"positive "<<positive_vec<<" ";
                    cout<<"positive parents "<<"("<<positive_parents.at(j).size()<<")";
                    for(int t:positive_parents.at(j)){
                        cout<<t<<", ";
                    }
                    cout<<"\n\n";
                }
            }
        }
        vector<Expr> res;
        vector<unordered_set<int>> res_parents;
        vector<bool> derived(neutral_parents.size(),false);//второе правило, если true значит избыточное
        cout<<"second rule: \n";
        for(int i=0; i<neutral.size(); i++){
            for(int j=0; j<neutral.size(); j++){
                if(neutral_parents.at(i).size()!=neutral_parents.at(j).size()){
                    bool second_rule_flag=true;
                    for(int ind:neutral_parents.at(i)){
                        if(!neutral_parents.at(j).contains(ind)){
                            second_rule_flag=false;
                            break;
                        }
                    }
                    if(second_rule_flag){
                        derived.at(j)=true;
                        cout<<"eleminated "<<neutral.at(j)<<" ";
                        cout<<"stronger "<<neutral.at(i)<<" ";
                        cout<<"stronger parents "<<"("<<neutral_parents.at(i).size()<<")";
                        for(int t:neutral_parents.at(i)){
                            cout<<t<<", ";
                        }
                        cout<<"\n";
                        cout<<"eleminated parents "<<"("<<neutral_parents.at(j).size()<<")";
                        for(int t:neutral_parents.at(j)){
                            cout<<t<<", ";
                        }
                        cout<<"\n\n";
                    }
                }
            }
        }
        //удаляем все те кого правило черникова пометило как избыточные
        for(int i=0; i<neutral.size(); i++){
            if(!derived.at(i)){
                res.push_back(neutral.at(i));
                res_parents.push_back(neutral_parents.at(i));
            }
        }
        cout<<"result\n";
        for(int i=0; i<res.size(); i++){
            cout<<"expr: "<<res.at(i)<<" ";
            cout<<"parents: ";
            for(int it:res_parents.at(i)){
                cout<<it<<" ";
            }
            cout<<"\n\n";
        }
        return {res,res_parents,s+1};
    };
    static tuple<vector<Expr>, vector<unordered_set<int>>>
    chernikov_elimination(vector<Expr>& p, vector<Expr>& dims,
                          LinearAlgebra<Expr,Coef>& la, LinearOrder<Coef>& order){
        cout<<"dims:\n";
        for(Expr& e:dims){
            cout<<e<<" ";
        }
        cout<<"\n\n";
        vector<Expr> curr=p;
        vector<unordered_set<int>> parents;
        int s=1; //номер итерации
        //заполняем parents
        for(int i=0; i<p.size(); i++){
            parents.push_back({i});
        }
        //исключаем измерения
        for(Expr& e:dims){
            std::tie(curr,parents,s)=chernikov_step(curr, e, parents, s, la, order);
        }
        return {curr,parents};
    };
    static Polyhedron<Expr> project_parallel_dims(Polyhedron<Expr> p, vector<Expr>& dims,
                                                  LinearAlgebra<Expr,Coef>& la,
                                                  LinearOrder<Coef>& order,
                                                    GeometryObjectFactory<Expr>& factory){
        cout<<"dims:\n";
        for(Expr& e:dims){
            cout<<e<<" ";
        }
        cout<<"\n\n";
        vector<Expr> res;
        vector<unordered_set<int>> parents;
        vector<Expr> exprs;
        vector<bool> is_strong;
        for(HalfSpace<Expr> hs:*p.get_faces()){
            exprs.push_back(*hs.get_upper_bound());
            is_strong.push_back(hs.is_strong());
        }
        std::tie(res,parents)=chernikov_elimination(exprs,dims,la,order);
        cout<<"res: \n";
        vector<HalfSpace<Expr>> faces;
        for(int i=0; i<res.size(); i++){
            bool strong=false;
            cout<<"expr: "<<res.at(i)<<"\n";
            cout<<"parents: ";
            for(int ind:parents.at(i)){
                strong=strong|is_strong.at(ind);
                cout<<ind<<" ";
            }
            cout<<"\n";
            cout<<"strong: "<<strong<<"\n";
            faces.push_back(factory.make_halfspace(res.at(i),strong));
        }
        return factory.make_polyhedron(faces);
    }
};
}
#endif // CHERNIKOVRULESSUPPORT_H
