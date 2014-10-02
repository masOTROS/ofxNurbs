// Based on implementations for cinder:
// https://gist.github.com/num3ric/1324138
// http://itr0510.blogspot.com.ar/2011/03/cindernurbs.html

#include "testApp.h"

#define IMAGE_WIDTH 100
#define IMAGE_HEIGHT 100
#define SURFACE_WIDTH 200
#define SURFACE_HEIGHT 200
#define SURFACE_DEPTH 100

#define U_POINTS	4
#define V_POINTS	4
#define U_ORDER		4
#define V_ORDER		4
#define U_KNOTS		(U_POINTS+U_ORDER)
#define V_KNOTS		(V_POINTS+V_ORDER)
GLfloat ctlpoints[U_POINTS][V_POINTS][3];
GLfloat knotsU[]={0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLfloat knotsV[]={0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
//GLfloat knotsU[]={0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
//GLfloat knotsV[]={0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
//GLfloat knotsU[]={0.0, 0.0, 0.0, 0.0, 0.33, 0.66, 1.0, 1.0, 1.0, 1.0};
//GLfloat knotsV[]={0.0, 0.0, 0.0, 0.0, 0.33, 0.66, 1.0, 1.0, 1.0, 1.0};
//GLfloat knotsU[]={0.0, 0.0, 0.0, 0.0, 0.0, 0.33, 0.66, 1.0, 1.0, 1.0, 1.0, 1.0};
//GLfloat knotsV[]={0.0, 0.0, 0.0, 0.0, 0.0, 0.33, 0.66, 1.0, 1.0, 1.0, 1.0, 1.0};

/*Note that a gluNurbsSurface with sKnotCount knots in the u direction and
tKnotCount knots in the v direction with orders sOrder and tOrder must
have (sKnotCount - sOrder) x (tKnotCount - tOrder) control points.*/
//http://www.glprogramming.com/red/chapter12.html

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
	ofEnableAlphaBlending();
	
	/*image.allocate(IMAGE_WIDTH,IMAGE_HEIGHT,OF_IMAGE_COLOR);
	for(int y=0;y<IMAGE_HEIGHT;y++){
		for(int x=0;x<IMAGE_WIDTH;x++){
			int index=x+y*IMAGE_WIDTH;
			image[index*3+0]=128;
			image[index*3+1]=ofMap(x,0,IMAGE_WIDTH,0,255);
			image[index*3+2]=ofMap(y,0,IMAGE_HEIGHT,0,255);
		}
	}*/
	ofLoadImage(image,"test.png");
	image.setImageType(OF_IMAGE_COLOR);

	// this sets the camera's distance from the object
	cam.setDistance(250);

	//Init lights
	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat mat_color[] = {1.0, 1.0, 1.0};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv( GL_FRONT, GL_DIFFUSE,	mat_color);
	
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL); //GLU_FILL, GLU_OUTLINE_POLYGON, GLU_OUTLINE_PATCH
	//gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);
	for (int u = 0; u < U_POINTS; u++) {
		for (int v = 0; v < V_POINTS; v++) {
			ctlpoints[u][v][0] = ofMap(u,0,U_POINTS-1,0.0f,1.0f);
			ctlpoints[u][v][1] = ofMap(v,0,V_POINTS-1,0.0f,1.0f);
			ctlpoints[u][v][2] = 0.0f;
		}
	}

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
	ofBackgroundGradient(ofColor(200),ofColor(0));

	cam.begin();

	ofPushMatrix();
	ofScale(SURFACE_WIDTH,SURFACE_HEIGHT,SURFACE_DEPTH);
	ofTranslate(-0.5,-0.5);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_AUTO_NORMAL);
        glBindTexture(GL_TEXTURE_2D, textures[0]);  
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		gluBeginSurface(theNurb);
		gluNurbsSurface(theNurb, U_KNOTS, knotsU, V_KNOTS, knotsV, V_POINTS * 3, 3, &ctlpoints[0][0][0], U_POINTS, V_POINTS, GL_MAP2_TEXTURE_COORD_2);
		//gluNurbsSurface(theNurb, U_KNOTS, knotsU, V_KNOTS, knotsV, V_POINTS * 3, 3, &ctlpoints[0][0][0], U_POINTS, V_POINTS, GL_MAP2_NORMAL);
		//gluNurbsSurface(theNurb, U_KNOTS, knotsU, V_KNOTS, knotsV, V_POINTS * 3, 3, &ctlpoints[0][0][0], U_POINTS, V_POINTS, GL_MAP2_INDEX);
		gluNurbsSurface(theNurb, U_KNOTS, knotsU, V_KNOTS, knotsV, V_POINTS * 3, 3, &ctlpoints[0][0][0], U_POINTS, V_POINTS, GL_MAP2_VERTEX_3);
		gluEndSurface(theNurb);
		glFlush();  
    glDisable(GL_TEXTURE_2D);

	drawCtlPoints();

	ofPopMatrix();

	cam.end();

    glEnable(GL_TEXTURE_2D); 
        glBindTexture(GL_TEXTURE_2D, textures[0]);  
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  
        glBegin(GL_QUADS);  
            glTexCoord2f(0.0, 0.0); glVertex2f(10,20);  
            glTexCoord2f(0.0, 1.0); glVertex2f(10,20+100);  
            glTexCoord2f(1.0, 1.0); glVertex2f(10+100,20+100);  
            glTexCoord2f(1.0, 0.0); glVertex2f(10+100,20);  
        glEnd();
        glFlush();  
    glDisable(GL_TEXTURE_2D);

	if(editMode)
		ofDrawBitmapString("Edit mode.",10,10);
	else
		ofDrawBitmapString("Camera mode.",10,10);

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
	if(key=='w'){
		gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
	}
	else if(key=='f'){
		gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
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
	}
}