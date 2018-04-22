/*
    Copyright Mike Chambers 2018
    mikechambers@gmail.com

    http://www.mikechambers.com
    https://github.com/mikechambers/CreativeCoding

    Released un an MIT License
    https://github.com/mikechambers/CreativeCoding/blob/master/LICENSE.txt
*/

#include "ofApp.h"

#include "ofxSyphonClient.h"
#include "MeshUtils.h"
#include "ImageLoader.h"


MeshUtils utils;
ofxSyphonServer syphon;

const string APP_NAME = "PerlinPlay";
const string IMG_PATH = "../../../images/gradient_8.jpg";
const bool CLEAR_BETWEEN_FRAMES = false;

ofRectangle windowBounds;
ofRectangle renderBounds;

bool paused = false;

ofVboMesh mesh;
ImageLoader image;

ofFbo renderer;






//--------------------------------------------------------------
void ofApp::setup(){
    //utils.enableScreenshot(APP_NAME, 's');
    syphon.setName(APP_NAME);

    //bounds = ofGetWindowRect();
    
    windowBounds = ofGetWindowRect();
    renderBounds = ofRectangle(0,0, 1920, 1080);
    
    renderer.allocate(renderBounds.width, renderBounds.height, GL_RGB);
    ofEnableAlphaBlending();
    
    renderer.begin();
    ofClear(ofColor::white);
    //ofSetColor(ofColor::white);
    //ofDrawRectangle(bounds);
    renderer.end();
    
    image.load(IMG_PATH);
    image.resize(renderBounds.width, renderBounds.height);
    
    ofSetBackgroundAuto(CLEAR_BETWEEN_FRAMES);
    ofSetBackgroundColor(ofColor::white);
    
    mesh.enableColors();
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
}
float t = 0;
//--------------------------------------------------------------
void ofApp::update(){
    if(paused) {
        return;
    }
    
    ofVec3f center = renderBounds.getCenter();
    int STEPS = 50;
    
    bool CIRCLE = false;
    int RADIUS = 600;
    int OPACITY = 20;
    
    mesh.clear();

    float radius = RADIUS;
    float centerY = center.y;

    for(int i = 0; i < STEPS; i++) {
        
        ofVec3f p;
        float distortion = RADIUS * ofNoise(i * 0.008, t * 0.005); //, t * i * 0.0001
        if(CIRCLE) {
            float ang = ofMap(i, 0, STEPS -  1,  0, M_TWO_PI );
            //float radius = 200 * ofNoise(i * 0.2, t * 0.0005);

            radius = distortion * ofNoise(ofGetSystemTimeMicros() * 0.001);
            p = mGetPointOnCircle(center, radius, ang);
        } else {
            float ang = ofMap(i, 0, STEPS - 1,  0, renderBounds.width);
            //float distortion = 100 * ofNoise(ang * 0.003, t * 0.005);
            p = ofVec3f(ang, (centerY + distortion)  / 2);
        }

        mesh.addVertex(p);
        mesh.addColor(ofColor(image.getColor(p),  OPACITY));
    }
    
    t++;
    
    renderer.begin();
    mesh.draw();
    renderer.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(paused) {
        syphon.publishScreen();
        return;
    }
    
    //mesh.draw();
    /*
    if (config.SCALE_CANVAS) {
        var maxW = window.innerWidth;
        var maxH = window.innerHeight;
        
        //http://www.ajaxblender.com/howto-resize-image-proportionally-using-javascript.html
        if (canvasH > maxH ||
            canvasW > maxW) {
            
            var ratio = canvasH / canvasW;
            
            if (canvasW >= maxW && ratio <= 1) {
                canvasW = maxW;
                canvasH = canvasW * ratio;
            } else if (canvasH >= maxH) {
                canvasH = maxH;
                canvasW = canvasH / ratio;
            }
        }
    }
     */
    
    float tW = windowBounds.height;
    float tH = (tW * renderBounds.height) / renderBounds.width;
    cout  << tW << ":" << tH << endl;
    
    float offset = windowBounds.getCenter().y - (tH / 2);

    renderer.draw(0, offset, tW, tH);
    
    /*
    ofNoFill();
    ofSetColor(ofColor::red);
    ofDrawRectangle(0,offset,tW, tH);
    */
    
    syphon.publishScreen();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        paused = !paused;
    } else if(key == 's') {
        ofPixels pixels;
        renderer.readToPixels(pixels);
        string n = "../../../screenshots/" + APP_NAME + "_" + ofGetTimestampString() + ".png";
        ofSaveImage(pixels, n);
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
