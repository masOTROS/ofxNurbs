#include "testApp.h"
//http://www.glprogramming.com/red/chapter12.html

#define U_POINTS	4
#define V_POINTS	4
GLfloat ctlpoints[U_POINTS][V_POINTS][3];
GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {1.0, 0.0}}, 
                        {{0.0, 1.0}, {1.0, 1.0}}};

#define X_SCALE 2.0
#define Y_SCALE 2.0
#define Z_SCALE 2.0

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
	glShadeModel(GL_FLAT);

	drawMode=GL_FILL;

	
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

	editMode=false;
	editSelection=0;
}

//--------------------------------------------------------------
void testApp::update(){
	if(!editMode){
		for (int u = 0; u < U_POINTS; u++) {
			for (int v = 0; v < V_POINTS; v++) {
				if ( (u == (U_POINTS/2-1) || u == U_POINTS/2) && (v == (V_POINTS/2-1) || v == V_POINTS/2))
					ctlpoints[u][v][2] = sin(ofGetElapsedTimef()/5);
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(50,10,10);

	int w=ofGetWidth();
	int h=ofGetHeight();
	ofPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-X_SCALE, X_SCALE, -Y_SCALE*(GLfloat)h/(GLfloat)w, 
              Y_SCALE*(GLfloat)h/(GLfloat)w, -Z_SCALE, Z_SCALE);
	else
		glOrtho(-X_SCALE*(GLfloat)w/(GLfloat)h, 
              X_SCALE*(GLfloat)w/(GLfloat)h, -Y_SCALE, Y_SCALE, -Z_SCALE, Z_SCALE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glRotatef(85.0, 1.0, 1.0, 1.0);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D); 
        glBindTexture(GL_TEXTURE_2D, textures[0]);  
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEvalMesh2(drawMode, 0, 20, 0, 20);
	glFlush();  
    glDisable(GL_TEXTURE_2D);

	drawCtlPoints();

	ofPopMatrix();

	ofDrawBitmapString("Esto no se dibuja.", mouseX,mouseY);

	ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
}

//--------------------------------------------------------------
void testApp::drawCtlPoints(){
	glColor3f(0.6, 0.6, 0.6);
    for (int u=0; u<U_POINTS; u++) {
        glBegin(GL_LINE_STRIP);
        for (int v=0; v<V_POINTS; v++) {
            glVertex3f(ctlpoints[u][v][0],
                       ctlpoints[u][v][1],
                       ctlpoints[u][v][2]);
        }
        glEnd();
    } 
    for (int v=0; v<V_POINTS; v++) {
        glBegin(GL_LINE_STRIP);
        for (int u=0; u<U_POINTS; u++) {
            glVertex3f(ctlpoints[u][v][0],
                       ctlpoints[u][v][1],
                       ctlpoints[u][v][2]);
        }
        glEnd();
    } 
    glColor3f(1.0, 1.0, 1.0);
	glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int v=0; v<U_POINTS; v++) {
        for (int u=0; u<V_POINTS; u++) {
            glVertex3f(ctlpoints[u][v][0],
                       ctlpoints[u][v][1],
                       ctlpoints[u][v][2]);
        }
    } 
	if(editMode){
		int selU=editSelection%U_POINTS;
		int selV=editSelection/U_POINTS;
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(ctlpoints[selU][selV][0], ctlpoints[selU][selV][1], ctlpoints[selU][selV][2]);
	}
    glEnd();
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
		if(editMode){
			cam.disableMouseInput();
		}
		else{
			cam.enableMouseInput();
		}
	}
	else if(key=='r'){
		cam.reset();
	}
	else if(key==OF_KEY_RIGHT){
		editSelection=((editSelection+1)%(U_POINTS*V_POINTS));
		int selU=editSelection%U_POINTS;
		int selV=editSelection/U_POINTS;
		cout<<"Selected "<<editSelection<<":"<<ctlpoints[selU][selV][0]<<","<<ctlpoints[selU][selV][1]<<endl;
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(editMode){
		int selU=editSelection%U_POINTS;
		int selV=editSelection/U_POINTS;
		ctlpoints[selU][selV][0]=(float)x/ofGetWidth();
		ctlpoints[selU][selV][1]=1.0f-(float)y/ofGetHeight();

		//MESH
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctlpoints[0][0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 
           0, 1, 4, 2, &texpts[0][0][0]);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glShadeModel(GL_FLAT);
	}
}