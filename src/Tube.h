#pragma once

#include "cinder/BSpline.h"
#include "cinder/Matrix.h"
#include "cinder/TriMesh.h"
using namespace cinder;

namespace sc {
    
using TubeRef = std::shared_ptr<class Tube>;
    
class Tube {
public:
	
	Tube();
    Tube(const int32_t segs, const int32_t scale0,const int32_t scale1);
	~Tube() {}
    
    void					setTubeSpline( const BSpline3f& bspline ) { mBSpline = bspline; }
	void					setTubeProfile( const std::vector<vec3>& prof ) { mProf = prof; }
	void					setNumSegments( int n ) { mNumSegs = n; }
    
	void					sampleCurve();
	
	// Builds parallel transport frames for each curve sample
	void					buildPTF();
	
	// Creates a 'tubed' mesh using the profile points
	void					buildMesh( ci::TriMesh *tubeMesh );
    
	virtual void					draw() = 0;
    virtual void					update() = 0;
	void					drawPs( float lineWidth = 1.0f );
    
    void drawTs( float lineLength , float lineWidth );
    
    void setScale0(const float len);
    float getScale0();
    void setScale1(const float len);
    float getScale1();
    void setSegments(const int32_t segs);
    int32_t getSegments();
    
    
    void addChild(std::string name,TubeRef child){
        child->setParent(this);
        mChildren.insert(std::make_pair(name, child));
    };
    
    TubeRef getChild(const std::string& name){
        return mChildren.find(name)->second;
    }

    void setParent( Tube* parent ){ mParent = parent; }
    
protected:
    virtual void setProfile() =0;
	int						mNumSegs;		// Number of segments to sample on the b-spline
	
	BSpline3f				mBSpline;		// b-spline path
	std::vector<vec3>		mProf;			// points to tube (read extrude)
	float					mScale0;		// min scale of profile along curves
	float					mScale1;		// max scale of profile along curves
    
	std::vector<vec3>		mPs;			// Points in b-spline sample
	std::vector<vec3>		mTs;			// Tangents in b-spline sample
	std::vector<mat4>	mFrames;		// Coordinate frame at each b-spline sample
    
    std::map<std::string, TubeRef> mChildren;
    Tube* mParent;
};
}