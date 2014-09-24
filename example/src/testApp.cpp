#include "testApp.h"

#define U_POINTS 4
#define V_POINTS 4
GLfloat ctlpoints[U_POINTS][V_POINTS][3];
GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

GLvoid nurbsError(GLenum errorCode){
	const GLubyte *estring;
	
	estring = gluErrorString(errorCode);
//	fprintf (stderr, “Nurbs Error: %s\n”, estring);
	exit (0);
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowPosition(100,100);
	ofSetLogLevel(OF_LOG_VERBOSE);

	image.loadImage("test.png");

	// this sets the camera's distance from the object
	cam.setDistance(1000);

	//Init lights
	/*GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat mat_color[] = {1.0, 1.0, 1.0};
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv( GL_FRONT, GL_DIFFUSE,	mat_color);*/

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL); //GLU_FILL, GLU_OUTLINE_POLYGON, GLU_OUTLINE_PATCH
	//gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);
	for (int u = 0; u < U_POINTS; u++) {
		for (int v = 0; v < V_POINTS; v++) {
			ctlpoints[u][v][0] = 2.0*((GLfloat)u - 1.5);
			ctlpoints[u][v][1] = 2.0*((GLfloat)v - 1.5);
			
			if ( (u == 1 || u == 2) && (v == 1 || v == 2))
				ctlpoints[u][v][2] = 6.0;
			else
				ctlpoints[u][v][2] = 0.0;
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
	for (int u = 0; u < U_POINTS; u++) {
		for (int v = 0; v < V_POINTS; v++) {
			if ( (u == 1 || u == 2) && (v == 1 || v == 2))
				ctlpoints[u][v][2] = 6.0*cos(ofGetElapsedTimef()/5);
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(200),ofColor(0));

	cam.begin();

	ofPushMatrix();
	ofScale(100,100,100);
	glColor3f(1.0, 1.0, 1.0);
	image.bind();
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 
					8, knots, 8, knots,
					4 * 3, 3, &ctlpoints[0][0][0], 
					4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);
	image.unbind();

	drawCtlPoints();

	ofPopMatrix();

	cam.end();

	image.draw(0,0,100,100);

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
    glEnd();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='w'){
		gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
	}
	else if(key=='f'){
		gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	}
}