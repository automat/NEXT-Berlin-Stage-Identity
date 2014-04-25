#include "util/GeomUtil.h"
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "cinder/Rand.h"

namespace next{

    namespace util {
        using namespace boost::assign;
        using namespace ci;
        using namespace std;

        bool getIntersection(const Planef& plane, const Vec3f& p0, const Vec3f& p1, Vec3f* intersection){
            const Vec3f& normal = plane.getNormal();
            const Vec3f& point  = plane.getPoint();

            float t = normal.dot(point - p0) / normal.dot(p1 - p0);

            if(t > 0 && t < 1){
                *intersection = p0 + (p1 - p0) * t;
                return true;
            } else if( t == 0){
                *intersection = p0;
                return true;
            } else if( t == 1){
                *intersection = p1;
                return true;
            }
            return false;
        }

        void push3(uint32_t a, uint32_t b, uint32_t c, vector<uint32_t>* v){
            v->push_back(a);
            v->push_back(b);
            v->push_back(c);
        }

        void subdivideTriangle(uint32_t indexA, uint32_t indexB, uint32 indexC, vector<Vec3f>& vertices, vector<uint32_t>& indicesOut, bool normalize = false){
            const Vec3f& pA = vertices[indexA];
            const Vec3f& pB = vertices[indexB];
            const Vec3f& pC = vertices[indexC];

            // create interpolated points on triangles edges
            Vec3f pAB = ((pA + pB) * 0.5f);
            Vec3f pBC = ((pB + pC) * 0.5f);
            Vec3f pCA = ((pC + pA) * 0.5f);

            if(normalize){
                pAB.safeNormalize();
                pBC.safeNormalize();
                pCA.safeNormalize();
            }

            // push those to the orignal mesh
            vertices.push_back(pAB);
            vertices.push_back(pBC);
            vertices.push_back(pCA);

            // get their vertex indices
            uint32_t indexAB = vertices.size() - 3;
            uint32_t indexBC = vertices.size() - 2;
            uint32_t indexCA = vertices.size() - 1;

            //         A
            //        / \
            //       /   \         CCW
            //      AB---CA
            //     / \   / \
            //    /   \ /   \
            //   B----BC-----C

            indicesOut += indexA,  indexAB, indexCA;
            indicesOut += indexB,  indexBC, indexAB;
            indicesOut += indexC,  indexCA, indexBC;
            indicesOut += indexAB, indexBC, indexCA;
        }

        using namespace std;
        void subdivide(vector<Vec3f>& vertices, vector<uint32_t>& indices, int numSubDivisions, bool normalize){
            vector<uint32_t> tempIndices;

            int numTriangles;
            uint32_t indexA, indexB, indexC;

            int i,j;
            i = -1;
            while (++i < numSubDivisions) {
                tempIndices.clear(); //reset triangle indices
                j = -1;
                numTriangles = indices.size() / 3;
                while (++j < numTriangles) {
                    indexA = indices[j*3  ];
                    indexB = indices[j*3+1];
                    indexC = indices[j*3+2];

                    subdivideTriangle(indexA,
                            indexB,
                            indexC,
                            vertices, tempIndices, normalize);
                }

                indices.swap(tempIndices);
            }
        }

        void randomSubdivide(vector<Vec3f>& vertices, vector<uint32_t>& indices, int numSubDivisions, float threshold, bool normalize){
            vector<uint32_t> tempIndices;

            int numTriangles;
            int i,j;
            i = -1;
            while (++i < numSubDivisions) {
                j = -1;
                numTriangles = indices.size() / 3;
                while (++j < numTriangles) {
                    if(Rand::randFloat() < threshold){
                        subdivideTriangle(indices[j*3  ],
                                indices[j*3+1],
                                indices[j*3+2],
                                vertices, indices, normalize);
                        indices.erase(indices.begin() + j*3, indices.begin() + j * 3 + 3);
                        numTriangles = indices.size() / 3;
                    }
                }
            }
        }

        void genUniqueFaces(vector<Vec3f>& vertices, vector<uint32_t>& indices, vector<Vec3f>& normals){
            vector<Vec3f>    tempVertices;
            vector<uint32_t> tempIndices;
            vector<Vec3f>    tempNormals;
            int numTriangles = indices.size() / 3;
            int i = -1;
            int j = 0;

            uint32_t indexA,indexB,indexC;

            while (++i < numTriangles) {
                indexA = indices[i*3  ];
                indexB = indices[i*3+1];
                indexC = indices[i*3+2];
                tempVertices += vertices[indexA];
                tempVertices += vertices[indexB];
                tempVertices += vertices[indexC];
                tempIndices  += j++;
                tempIndices  += j++;
                tempIndices  += j++;
            }

            tempNormals.resize(tempVertices.size());

            vertices.swap(tempVertices);
            indices.swap(tempIndices);
            normals.swap(tempNormals);
        }
    }
}