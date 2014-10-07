#include "ofxNurbs.h"

//--------------------------------------------------------------
void ofxNurbs::setup(int u_points, int v_points){	
	corners[0].x = 0.0;
	corners[0].y = 0.0;
	
	corners[1].x = 1.0;
	corners[1].y = 0.0;
	
	corners[2].x = 1.0;
	corners[2].y = 1.0;
	
	corners[3].x = 0.0;
	corners[3].y = 1.0;
	
	ofUnregisterMouseEvents(this);
	editMode=false;
}
//--------------------------------------------------------------
void ofxNurbs::enableEditMode(){
	ofRegisterMouseEvents(this);
	editMode=true;
}
//--------------------------------------------------------------
void ofxNurbs::disableEditMode(){
	ofUnregisterMouseEvents(this);
	editMode=false;
}
//--------------------------------------------------------------
void ofxNurbs::toggleEditMode(){
	editMode=!editMode;
	if(editMode)
		enableEditMode();
	else
		disableEditMode();
}
//--------------------------------------------------------------
bool ofxNurbs::getEditMode(){
	return editMode;
}
//--------------------------------------------------------------
void ofxNurbs::draw(){
	if (editMode) {
		ofPushStyle();
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofRect(x, y, width, height);
		ofPopStyle();
	}
}
//--------------------------------------------------------------
void ofxNurbs::end(){
	glPopMatrix();
}
//--------------------------------------------------------------
void ofxNurbs::save(string saveFile){
	ofxXmlSettings XML;
	XML.clear();
	XML.addTag("corners");
	XML.pushTag("corners");
	
	
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.setValue("corner:x",corners[i].x, t);
		XML.setValue("corner:y",corners[i].y, t);
	}
	XML.saveFile(saveFile);
}
//--------------------------------------------------------------
void ofxNurbs::load(string loadFile){
	ofxXmlSettings XML;
	if( !XML.loadFile(loadFile) ){
		ofLog(OF_LOG_ERROR, "ofxGLWarper : xml file not loaded. Check file path.");
	}
	
	if(!XML.tagExists("corners")){
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. No \"corners\" tag found");
		return;
	}
	XML.pushTag("corners");
	if (XML.getNumTags("corner")<4 ) {
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. less than 4 \"corner\" tags found");
		return;	
	}
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.pushTag("corner", i);
		if (XML.tagExists("x") && XML.tagExists("y")){
			corners[i].x = XML.getValue("x", double(1.0));
			corners[i].y = XML.getValue("y", double(1.0));
		}
		XML.popTag();
	}

	//UPDATE MESH
	ofLog(OF_LOG_WARNING, "ofxGLWarper : xml file loaded OK!.");
}
//--------------------------------------------------------------
void ofxNurbs::saveToXml(ofxXmlSettings &XML){
	XML.clear();
	XML.addTag("corners");
	XML.pushTag("corners");
	
	
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.setValue("corner:x",corners[i].x, t);
		XML.setValue("corner:y",corners[i].y, t);
	}
	XML.popTag();
}
//--------------------------------------------------------------
void ofxNurbs::loadFromXml(ofxXmlSettings &XML){	
	if(!XML.tagExists("corners")){
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. No \"corners\" tag found");
		return;
	}
	XML.pushTag("corners");
	if (XML.getNumTags("corner")<4 ) {
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. less than 4 \"corner\" tags found");
		return;
	}
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.pushTag("corner", i);
		if (XML.tagExists("x") && XML.tagExists("y")){
			corners[i].x = XML.getValue("x", double(1.0));
			corners[i].y = XML.getValue("y", double(1.0));
		}
		XML.popTag();
	}
	XML.popTag();

	//UPDATE MESH
	ofLog(OF_LOG_WARNING, "ofxGLWarper : xml object loaded OK!.");
	
}
//--------------------------------------------------------------
void ofxNurbs::mouseDragged(ofMouseEventArgs &args){
}
//--------------------------------------------------------------
void ofxNurbs::mousePressed(ofMouseEventArgs &args){
}
//--------------------------------------------------------------
void ofxNurbs::mouseReleased(ofMouseEventArgs &args){
}
//--------------------------------------------------------------
void ofxNurbs::mouseMoved(ofMouseEventArgs &args){
}