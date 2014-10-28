
#include "Tube.h"
#include "cinder/gl/gl.h"

namespace sc {
    

void addQuadToMesh( TriMesh& mesh, const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3 )
{
    mesh.appendVertex( p0 );
    mesh.appendVertex( p1 );
    mesh.appendVertex( p2 );
    mesh.appendVertex( p3 );
    
    int vert0 = mesh.getNumVertices() -4;
    int vert1 = mesh.getNumVertices() -1;
    int vert2 = mesh.getNumVertices() -2;
    int vert3 = mesh.getNumVertices() -3;
	mesh.appendTriangle( vert0, vert1, vert3 );
	mesh.appendTriangle( vert3, vert1, vert2 );
}

Tube::Tube()
: mNumSegs(8), mScale0(0), mScale1(1)
{
}

Tube::Tube(const int32_t segs, const int32_t scale0,const int32_t scale1)
: mNumSegs(segs), mScale0(scale0), mScale1(scale1)
{
}

void Tube::setSegments(const int32_t segs){
    mNumSegs = segs;
}
    
int32_t Tube::getSegments(){
    return mNumSegs;

}
    
void Tube::setScale0(const float len){
    
    mScale0 = len;
}

float Tube::getScale0(){
    return mScale0;
}
    
void Tube::setScale1(const float len){
        
        mScale1 = len;
}
    
float Tube::getScale1(){
        return mScale1;
}
    
    
void Tube::sampleCurve()
{
    mPs.clear();
    mTs.clear();
    
    float dt = 1.0f/(float)mNumSegs;
    
    for (int i = 0; i < mNumSegs; ++i) {
        float t = i*dt;
        
        vec3 P = mBSpline.getPosition(t);
        mPs.push_back(P);
        
        vec3 T = mBSpline.getDerivative(t);
        mTs.push_back( normalize(T));
        
    }
}

void Tube::buildPTF()
{
    mFrames.clear();
    
    int n = mPs.size();
    if( n >= 3 ){
        mFrames.resize(n);
        mFrames[0] = firstFrame( mPs[0], mPs[1], mPs[2] );
        for( int i = 1; i <  n-1; ++i ){
            vec3 PrevT = mTs[i-1];
            vec3 curT = mTs[i];
            mFrames[i] = nextFrame( mFrames[i-1], mPs[i-1], mPs[i], PrevT, curT);
        }
        
        mFrames[n-1] = lastFrame( mFrames[n-2], mPs[n-2], mPs[n-1] );
    }
}

void Tube::buildMesh( ci::TriMesh *tubeMesh )
{
    if(mPs.size() != mFrames.size() || mFrames.size() <3 || mProf.empty() )
        return;
    
    tubeMesh -> clear();
    
    for(int i = 0; i< mPs.size() -1; ++i){
        mat4 mat0 = mFrames[i];
        mat4 mat1 = mFrames[i+1];
        
        float r0 = (float)(i+0) / (float)(mPs.size() - 1);
        float r1 = (float)(i+1) / (float)(mPs.size() - 1);
        
        float rs0 = mScale0 + (mScale1 - mScale0) * r0;
        float rs1 = mScale0 + (mScale1 - mScale0) * r1;
        
        
        for( int ci = 0; ci< mProf.size(); ++ci ) {
            int idx0 = ci;
            int idx1 = (ci == (mProf.size()-1)) ? 0 : ci+1;
            
            vec3 P0 = vec3( mat0 * vec4(mProf[idx0]*rs0, 1) );
			vec3 P1 = vec3( mat0 * vec4(mProf[idx1]*rs0, 1) );
			vec3 P2 = vec3( mat1 * vec4(mProf[idx1]*rs1, 1) );
			vec3 P3 = vec3( mat1 * vec4(mProf[idx0]*rs1, 1) );
            
            addQuadToMesh( *tubeMesh, P0, P3, P2, P1 );
        }
    }
}


void Tube::drawPs( float lineWidth )
{
	if( mPs.empty() )
		return;
    
    gl::lineWidth( lineWidth );
    gl::begin( GL_LINES );
	for( int i = 0; i < ( mPs.size() - 1 ); ++i ) {
        gl::vertex( mPs[i] );
        gl::vertex( mPs[i + 1] );
	}
    gl::end();
}


void Tube::drawTs( float lineLength , float lineWidth )
{
	if( mPs.empty() || mTs.empty() )
		return;
    
	gl::lineWidth( lineWidth );
	gl::begin( GL_LINES );
	for( int i = 0; i < ( mPs.size() - 1 ); ++i ) {
		gl::vertex( mPs[i] );
		gl::vertex( mPs[i] + mTs[i]*lineLength );
	}
	gl::end();
}



}