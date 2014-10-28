#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Thorn.h"
#include "Body.h"
#include "OtherBody.h"
#include "cinder/Arcball.h"
#include "cinder/Camera.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Shader.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace sc;

class SeaCucumberApp : public AppNative {
public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    void resize();

private:
    CameraPersp mCam;
    
//    gl::GlslProgRef mSinShader;
    //Thorn mThorn;
    //Body mBody = *new Body(20);
    BodyRef mBody = Body::create(20);
    //OtherBodyRef mOtherBody = OtherBody::create(20);
    
    bool isWireframe;
    
    gl::GlslProgRef mFlatShading;
//    gl::BatchRef mTorus;
};

void SeaCucumberApp::setup()
{
    setWindowSize(600, 600 );

	mCam.lookAt( vec3( 0, 0, 50 ), vec3(0,0,0) );
    
    isWireframe = true;
    
    
    
    mFlatShading = gl::GlslProg::create(loadAsset("Flat.vert"), loadAsset("Flat.frag"));
    //mTorus = gl::Batch::create(geom::Torus(), mFlatShading);
}

void SeaCucumberApp::resize()
{
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	gl::setMatrices( mCam );
}

void SeaCucumberApp::mouseDown( MouseEvent event )
{
}

void SeaCucumberApp::update()
{
    //mThorn.update();
    mBody->update();
    //mOtherBody->update();
}

void SeaCucumberApp::draw()
{
	gl::clear( Color( 0,0,0 ));
    gl::setMatrices( mCam );
    
    
    {
    gl::ScopedMatrices sm;
    mFlatShading->bind();
    mBody->draw();
    }
    
}

CINDER_APP_NATIVE( SeaCucumberApp, RendererGl )
