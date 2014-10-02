#include "testApp.h"
//http://www.glprogramming.com/red/chapter12.html

#define U_POINTS	4
#define V_POINTS	4
GLfloat ctlpoints[U_POINTS][V_POINTS][3];
GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {1.0, 0.0}}, 
                        {{0.0, 1.0}, {1.0, 1.0}}};

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowPosition(100,100);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableAlphaBlending();

	//CTLPOINTS
	for (int u = 0; u < U_POINTS; u++) {
		for (int v = 0; v < V_POINTS; v++) {
			ctlpoints[u][v][0] = ofMap(u,0,U_POINTS-1,-1.0f,1.0f);
			ctlpoints[u][v][1] = ofMap(v,0,V_POINTS-1,-1.0f,1.0f);
			ctlpoints[u][v][2] = 0.0f;
		}
	}
	
	//MESH
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctlpoints[0][0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 
           0, 1, 4, 2, &texpts[0][0][0]);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glShadeModel (GL_FLAT);

	
	//TEXTURE
	ofLoadImage(image,"test.png");
	image.setImageType(OF_IMAGE_COLOR);

	glGenTextures(1, &textures[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth() , image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getPixels());
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(50,10,10);

	int w=ofGetWidth();
	int h=ofGetHeight();
	ofPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float orthogonalityX=4.0*sin(ofGetElapsedTimef());
	float orthogonalityY=4.0;
	float orthogonalityZ=4.0;
	if (w <= h)
		glOrtho(-orthogonalityX, orthogonalityX, -orthogonalityY*(GLfloat)h/(GLfloat)w, 
              orthogonalityY*(GLfloat)h/(GLfloat)w, -orthogonalityZ, orthogonalityZ);
	else
		glOrtho(-orthogonalityX*(GLfloat)w/(GLfloat)h, 
              orthogonalityX*(GLfloat)w/(GLfloat)h, -orthogonalityY, orthogonalityY, -orthogonalityZ, orthogonalityZ);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glRotatef(85.0, 1.0, 1.0, 1.0);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D); 
        glBindTexture(GL_TEXTURE_2D, textures[0]);  
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	glFlush();  
    glDisable(GL_TEXTURE_2D);

	ofPopMatrix();

	ofDrawBitmapString("Hola a todos.", mouseX,mouseY);

	ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}