
#include "Body.h"


namespace sc {
    

Body::Body()
:mNumSegs(20)
{
    setBodySpline();
    sampleCurve();
    //buildPTF();
    
}
    
Body::Body(const int32_t segs,const int32_t len):mNumSegs(segs),length(len)
{
    
    
        setBodySpline();
        //sampleCurve();
        //buildPTF();
        
    }

void Body::draw()
{
    gl::color( Color( 1.0f, 0.0f, 0.0f ) );
    drawPs(2.0);
    drawThorns();
}

void Body::setLength(const int32_t len){
    length = len;
}

int32_t Body::getLength(){
    
    return length;
}

int32_t Body::getSegments(){
    
    return mNumSegs;
}

void Body::setSegments(const int32_t segs){
    mNumSegs = segs;
}
    
void Body::update()
{
    
    float t = cinder::app::getElapsedSeconds();
    float r = randFloat(0.99, 1.01);
    for( size_t i =0; i < mLineBase.size(); ++i){
        float dx = 0;
        float dy = 0;
        float dz = 0;
        
        if( ( i < mLineBase.size()-1)){
            dx = sin( t*((mLineBase.size()-1)/2.0f + i )) * 3;
            dy = sin( t*i / 1.0f ) * 4;
            dz = cos( i*i *r) * 6;
        }
        mLinePoints[i] = mLineBase[i] + vec3( dx, dy, dz );
    }
    
    // make the b spline;
    int degree = 2;
    bool loop = false;
    bool open = true;
    
    mBSpline = BSpline3f( mLinePoints, degree, loop, open );
    
    
    gl::color( Color( 1.0f, 0.0f,  0.0f ) );
    
    sampleCurve();
    //buildPTF();
    drawPs(2.0);
    
    mThorn->update();
}

void Body::setBodySpline()
{
    int len = 6;
    mLineBase.push_back( vec3( len*3, 0,0 ) );
	mLineBase.push_back( vec3( len*2, 0,0 ) );
	mLineBase.push_back( vec3( len*1, 0,0 ) );
	mLineBase.push_back( vec3( 0, 0, 0 ) );
	mLinePoints = mLineBase;
    
    int  degree = 3;
	bool loop = false;
	bool open = true;
	mBSpline = BSpline3f( mLinePoints, degree, loop, open );
    
}

void Body::sampleCurve()
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

void Body::drawThorns()
{
    float numRot = 5;
    for( int i = 0; i< mPs.size()-1; ++i ){
        for( float j = 0; j < numRot ; ++j ){
            mModelMatrix = mat4( translate( mPs[i]) );
            mModelMatrix *= mat4( rotate( numRot * j, mTs[i]));
            gl::ScopedModelMatrix push;
            gl::multModelMatrix( mModelMatrix );
            mThorn->draw();
        }
    }
    //std::cout<< "thisis"<<mFrames[5] <<std::endl;
}

void Body::buildPTF()
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


void Body::drawPs( float lineWidth )
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


void Body::drawTs( float lineLength , float lineWidth )
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