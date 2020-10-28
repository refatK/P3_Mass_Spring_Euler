#include "pickprimitivedataback.h"

#include <algorithm>

using namespace std;

class ComparePickPrimitive {
public:
    bool comp(const PickPrimitive& a, const PickPrimitive& b){
        return (a.m_d<b.m_d);
    }

    bool operator()(const PickPrimitive& a, const PickPrimitive& b){
        return (a.m_d<b.m_d);
    }
};

void PickPrimitiveDataback::sort(){

    ComparePickPrimitive c;
    std::sort(m_primitives.begin(), m_primitives.end(), c);

}


void PickPrimitiveDataback::initialize(){
    m_primitives.clear();
}
