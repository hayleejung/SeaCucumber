//
//  OtherBody.h
//  SeaCucumber
//
//  Created by Adarsh on 20/10/2014.
//
//

#pragma once

#include <iostream>
#include "Tube.h"
#include "Sphere.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

namespace sc {
    
    using OtherBodyRef = std::shared_ptr<class OtherBody>;
    
    class OtherBody: public Tube{
        
    public:
        static OtherBodyRef create(const int32_t segs,const int32_t len = 6,const int32_t scale0=0,const int32_t scale1=1,const int32_t rad =1,const int32_t prof=8){
        
            return OtherBodyRef(new OtherBody(segs,len, scale0, scale1,rad,prof));
        }
        //~OtherBody();
        
        void setProfile();
        
        void setSpline();
        
        
        void draw() override;
        void update() override;
        
        void setBodySpline();
        void drawThorns();
        
        void setLength(const int32_t len);
        int32_t getLength();
        void setSegments(const int32_t segs);
        int32_t getSegments();
        
        void					drawPs( float lineWidth = 1.0f );
        
        void drawTs( float lineLength , float lineWidth );
        
    protected:
        OtherBody();
        OtherBody(const int32_t segs,const int32_t len = 6,const int32_t scale0=0,const int32_t scale1=1,const int32_t rad =1,const int32_t prof=8);
        
    private:
        std::vector<vec3>		mProf;
        
        std::vector<vec3>		mLineBase;
        std::vector<vec3>		mLinePoints;
        
        std::vector<vec3>		mProfPoints;
        
        int32_t mNumSegs;
        BSpline3f				mBSpline;
        int32_t length;
        mat4 mModelMatrix;
        int32_t radius;
    };
}