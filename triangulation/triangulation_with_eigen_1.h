#ifndef TRIANGULATION_WITH_EIGEN_1_H
#define TRIANGULATION_WITH_EIGEN_1_H
#include <iostream>
#include <vector>
#include <tuple>
#include <Eigen/Dense>
//#include "triangulation.h"
using namespace std;
using namespace Eigen;

vector<tuple<Vector3f, Vector3f, Vector3f>> easy_triangulate(vector<Vector3f> normals,vector<float> distances,float epsilon = 1e-5f);

#endif // TRIANGULATION_WITH_EIGEN_1_H
