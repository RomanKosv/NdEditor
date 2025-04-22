#ifndef TRIANGULATION_WITH_EIGEN_1_H
#define TRIANGULATION_WITH_EIGEN_1_H
#include <iostream>
#include <vector>
#include <tuple>
#include <Eigen/Dense>
//#include "triangulation.h"
using namespace std;
using namespace Eigen;

vector<tuple<Vector3f, Vector3f, Vector3f>> easy_triangulate(vector<Vector3f> normals,vector<float> distances,float epsilon = 1e-5f) {
    // 1. Задаем данные
    // vector<Vector3f> normals = {
    //     {1, 0, 0},
    //     {0, 1, 0},
    //     {0, 0, 1},
    //     {-1, 0, 0},
    //     {0, -1, 0},
    //     {0, 0, -1}
    // };
    // vector<float> distances = {1, 1, 1, 1, 1, 1}; // например куб со стороной 2
    // float epsilon = 1e-5f;
    //список вершин, лежащих на пересечении данных двух граней (если пересечения нет, пустой)
    vector<vector<vector<Vector3f>>> vertices;

    // 2. Перебор всех троек граней
    int n = normals.size();
    for (int i = 0; i < n; ++i) {
        vertices.push_back(vector<vector<Vector3f>>{});
        for (int j = 0; j < n; ++j) {
            vertices[i].push_back(vector<Vector3f>{});
            for (int k = 0; k < n; ++k) {
                Matrix3f A;
                A.row(0) = normals[i];
                A.row(1) = normals[j];
                A.row(2) = normals[k];

                Vector3f b(distances[i], distances[j], distances[k]);

                // Проверяем ранг матрицы
                FullPivLU<Matrix3f> lu_decomp(A);
                if (lu_decomp.rank() == 3) {
                    // Единственное решение существует
                    Vector3f x = A.colPivHouseholderQr().solve(b);
                    vertices[i][j].push_back(x);
                }
            }
        }
    }

    cout << "Найдено вершин до фильтрации: " << vertices.size() << endl;

    // 3. Фильтрация вершин по принадлежности полиэдру
    vector<vector<vector<Vector3f>>> filtered_vertices;
    for(vector<vector<Vector3f>>& face1:vertices){
        filtered_vertices.push_back({});
        for(vector<Vector3f>& face2:face1){
            filtered_vertices.back().push_back({});
            for(Vector3f& v:face2){
                bool inside = true;
                for (size_t idx = 0; idx < normals.size(); ++idx) {
                    float val = normals[idx].dot(v) - distances[idx];
                    if (val > epsilon) {
                        inside = false;
                        break;
                    }
                    // if (abs(val) <= epsilon)
                    //     on_surface = true;
                }
                if(inside){
                    filtered_vertices.back().back().push_back(v);
                }
            }
        }
    }
    /*
    for (const auto& v : vertices) {
        bool inside = true;
        //bool on_surface = false;
        for (size_t idx = 0; idx < normals.size(); ++idx) {
            float val = normals[idx].dot(v) - distances[idx];
            if (val > epsilon) {
                inside = false;
                break;
            }
            // if (abs(val) <= epsilon)
            //     on_surface = true;
        }
        if (inside)
            filtered_vertices.push_back(v);
    }
    */
    //cout << "Вершин после фильтрации: " << filtered_vertices.size() << endl;

    // 4. Создаем список треугольников из всех троек вершин
    vector<tuple<Vector3f, Vector3f, Vector3f>> triangles;
    int m = filtered_vertices.size();
    for (int i = 0; i < filtered_vertices.size(); ++i) {
        Vector3f s;
        for(vector<Vector3f>& a:filtered_vertices[i]){
            for(Vector3f& b:a){
                s=b;
                break;
            }
        }
        for (int j = 0; j < filtered_vertices[i].size(); ++j) {
            // for (int k = j+1; k < m; ++k) {
            //     triangles.emplace_back(filtered_vertices[i], filtered_vertices[j], filtered_vertices[k]);
            // }
            for(int k = 1; k < filtered_vertices[i][j].size(); k++){
                triangles.push_back({s,filtered_vertices[i][j][k-1],filtered_vertices[i][j][k]});
            }
        }
    }

    cout << "Количество треугольников: " << triangles.size() << endl;

    // Пример вывода первых нескольких треугольников
    // for (size_t t=0; t<min<size_t>(5, triangles.size()); ++t){
    //     auto& tri = triangles[t];
    //     cout << "Треугольник " << t << ": "
    //          << get<0>(tri).transpose() << ", "
    //          << get<1>(tri).transpose() << ", "
    //          << get<2>(tri).transpose() << endl;
    // }

    return triangles;
}

#endif // TRIANGULATION_WITH_EIGEN_1_H
