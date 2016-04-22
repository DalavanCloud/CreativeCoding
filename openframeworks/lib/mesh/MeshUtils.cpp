
#include "MeshUtils.h"

void MeshUtils::enableScreenShot(string dirName) {
    _dirName = dirName;
    
    //register for event here
    
    ofAddListener(ofEvents().keyPressed, this, &MeshUtils::onKeyPressed);
}

void MeshUtils::enableScreenShot(string dirName, char key) {
    _screenshotKey = key;
    
    enableScreenShot(dirName);
}

void MeshUtils::disableScreenShot() {
    ofRemoveListener(ofEvents().keyPressed, this, &MeshUtils::onKeyPressed);
}

void MeshUtils::onKeyPressed(ofKeyEventArgs& eventArgs) {
    
    if(eventArgs.key == _screenshotKey)  {
        string n = "../../../screenshots/" + _dirName + "_" + ofGetTimestampString() + ".png";
        ofSaveScreen(n);
        cout << "Screenshot Saved : '" + n  + "'"<< endl;
    }
}