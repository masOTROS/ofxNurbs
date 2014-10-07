#include "testApp.h"
#include "homography.h"
//http://www.glprogramming.com/red/chapter12.html

#define IMAGE_WIDTH 150
#define IMAGE_HEIGHT 150
#define IMAGE_DEPTH 150

#define CTLPOINT_RADIUS 0.05f

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}}, 
                        {{1.0, 0.0}, {1.0, 1.0}}};

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowPosition(100,100);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableAlphaBlending();
	ofSetCircleResolution(50);

	resetMesh();

	src[0]=ofPoint(-1,-1); src[1]=ofPoint(1,-1);
	src[2]=ofPoint(-1,1); src[3]=ofPoint(1,1);
	resetQuad();

	drawMode=GL_FILL;

	//TEXTURE
	ofLoadImage(image,"test.png");
	image.setImageType(OF_IMAGE_COLOR);

	texture.allocate(image.getWidth(),image.getHeight(),GL_RGB);
	texture.loadData(image.getPixels(),image.getWidth(),image.getHeight(),GL_RGB);

	glGenTextures(1, &textures[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth() , image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getPixels());	

	editMode=false;
	editCorners=true;
	editMeshSelection=0;
	editQuadSelection=0;
}

//--------------------------------------------------------------
void testApp::updateQuad(){
	quadWarping=findHomography(src,corners);
	quadWarpingT=ofMatrix4x4::getTransposedOf(quadWarping);
	quadWarpingI=ofMatrix4x4::getInverseOf(quadWarping);
}

//--------------------------------------------------------------
void testApp::updateMesh(){
	GLfloat ctlpoints[U_POINTS][V_POINTS][POINT_DIM];
	for (int u = 0; u < U_POINTS; u++) {
		for (int v = 0; v < V_POINTS; v++) {
			ctlpoints[u][v][0] = controlPoints[u][v].x;
			ctlpoints[u][v][1] = controlPoints[u][v].y;
			ctlpoints[u][v][2] = controlPoints[u][v].z;
		}
	}
	//MESH
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, POINT_DIM, U_POINTS,
           0, 1, V_POINTS*POINT_DIM, V_POINTS, &ctlpoints[0][0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, TEX_DIM, TEX_POINTS, 
           0, 1, TEX_POINTS*TEX_DIM, TEX_POINTS, &texpts[0][0][0]);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(GRID_RES, 0.0, 1.0, GRID_RES, 0.0, 1.0);
	glShadeModel(GL_FLAT);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(200),ofColor(0));
	
	ofSetColor(255);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()*0.5,ofGetHeight()*0.5);
	ofScale(IMAGE_WIDTH,IMAGE_HEIGHT,IMAGE_DEPTH);
	
	ofPushMatrix();

	glMultMatrixf(quadWarpingT.getPtr());
	glEnable(GL_TEXTURE_2D); 
		glBindTexture(GL_TEXTURE_2D, textures[0]);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEvalMesh2(drawMode, 0, GRID_RES, 0, GRID_RES);
	glFlush();  
    glDisable(GL_TEXTURE_2D);

	ofPopMatrix();

	if(editMode)
		drawCtlPoints();

	ofPopMatrix();

	ofSetColor(255);
	if(editMode)
		ofDrawBitmapString("Edit mode.",10,10);
	else
		ofDrawBitmapString("Camera mode.",10,10);

	texture.draw(10,20,100,100);

	ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
}

//--------------------------------------------------------------
void testApp::drawCtlPoints(){
	ofPushStyle();
	if(editCorners){
		for (int i=0; i<TEX_CORNERS; i++) {
			for(int j=i; j<TEX_CORNERS; j++){
				ofSetColor(255,200);
				ofLine(corners[i],corners[j]);
			}
			ofSetColor(255);
			ofFill();
			ofCircle(corners[i],0.02f);
			ofSetColor(255,150);
			ofNoFill();
			ofCircle(corners[i],CTLPOINT_RADIUS);
		}
		ofSetColor(0,0,255);
		ofFill();
        ofCircle(corners[editQuadSelection],0.03f);
	}
	else{
		for (int u=0; u<U_POINTS; u++) {
			for (int v=0; v<V_POINTS; v++) {
				ofSetColor(255,200);
				if((u+1)<U_POINTS)
					ofLine(quadWarping*controlPoints[u][v],quadWarping*controlPoints[u+1][v]);
				if((v+1)<V_POINTS)
					ofLine(quadWarping*controlPoints[u][v],quadWarping*controlPoints[u][v+1]);

				ofSetColor(255);
				ofFill();
				ofCircle(quadWarping*controlPoints[u][v],0.01f);
				ofSetColor(255,150);
				ofNoFill();
				ofCircle(quadWarping*controlPoints[u][v],CTLPOINT_RADIUS);
			}
		}
		int selU=editMeshSelection%U_POINTS;
		int selV=editMeshSelection/U_POINTS;
		ofSetColor(255,0,0);
		ofFill();
        ofCircle(quadWarping*controlPoints[selU][selV],0.02f);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::resetQuad(){
	corners[0]=ofPoint(-1,-1); corners[1]=ofPoint(1,-1);
	corners[2]=ofPoint(-1,1); corners[3]=ofPoint(1,1);
	updateQuad();
}
//--------------------------------------------------------------
void testApp::resetMesh(){
	for (int u = 0; u < U_POINTS; u++) {
		for (int v = 0; v < V_POINTS; v++) {
			controlPoints[u][v].set(ofMap(u,0,U_POINTS-1,-1.0f,1.0f),ofMap(v,0,V_POINTS-1,-1.0f,1.0f));
		}
	}
	updateMesh();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='1'){
		drawMode=GL_FILL;
	}
	else if(key=='2'){
		drawMode=GL_LINE;
	}
	else if(key=='3'){
		drawMode=GL_POINT;
	}
	else if(key=='e'){
		editMode=!editMode;
	}
	else if(key=='c'){
		editCorners=!editCorners;
	}
	else if(key=='r'){
		resetQuad();
		resetMesh();
	}
	else if(key==OF_KEY_RIGHT){
		editMeshSelection=((editMeshSelection+1)%(U_POINTS*V_POINTS));
		int selU=editMeshSelection%U_POINTS;
		int selV=editMeshSelection/U_POINTS;
		cout<<"Selected "<<editMeshSelection<<":"<<controlPoints[selU][selV]<<endl;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	if(editMode){
		if(editCorners){
			ofPoint pos(mouseToMesh(x,y));
			for (int i=0; i<TEX_CORNERS; i++) {
				if(pos.distance(corners[i])<CTLPOINT_RADIUS){
					editQuadSelection=i;
					return;
				}
			}
		}
		else{
			ofPoint pos(quadWarpingI*mouseToMesh(x,y));
			for (int u = 0; u < U_POINTS; u++) {
				for (int v = 0; v < V_POINTS; v++) {
					if(pos.distance(controlPoints[u][v])<CTLPOINT_RADIUS){
						editMeshSelection=u+v*U_POINTS;
						return;
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(editMode){
		if(editCorners){
			ofPoint pos(mouseToMesh(x,y));
			corners[editQuadSelection].set(pos);
			updateQuad();
		}
		else{
			ofPoint pos(quadWarpingI*mouseToMesh(x,y));
			int selU=editMeshSelection%U_POINTS;
			int selV=editMeshSelection/U_POINTS;
			controlPoints[selU][selV].set(pos);
			updateMesh();
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
ofPoint testApp::mouseToMesh(int x, int y){
	float xRatio=0.5*ofGetWidth()/IMAGE_WIDTH;
	float yRatio=0.5*ofGetHeight()/IMAGE_HEIGHT;
	return ofPoint(ofMap(x,0,ofGetWidth(),-xRatio,xRatio),ofMap(y,0,ofGetHeight(),-yRatio,yRatio));
}