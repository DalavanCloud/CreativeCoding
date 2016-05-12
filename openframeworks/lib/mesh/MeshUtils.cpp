
#include "MeshUtils.h"

void MeshUtils::enableScreenshot(string name) {
    _name = name;
    
    //register for event here
    
    ofAddListener(ofEvents().keyPressed, this, &MeshUtils::onKeyPressed);
}

void MeshUtils::enableScreenshot(string name, char key) {
    _screenshotKey = key;
    
    enableScreenshot(name);
}

void MeshUtils::disableScreenshot() {
    ofRemoveListener(ofEvents().keyPressed, this, &MeshUtils::onKeyPressed);
}

void MeshUtils::onKeyPressed(ofKeyEventArgs& eventArgs) {
    
    if(eventArgs.key == _screenshotKey)  {
        string n = "../../../screenshots/" + _name + "_" + ofGetTimestampString() + ".png";
        ofSaveScreen(n);
        cout << "Screenshot Saved : '" + n  + "'"<< endl;
    }
}

/*******************  General Util APIs        *******************/


ofVec3f meshGetRandomPointOnSphere(ofVec3f center, float radius) {
    float u = ofRandomf();
    float v = ofRandomf();
    float theta = 2 * PI * u;
    float phi = acos(2 * v - 1);
    float x = center.x + (radius * sin(phi) * cos(theta));
    float y = center.y + (radius * sin(phi) * sin(theta));
    float z = center.z + (radius * cos(phi));
    
    return ofVec3f(x, y, z);
}

vector<ofVec3f> meshGetRandomPointsOnSphere(ofVec3f center, float radius, int number) {
    vector<ofVec3f>points;
    
    for(int i = 0; i < number; i++) {
        points.push_back(meshGetRandomPointOnSphere(center, radius));
    }
    
    return points;
}

ofVec3f meshGetRandomPointInSphere(ofVec3f center, float radius) {

    ofVec3f out = meshGetRandomPointOnSphere(center, radius);
    
    ofVec3f dir = center - out;
    dir *= ofRandomf();
    
    
    return center + dir;
}

vector<ofVec3f> meshGetRandomPointsInSphere(ofVec3f center, float radius, int number) {
    vector<ofVec3f>points;
    
    for(int i = 0; i < number; i++) {
        points.push_back(meshGetRandomPointInSphere(center, radius));
    }
    
    return points;
}

ofVec3f meshGetRandomPointInBounds(const ofRectangle & bounds) {
    return meshGetRandomPointInBounds(bounds, 0.0);
}

ofVec3f meshGetRandomPointInBounds(const ofRectangle & bounds, float depth) {
    float x = ofRandom(bounds.x, bounds.x + bounds.width);
    float y = ofRandom(bounds.y, bounds.y + bounds.height);
    
    
    float z = (depth == 0.0)? 0.0 : ofRandom(-depth / 2, depth / 2);
    
    return ofVec3f(x, y, z);
}


vector<ofVec3f> meshGetRandomPointsInBounds(const ofRectangle & bounds, uint number, float depth) {
    vector<ofVec3f>points;
    
    for(int i = 0; i < number; i++) {
        points.push_back(meshGetRandomPointInBounds(bounds, depth));
    }
    
    return points;
}


vector<ofVec3f> meshGetRandomPointsInBounds(const ofRectangle & bounds, uint number) {
    return meshGetRandomPointsInBounds(bounds, number, 0.0);
}

//note, could pass in a vector to populate, so larger vectors dont have to be copied
ofRectangle meshGetBoundsWithPadding(const ofRectangle & bounds, float padding) {
    float _x = bounds.x + padding;
    float _width = bounds.width - (padding * 2);
    float _y = bounds.y + padding;
    float _height = bounds.height - (padding * 2);
    
    return ofRectangle(_x, _y, _width, _height);
}


//https://forum.openframeworks.cc/t/more-utils/1413/2
float meshConstrain(float amt, float low, float high) {
    return (amt < low) ? low : ((amt > high) ? high : amt);
}


float meshGetAngleOfLine(ofVec3f p1, ofVec3f p2) {
    float dy = p2.y - p1.y;
    float dx = p2.x - p1.x;
    
    return atan2(dy,dx);
}

//angle is in radians
ofVec3f meshGetPointOnCircle(ofVec3f center, float radius, float angleInRadians) {
    
    ofVec3f out;
    out.x = (cos(angleInRadians) * radius) + center.x;
    out.y = (sin(angleInRadians) * radius) + center.y;
    
    return out;
}

ofVec3f meshGetPointOnLine(ofVec3f p1, ofVec3f p2, float distance) {
    //float angle = p1.angleRad(p2);
    
    float angle = meshGetAngleOfLine(p1, p2);
    
    return meshGetPointOnCircle(p1, distance, angle);
}

ofVec3f meshGetPointOnCircleAlongLing(ofVec3f center1, float radius, ofVec3f center2) {
    return meshGetPointOnLine(center1, center2, radius);
}

mSign sign(float n) {
    if (n < 0) {
        return M_NEGATIVE;
    } else if (n > 0) {
        return M_POSITIVE;
    }
    
    return M_ZERO;
}

//It is 0 on the line, and +1 on one side, -1 on the other side.
//http://stackoverflow.com/a/3461533
mPosition mGetOrientationOfPointToLine(const ofVec3f & v1, const ofVec3f & v2, const ofVec3f & p) {
    float position = ((v2.x - v1.x) * (p.y - v1.y) - (v2.y - v1.y) * (p.x - v1.x));
    
    mSign s = sign(position);
    
    if(s == M_POSITIVE) {
        return M_LEFT;
    } else if (s == M_NEGATIVE) {
        return M_RIGHT;
    }
    
    return M_CENTER;
}

int mFindLeftMostPointIndex(const vector<ofVec3f> & points) {
    ofVec3f leftMost = points[0];
    int leftMostIndex = 0;
    
    int size = points.size();
    
    for(int i = 1; i < size; i++){
        
        ofVec3f p = points[i];
        if(p.x < leftMost.x) {
            leftMost = p;
            leftMostIndex = i;
        }
    }
    
    return leftMostIndex;
}


//note, we could pass in out vector by reference for performance with large sets
vector<ofVec3f> mFindConvexHull(const vector<ofVec3f> & points) {
    //check points length;
    ofVec3f leftMost = points[mFindLeftMostPointIndex(points)];
    
    ofVec3f endPoint;
    vector<ofVec3f>pointsOnHull;
    
    ofVec3f pointOnHull = leftMost;
    
    int size = points.size();
    
    do {
        pointsOnHull.push_back(pointOnHull);
        endPoint = points[0];
        
        for(int j = 1; j < size; j++){
            
            if((endPoint == pointOnHull) ||
               mGetOrientationOfPointToLine(pointOnHull, endPoint, points[j]) == M_LEFT
               ){
                
                endPoint = points[j];
            }
        }
        //i++;
        pointOnHull = endPoint;
        
    } while(!(endPoint == pointsOnHull[0]));
    
    return pointsOnHull;
}

