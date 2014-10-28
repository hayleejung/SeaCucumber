
#include "Thorn.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

namespace sc {
    


Thorn::Thorn():Tube()
{
    
    radius = 1;
    profile = 8;
    isWireframe = false;
//    mNumSegs = 8;
    setSpline();
    setProfile();

    mTubeMesh = TriMesh::create( TriMesh::Format().positions() );
	//this->setNumSegments( mNumSegs );
	this->sampleCurve();
    this->buildPTF();
    this->buildMesh( mTubeMesh.get() );
}

Thorn::Thorn(const int32_t segs, const int32_t scale0,const int32_t scale1,const int32_t rad ,const int32_t prof):Tube(segs,scale0,scale1),radius(rad),profile(prof)
{
    isWireframe = false;
    //    mNumSegs = 8;
    setSpline();
    setProfile();
    
    mTubeMesh = TriMesh::create( TriMesh::Format().positions() );
	//this->setNumSegments( mNumSegs );
	this->sampleCurve();
    this->buildPTF();
    this->buildMesh( mTubeMesh.get() );
}
    
    
void Thorn::setProfileValue(const int32_t& prof){
        profile = prof;
}
    
int32_t Thorn::getProfileValue() const{
    return profile;
}
    
void Thorn::setRadius(const int32_t& rad){
        radius = rad;
}
    
int32_t Thorn::getRadius() const{
    return radius;
}

void Thorn::update()
{
    std::vector<vec3> prof;
    //makeCircleProfile( prof, 0.25f, 16 );
    //mTube.setProfile( prof );
    
    // B Spline
    //float t = getElapsedSeconds();
    float t = cinder::app::getElapsedSeconds();
    float r = randFloat(0.99, 1.01);
    for( size_t i =0; i < mLineBase.size(); ++i){
        float dx = 0;
        float dy = 0;
        float dz = 0;
        
        if( ( i < mLineBase.size()-1)){
            dx = sin( t*((mLineBase.size()-1)/2.0f + i )) * 0.5f;
            dy = sin( t*i / 3.0f ) * 0.8f;
            dz = cos( i*i *r) * 1.0f;
        }
        mLinePoints[i] = mLineBase[i] + vec3( dx, dy, dz );
    }
    
    // make the b spline;
    int degree = 3;
    bool loop = false;
    bool open = true;
    
    mBSpline = BSpline3f( mLinePoints, degree, loop, open );
    
    //Tube
    this->setTubeSpline( mBSpline );
    this->setNumSegments( mNumSegs );
    this->sampleCurve();
    
    this->buildPTF();
    
    this->buildMesh( mTubeMesh.get() );
}

void Thorn::setProfile()
{
    //mProfPoints.clear();
    std::vector<vec3> prof;
    float dt = 6.28318531f / (float)mNumSegs;
    for( int i = 0; i < mNumSegs; ++i ){
        float t = i*dt;
        prof.push_back( vec3( cos(t)*radius, sin(t)*radius, 0 ));
    }
    
    this->setTubeProfile( prof );
    
}

void Thorn::setSpline(const int degr)
{
    mLineBase.push_back( vec3( 0, 6, 0 ) );
	mLineBase.push_back( vec3( 0, 4, 0 ) );
	mLineBase.push_back( vec3( 0, 2, 0 ) );
	mLineBase.push_back( vec3( 0, 0, 0 ) );
	mLinePoints = mLineBase;
    
    //int  degree = degr;
	bool loop = false;
	bool open = true;
	mBSpline = BSpline3f( mLinePoints, degr, loop, open );
    this->setTubeSpline( mBSpline );
}



void Thorn::draw()
{
    this->drawPs();
    
    gl::enableAdditiveBlending();
    gl::color( ColorA( 1, 1, 1, 0.25f ) );
    
    gl::draw( *mTubeMesh );
    
    if( isWireframe ){
        gl::enableWireframe();
        gl::color( Color( 0.2f, 0.2f, 0.5f ) );
        gl::draw( *mTubeMesh );
        gl::disableWireframe();
    }
}
}