//
//  GeomHelper.h
//  Room_00
//
//  Created by Henryk Wollik on 01/02/14.
//
//

#ifndef Room_00_GeomHelper_h
#define Room_00_GeomHelper_h

#include <math.h>
#include <vector>

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/Rand.h"

using namespace std;
using namespace ci;

class GeomHelper {
    static void push3(uint32_t a, uint32_t b, uint32_t c, vector<uint32_t>* v){
        v->push_back(a);
        v->push_back(b);
        v->push_back(c);
    }
    
public:
    
    static void addFace(uint32_t a, uint32_t b, uint32_t c, TriMesh* mesh){
        push3(a,b,c,&(mesh->getIndices()));
    }
    
    static void subdivideFaces(TriMesh* mesh, int numSubDivisions = 0){
        vector<Vec3f>&   meshVertices  = mesh->getVertices();
        vector<Vec2f>&   meshTexcoords = mesh->getTexCoords();
        vector<uint32_t> meshIndices   = mesh->getIndices();
        
        vector<uint32_t> tempIndices;
        
        int i,j;
        i = -1;
        while (++i < numSubDivisions) {
            tempIndices.clear(); //reset triangle indices
            j = -1;
            while (++j < mesh->getNumTriangles()) {
                
                
                // get triagnle indices from original mesh
                uint32_t indexA = meshIndices[j*3  ];
                uint32_t indexB = meshIndices[j*3+1];
                uint32_t indexC = meshIndices[j*3+2];
                
                const Vec3f& pA = meshVertices[indexA];
                const Vec3f& pB = meshVertices[indexB];
                const Vec3f& pC = meshVertices[indexC];
                
                const Vec2f& tA = meshTexcoords[indexA];
                const Vec2f& tB = meshTexcoords[indexB];
                const Vec2f& tC = meshTexcoords[indexC];
                
                // create interpolated points on triangles edges
                Vec3f pAB = (pA + pB) * 0.5f;
                Vec3f pBC = (pB + pC) * 0.5f;
                Vec3f pCA = (pC + pA) * 0.5f;
                
                Vec2f tAB = (tA + tB) * 0.5f;
                Vec2f tBC = (tB + tC) * 0.5f;
                Vec2f tCA = (tC + tA) * 0.5f;
                
                // push those to the orignal mesh
                mesh->appendVertex(pAB);
                mesh->appendVertex(pBC);
                mesh->appendVertex(pCA);
                
                mesh->appendTexCoord(tAB);
                mesh->appendTexCoord(tBC);
                mesh->appendTexCoord(tCA);
                
                
                // get their vertex indices
                uint32_t indexAB = meshVertices.size() - 3;
                uint32_t indexBC = meshVertices.size() - 2;
                uint32_t indexCA = meshVertices.size() - 1;
                
                //         A
                //        / \
                //       /   \         CCW
                //      AB---CA
                //     / \   / \
                //    /   \ /   \
                //   B----BC-----C
                
                push3(indexA,  indexAB, indexCA, &tempIndices);
                push3(indexB,  indexBC, indexAB, &tempIndices);
                push3(indexC,  indexCA, indexBC, &tempIndices);
                push3(indexAB, indexBC, indexCA, &tempIndices);
                
            }
            mesh->getIndices().swap(tempIndices);
        }
    }
    
    static void genIcosahedron(TriMesh* mesh, int numSubDivisions = 0){
        vector<uint32_t>& meshIndices  = mesh->getIndices();
        vector<Vec3f>&    meshVertices = mesh->getVertices();
        
        float unit = (1.0f + sqrtf(5.0f)) / 2.0f;
        //gen 12 base vertices
        
        mesh->appendVertex(Vec3f(-1.0f, unit, 0.0f).normalized());
        mesh->appendVertex(Vec3f( 1.0f, unit, 0.0f).normalized());
        mesh->appendVertex(Vec3f(-1.0f,-unit, 0.0f).normalized());
        mesh->appendVertex(Vec3f( 1.0f,-unit, 0.0f).normalized());
        
        mesh->appendVertex(Vec3f( 0.0f,-1.0f, unit).normalized());
        mesh->appendVertex(Vec3f( 0.0f, 1.0f, unit).normalized());
        mesh->appendVertex(Vec3f( 0.0f,-1.0f,-unit).normalized());
        mesh->appendVertex(Vec3f( 0.0f, 1.0f,-unit).normalized());
        
        mesh->appendVertex(Vec3f( unit, 0.0f,-1.0f).normalized());
        mesh->appendVertex(Vec3f( unit, 0.0f, 1.0f).normalized());
        mesh->appendVertex(Vec3f(-unit, 0.0f,-1.0f).normalized());
        mesh->appendVertex(Vec3f(-unit, 0.0f, 1.0f).normalized());
        
        vector<uint32_t> tempIndices;
        
        // p0
        push3(  0, 11,  5, &tempIndices);
        push3(  0,  5,  1, &tempIndices);
        push3(  0,  1,  7, &tempIndices);
        push3(  0,  7, 10, &tempIndices);
        push3(  0, 10, 11, &tempIndices);
        
        
        push3(  1,  5,  9, &tempIndices);
        push3(  5, 11,  4, &tempIndices);
        push3( 11, 10,  2, &tempIndices);
        push3( 10,  7,  6, &tempIndices);
        push3(  7,  1,  8, &tempIndices);
        
        // p3
        push3(  3,  9,  4, &tempIndices);
        push3(  3,  4,  2, &tempIndices);
        push3(  3,  2,  6, &tempIndices);
        push3(  3,  6,  8, &tempIndices);
        push3(  3,  8,  9, &tempIndices);
        
        push3(  4,  9,  5, &tempIndices);
        push3(  2,  4, 11, &tempIndices);
        push3(  6,  2, 10, &tempIndices);
        push3(  8,  6,  7, &tempIndices);
        push3(  9,  8,  1, &tempIndices);
        
        meshIndices.swap(tempIndices);
        tempIndices.clear();
        
        int i,j;
        i = -1;
        while (++i < numSubDivisions) {
            tempIndices.clear(); //reset triangle indices
            j = -1;
            while (++j < mesh->getNumTriangles()) {
                
                
                // get triagnle indices from original mesh
                uint32_t indexA = meshIndices[j*3  ];
                uint32_t indexB = meshIndices[j*3+1];
                uint32_t indexC = meshIndices[j*3+2];
                
                const Vec3f& pA = meshVertices[indexA];
                const Vec3f& pB = meshVertices[indexB];
                const Vec3f& pC = meshVertices[indexC];
                
                // create interpolated points on triangles edges
                Vec3f pAB = ((pA + pB) * 0.5f).normalized();
                Vec3f pBC = ((pB + pC) * 0.5f).normalized();
                Vec3f pCA = ((pC + pA) * 0.5f).normalized();
                
                // push those to the orignal mesh
                mesh->appendVertex(pAB);
                mesh->appendVertex(pBC);
                mesh->appendVertex(pCA);
                
                // get their vertex indices
                uint32_t indexAB = meshVertices.size() - 3;
                uint32_t indexBC = meshVertices.size() - 2;
                uint32_t indexCA = meshVertices.size() - 1;
                
                //         A
                //        / \
                //       /   \         CCW
                //      AB---CA
                //     / \   / \
                //    /   \ /   \
                //   B----BC-----C
                
                push3(indexA,  indexAB, indexCA, &tempIndices);
                push3(indexB,  indexBC, indexAB, &tempIndices);
                push3(indexC,  indexCA, indexBC, &tempIndices);
                push3(indexAB, indexBC, indexCA, &tempIndices);
  
            }
            mesh->getIndices().swap(tempIndices);
        }
        
        //TODO: Fix inverted normals
        i = 0;
        while (i < mesh->getNumTriangles()) {
            const Vec3f& p0 = mesh->getVertices()[i*3  ];
            const Vec3f& p1 = mesh->getVertices()[i*3+1];
            const Vec3f& p2 = mesh->getVertices()[i*3+2];
            
            Vec3f tangent  = (p2 - p0).normalized();
            Vec3f binormal = (p2 - p1).normalized();
            Vec3f normal   = tangent.cross(binormal).normalized() * -1;
            
            mesh->appendNormal(normal);
            mesh->appendNormal(normal);
            mesh->appendNormal(normal);
            
            ++i;
        }
    }
    
    static void invertNormals(TriMesh& mesh){
        vector<Vec3f>& normals = mesh.getNormals();
        for (vector<Vec3f>::iterator itr = normals.begin(); itr != normals.end(); ++itr) {
            *itr *= -1;
        }
    }
    
};



#endif
