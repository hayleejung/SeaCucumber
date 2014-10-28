#pragma once

#include "cinder/BSpline.h"
#include "cinder/Matrix.h"
#include "cinder/TriMesh.h"

#include "Tube.h"


using namespace ci;

namespace sc {
    
using ThornRef = std::shared_ptr<class Thorn>;
    
class Thorn: public Tube {
    
    
public:
    
    static ThornRef create(const int32_t segs, const int32_t scale0=0,const int32_t scale1=1,const int32_t rad =1,const int32_t prof=8){
        return ThornRef(new Thorn(segs,scale0,scale1,rad,prof));
    }
    
    static ThornRef create_default(){
        return ThornRef(new Thorn());
    }
    
    //~Thorn() {}
    
    void setSpline(const int degr = 3);
    void setProfile();
    
    void draw() override;
    void update() override;
    
    void setProfileValue(const int32_t& prof);
    int32_t getProfileValue() const;
    void setRadius(const int32_t& rad);
    int32_t getRadius() const;
    
protected:
    Thorn();
    Thorn(const int32_t segs, const int32_t scale0=0,const int32_t scale1=1,const int32_t rad =1,const int32_t prof=8);
    
    
private:
    std::vector<vec3>		mLineBase;
	std::vector<vec3>		mLinePoints;
    
    std::vector<vec3>		mProfPoints;
    
	BSpline3f				mBSpline;
	TriMeshRef				mTubeMesh;
    
    //Tube mTube;
//    int32_t					mNumSegs;
    
    bool isWireframe;
    
    int32_t radius;
    int32_t profile;
    
};

}