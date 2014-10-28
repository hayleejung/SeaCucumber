//
//  OtherBody.cpp
//  SeaCucumber
//
//  Created by Adarsh on 20/10/2014.
//
//

#include "OtherBody.h"

namespace sc {
    
    OtherBody::OtherBody():mNumSegs(20){
        
    }
    
OtherBody::OtherBody(const int32_t segs,const int32_t len,const int32_t scale0,const int32_t scale1,const int32_t rad,const int32_t prof):mNumSegs(segs),length(len),Tube(segs,scale0,scale1)
    {
        
        radius = 1;
        this->setBodySpline();
        this->sampleCurve();
        //buildPTF();
        
    }
    
    
    void OtherBody::setLength(const int32_t len){
        length = len;
    }
    
    int32_t OtherBody::getLength(){
        
        return length;
    }
    
    int32_t OtherBody::getSegments(){
        
        return mNumSegs;
    }
    
    void OtherBody::setSegments(const int32_t segs){
        mNumSegs = segs;
    }
    
    
    void OtherBody::setProfile()
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
    
    void OtherBody::setBodySpline(){
        
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
    
    void OtherBody::drawThorns()
    {
        float numRot = 5;
        for( int i = 0; i< mPs.size()-1; ++i ){
            for( float j = 0; j < numRot ; ++j ){
                mModelMatrix = mat4( translate( mPs[i]) );
                mModelMatrix *= mat4( rotate( numRot * j, mTs[i]));
                gl::ScopedModelMatrix push;
                gl::multModelMatrix( mModelMatrix );
                //mThorn->draw();
            }
        }
        //std::cout<< "thisis"<<mFrames[5] <<std::endl;
    }
    
    void OtherBody::draw(){
        
        gl::color( Color( 1.0f, 0.0f, 0.0f ) );
        drawPs(2.0);
        drawThorns();
        
    }
    
    
    void OtherBody::update(){
        
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
        
        //sampleCurve();
        //buildPTF();
        //drawPs(2.0);
        
        //mThorn->update();
    }
    
    void OtherBody::drawPs( float lineWidth )
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
    
    
    void OtherBody::drawTs( float lineLength , float lineWidth )
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