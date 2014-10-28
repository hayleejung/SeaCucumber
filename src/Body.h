#pragma once

#include "Thorn.h"
#include "Sphere.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;

namespace sc {
    
    using BodyRef = std::shared_ptr<class Body>;

class Body{
public:

    static BodyRef create(const int32_t segs,const int32_t len = 6){
        return BodyRef(new Body(segs,len));
    }
    
    ~Body() {}
    void setSpline();
    
    
    void draw();
    void update();
    
    void setBodySpline();
    void sampleCurve();
    void buildPTF();
    void drawPs( float lineWidth );
    void drawTs( float lineLength, float lineWIdth );
    void drawThorns();
    
    void setLength(const int32_t len);
    int32_t getLength();
    void setSegments(const int32_t segs);
    int32_t getSegments();
    
protected:
    Body();
    Body(const int32_t segs,const int32_t len = 6);
    
    
    int32_t mNumSegs;
    
    std::vector<vec3>		mPs;			// Points in b-spline sample
	std::vector<vec3>		mTs;			// Tangents in b-spline sample
	std::vector<mat4>	mFrames;
    
    
    std::vector<vec3>		mLineBase;
	std::vector<vec3>		mLinePoints;
    
    std::vector<vec3>		mProfPoints;
    
	BSpline3f				mBSpline;
    
    //Thorn mThorn;
    std::map<std::string,TubeRef> mTubeChildren;
    std::map<std::string,SphereRef> mSphereChildren;
    ThornRef mThorn = Thorn::create(20);
    //ThornRef mThorn = Thorn::create_default();
    
    mat4 mModelMatrix;
    int32_t length;
};

}