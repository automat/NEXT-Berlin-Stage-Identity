//
//  main.cpp
//  PointerTest_00
//
//  Created by Henryk Wollik on 17/03/14.
//  Copyright (c) 2014 atm. All rights reserved.
//

#include <iostream>
#include <vector>

/*
class Class {
    int* mData;
public:
    Class() :
        mData(new int(1)){
    }
    
    Class(const Class& other) :
        mData(other.mData){ // just copy pointer
    }
    
    Class& operator=(const Class& rhs){
        mData = rhs.mData; // just copy pointer
        return *this;
    }
    
    ~Class(){
        delete mData; // pointer gets deleted, therefore copied instances will get invalid. when destructor called, BOOM
        std::cout << "Destructor called." << std::endl;
    }
};*/

/*
class Class {
    int* mData;
public:
    Class() :
        mData(new int(1)){
    }
    
    Class(const Class& other) :
        mData(new int(*other.mData)){ // recreate data pointed too
    }
    
    Class& operator=(const Class& rhs){
        mData = new int(*rhs.mData);  // recreate data pointed too
        return *this;
    }
    
    ~Class(){
        delete mData; // now delete is safe, because data is unique. BUT recreated while copying, not goodygoody
        std::cout << "Destructor called." << std::endl;
    }
};*/

class Class {
    std::shared_ptr<int> mData; // hello happyland!
public:
    Class(int data = 1) :
        mData(new int(data)){
    }
    
    ~Class(){
        std::cout << "Destructor called." << std::endl;
    }
    
    const std::shared_ptr<int>& getDataPtr(){
        return mData;
    }
};

int main(int argc, const char * argv[])
{
    int i, num = 1000;
    /*
    i=-1;while(++i<num){
        Class();
    }*/
    
    /*
    i=-1;while(++i<num){
        Class c = Class();
    }*/
    
    /*
    Class c;
    i=-1;while(++i < num){
        c = Class();
    }*/
    
    Class c(666);
    std::vector<Class> d;
    i=-1;while(++i < num){
        d.push_back(Class(c));
        std::cout << c.getDataPtr().use_count() << std::endl;  // yippie
    }
    
    
    return 0;
}

