// Draw an Icosahedron
// ECE4893/8893 Project 4
// Xian Wu

#include <iostream>
#include <math.h>
#include <vector>

#ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/glext.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/glext.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

using namespace std;

#define NFACE 20
#define NVERTEX 12

#define X .525731112119133606
#define Z .850650808352039932

static const float DEG2RAD = M_PI/180;
// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

// These are the 20 faces.  Each of the three entries for each
// vertex gives the 3 vertices that make the face.

static GLint tindices[NFACE][3] = {
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};



int testNumber; // Global variable indicating which test number is desired
int depth=3;
static GLfloat mycolor[4][3]={
    {1.0,0.5,0.5},{0.5,1.0,0.5},{0.5,0.5,1.0},{0.5,0.5,0.5}
};

//static GLint AF[NFACE][3]={
//    {1,4,14},{0,2,16},{1,3,18},{2,4,7},{0,3,5},
//    {4,6,15},{5,7,10},{3,6,8},{7,9,18},{8,10,19},
//    {6,9,11},{10,12,15},{11,13,19},{12,14,16},{0,13,15},
//    {5,11,14},{1,13,17},{16,18,19},{2,8,17},{9,12,17}
//};

//------------------------------------
int updateRate = 50;

// use tindices to find adjacent faces
//return array AFace[0][]={1,4,14} face0 adjacent to face 1 4 14
bool IFadjacent(GLint* face1,GLint* face2)
{
    GLint flag=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(face1[i]==face2[j])
                flag+=1;
        }
    }
    if(flag==2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

GLint** adjacentFace(GLint tind[][3],GLint N_face)
{
    bool flag[N_face][N_face];

    for(int i=0;i<N_face;i++)
        for(int j=0;j<N_face;j++)
        {
            flag[i][j]=false;
        }

    GLint** AFace = new GLint * [N_face];
    for(GLint i = 0; i < N_face; ++i)
    {
        AFace[i] = new GLint[3];
    }

    for(int i=0;i<N_face;i++)
        for(int j=0;j<3;j++)
        {
            AFace[i][j]=0;
        }
    //finding
    for(int i=0;i<N_face;i++)
    {
        for(int j=i+1;j<N_face;j++)
        {
            if (IFadjacent(tind[i],tind[j]))
            {
                flag[i][j]=true;
                flag[j][i]=true;
            }
        }
    }

    for(int i=0;i<N_face;i++)
    {
        int k=0;
        for(int j=0;j<N_face;j++)
        {
            if(flag[i][j])
            {
                AFace[i][k]=j;
                k++;
            }
        }
    }

//    for(int i=0;i<N_face;i++)
//        for(int j=0;j<3;j++)
//        {
//            std::cout<<AFace[i][j]<<"  ";
//            if (j==2)
//                std::cout<<endl;
//        }

    return AFace;

}

void drawPoly(GLint tind[][3],GLfloat vd[][3],GLint N_face)
{

    //---------------TRIANGLES------------------
      GLint ** AF;
      AF = adjacentFace(tind,N_face);

      glBegin(GL_TRIANGLES);
      int faceColorFlag[N_face];  //mark color of each face
      for(int i=0 ; i<N_face ; i++)
      {
          faceColorFlag[i]=0;
      }
      for(int i=0 ; i<N_face ; i++)
      {
 //-------------get color for face i-------------------
        int i_color=0;        //from 1~4 , thus 0 means uncolored
        for (i_color=1;i_color<5;i_color++)
        {
            bool colorOK = true;
            for(int j=0;j<3;j++)
            {
                if(faceColorFlag[AF[i][j]]==i_color)
                {
                    colorOK=false;         //one face has color i_color, abandon i_color
                    break;
                }
            }
            if (colorOK)               //once find a proper i_color, you can stop
            {
                faceColorFlag[i]=i_color;
                break;
            }
        }
        if(((i%2)==0)&&(i_color==2))
        {
            i_color=4;
        }
        glColor3f(mycolor[i_color-1][0] , mycolor[i_color-1][1] , mycolor[i_color-1][2]);
  //------------------------------------------------
        //get three points' coordinates of face i
        GLfloat v4[3];    // vertex 1
        for(int k=0; k<3 ;k++)
        {
            v4[k]=vd[tind[i][0]][k];
        }
        GLfloat v5[3];   // vertex 2
        for(int k=0; k<3 ;k++)
        {
            v5[k]=vd[tind[i][1]][k];
        }
        GLfloat v6[3];   // vertex 3
        for(int k=0; k<3 ;k++)
        {
            v6[k]=vd[tind[i][2]][k];
        }
        // connect three lines
        glVertex3fv(v4);
        glVertex3fv(v5);
        glVertex3fv(v6);
    }
    glEnd();

    for(int i=0;i<N_face;i++)
    {
        delete [] AF[i];
    }
    delete AF;

    //----------LINES----------

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for(int i=0 ; i<N_face ; i++)
    {
        //get three points' coordinates of face i
        GLfloat v1[3];    // vertex 1
        for(int k=0; k<3 ;k++)
        {
            v1[k]=vd[tind[i][0]][k];
        }
        GLfloat v2[3];   // vertex 2
        for(int k=0; k<3 ;k++)
        {
            v2[k]=vd[tind[i][1]][k];
        }
        GLfloat v3[3];   // vertex 3
        for(int k=0; k<3 ;k++)
        {
            v3[k]=vd[tind[i][2]][k];
        }
        // connect three lines
        glVertex3fv(v1);
        glVertex3fv(v2);
        glVertex3fv(v1);
        glVertex3fv(v3);
        glVertex3fv(v2);
        glVertex3fv(v3);
    }
    glEnd();

}

//-------------------------------------------

void creatDepth(vector<GLint>& vectind,vector<GLfloat>& vecvdata,GLint& N_face,GLint& N_VERTEX,int dep)
{
    GLint NF=N_face;
    GLint NV=N_VERTEX;
    vector<GLint> Vtind(vectind);
    vector<GLfloat> Vvdata(vecvdata);

    //--------------each depth: based on current NF NV Vtind Vvdata-----------------
    for(int d=0;d<dep;d++)
    {
        vector<GLint> Vtindtmp;        //to get new value for Vtind and Vvdata
        GLint midpoint[NV][NV];          //mark midpoint index in a matrix, (-1 means no midpoint between point i j yet)
        for(int i=0; i<NV ;i++)
            for(int j=0; j<NV ;j++)
            {
                midpoint[i][j]=-1;
            }
        GLint MAX_POINT=NV-1;

        //----each face extend: based on NF NV Vtind Vvdata MAX_POINT midpoint---
        for(int i=0; i<NF ;i++)
        {
            // (1) get three original vertex v1 v2 v3 of face i
            //v1 is point Vtind[i*3] ,v2 is point Vtind[i*3+1],v3 is point Vtind[i*3+2]
            //point p's coordinate is {Vvdata[p*3],Vvdata[p*3+1],Vvdata[p*3+2]}
            GLint p1=Vtind[i*3];
            GLfloat v1[3]={Vvdata[p1*3],Vvdata[p1*3+1],Vvdata[p1*3+2]};
            GLint p2=Vtind[i*3+1];
            GLfloat v2[3]={Vvdata[p2*3],Vvdata[p2*3+1],Vvdata[p2*3+2]};
            GLint p3=Vtind[i*3+2];
            GLfloat v3[3]={Vvdata[p3*3],Vvdata[p3*3+1],Vvdata[p3*3+2]};

            // (2) -------add midpoint-------------------
                  //----------- get  v4---> v1v2
            GLint p4;
            GLfloat v4[3];
            if (midpoint[p1][p2]==-1)  //add new v4
            {
                p4=MAX_POINT+1;
                midpoint[p1][p2]=p4;
                midpoint[p2][p1]=p4;
                MAX_POINT++;
                for(int k=0;k<3;k++)
                {
                    v4[k]=(v1[k]+v2[k])/2.0;      //real midpoint coordinate
                }
                // transform to unit 1 point
                GLfloat Length=sqrt(v4[0]*v4[0]+v4[1]*v4[1]+v4[2]*v4[2]);
                for(int k=0;k<3;k++)
                {
                    v4[k]=v4[k]/Length;
                }
                Vvdata.push_back(v4[0]);
                Vvdata.push_back(v4[1]);
                Vvdata.push_back(v4[2]);
            }
            else            //read v4 from midpoint[][] and Vvdata
            {
                p4=midpoint[p1][p2];
                v4[0]=Vvdata[p4*3];
                v4[1]=Vvdata[p4*3+1];
                v4[2]=Vvdata[p4*3+2];
            }
                //----------------------------
                //----------- get  v5---> v2v3
            GLint p5;
            GLfloat v5[3];
            if (midpoint[p2][p3]==-1)  //add new v5
            {
                p5=MAX_POINT+1;
                midpoint[p2][p3]=p5;
                midpoint[p3][p2]=p5;
                MAX_POINT++;
                for(int k=0;k<3;k++)
                {
                    v5[k]=(v2[k]+v3[k])/2.0;      //real midpoint coordinate
                }
                // transform to unit 1 point
                GLfloat Length=sqrt(v5[0]*v5[0]+v5[1]*v5[1]+v5[2]*v5[2]);
                for(int k=0;k<3;k++)
                {
                    v5[k]=v5[k]/Length;
                }
                Vvdata.push_back(v5[0]);
                Vvdata.push_back(v5[1]);
                Vvdata.push_back(v5[2]);
            }
            else            //read v5 from midpoint[][] and Vvdata
            {
                p5=midpoint[p2][p3];
                v5[0]=Vvdata[p5*3];
                v5[1]=Vvdata[p5*3+1];
                v5[2]=Vvdata[p5*3+2];
            }

            //-----------------------------
            //----------- get  v6---> v1v3
            GLint p6;
            GLfloat v6[3];
            if (midpoint[p1][p3]==-1)  //add new v6
            {
                p6=MAX_POINT+1;
                midpoint[p1][p3]=p6;
                midpoint[p3][p1]=p6;
                MAX_POINT++;
                for(int k=0;k<3;k++)
                {
                    v6[k]=(v1[k]+v3[k])/2.0;      //real midpoint coordinate
                }
                // transform to unit 1 point
                GLfloat Length=sqrt(v6[0]*v6[0]+v6[1]*v6[1]+v6[2]*v6[2]);
                for(int k=0;k<3;k++)
                {
                    v6[k]=v6[k]/Length;
                }
                Vvdata.push_back(v6[0]);
                Vvdata.push_back(v6[1]);
                Vvdata.push_back(v6[2]);
            }
            else            //read v5 from midpoint[][] and Vvdata
            {
                p6=midpoint[p1][p3];
                v6[0]=Vvdata[p6*3];
                v6[1]=Vvdata[p6*3+1];
                v6[2]=Vvdata[p6*3+2];
            }
            //-------------------------------------------
        // (3) new faces-------------------------
            Vtindtmp.push_back(p1);
            Vtindtmp.push_back(p4);
            Vtindtmp.push_back(p6);

            Vtindtmp.push_back(p2);
            Vtindtmp.push_back(p4);
            Vtindtmp.push_back(p5);

            Vtindtmp.push_back(p3);
            Vtindtmp.push_back(p5);
            Vtindtmp.push_back(p6);

            Vtindtmp.push_back(p4);
            Vtindtmp.push_back(p5);
            Vtindtmp.push_back(p6);
        //----------------------------------------
        }// each face
        //now you just get a new polygon, then you should update these
        vector<GLint>().swap(Vtind);
        Vtind=Vtindtmp;
        NV=NV+(NF*3)/2;
        NF=NF*4;
        vector<GLint>().swap(Vtindtmp);
    }//each depth
    //finally change vectind and vecvdata, they can be accessed outside
    vectind=Vtind;
    vecvdata=Vvdata;
    N_face=NF;
    N_VERTEX=NV;
    vector<GLint>().swap(Vtind);
    vector<GLfloat>().swap(Vvdata);
}

//-------------------------------------------
void drawPolyDepth(GLint tind[][3],GLfloat vd[][3],GLint N_face,GLint N_VERTEX,int dep)
{
    vector<GLint> vectind;
    for(int i=0;i<N_face;i++)
    {
        vectind.insert(vectind.begin()+i*3+0,tind[i][0]);
        vectind.insert(vectind.begin()+i*3+1,tind[i][1]);
        vectind.insert(vectind.begin()+i*3+2,tind[i][2]);
    }
    vector<GLfloat> vecvdata;
    for(int i=0;i<N_VERTEX;i++)
    {
        vecvdata.insert(vecvdata.begin()+i*3+0,vd[i][0]);
        vecvdata.insert(vecvdata.begin()+i*3+1,vd[i][1]);
        vecvdata.insert(vecvdata.begin()+i*3+2,vd[i][2]);
    }
    GLint nface=N_face;
    GLint nvertex=N_VERTEX;
    creatDepth(vectind,vecvdata,nface,nvertex,dep);

    GLint arr_tind[nface][3];
    for(int i=0;i<nface;i++)
        for(int j=0;j<3;j++)
        {
            arr_tind[i][j]=vectind[i*3+j];
        }

    GLfloat arr_vdata[nvertex][3];
    for(int i=0;i<nvertex;i++)
        for(int j=0;j<3;j++)
        {
            arr_vdata[i][j]=vecvdata[i*3+j];
        }

    vector<GLint>().swap(vectind);
    vector<GLfloat>().swap(vecvdata);

    drawPoly(arr_tind,arr_vdata,nface);
}

//-------------------------------------------

// Test cases.  Fill in your code for each test case
void Test1(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0,1.0,-1.0,1.0);   //x1 x2    y1 y2
//    glViewport(0.0,0.0,200.0,200.0);   //left bottom x y w h
//    glTranslatef(200, 200, 0);
//    glScalef(200.0, 200.0, 0);
    drawPoly(tindices,vdata,NFACE);

    glutSwapBuffers();
}

void Test2()
{
          glClear(GL_COLOR_BUFFER_BIT);
          // Clear the matrix
          glLoadIdentity();
          // Set the viewing transformation
          gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
          // Add some slight animation
//          static GLfloat transX = 0.0;
//          static GLfloat transY = 0.0;
//          static bool    adding = true;
          glTranslatef(250, 250, 0);
//          if (adding)
//            {
//              transX += 1.0;
//              transY += 1.0;
//              if (transX > 400) adding = false;
//            }
//          else
//            {
//              transX -= 1.0;
//              transY -= 1.0;
//              if (transX < 0.0) adding = true;
//            }
          glScalef(200.0, 200.0, 0);

          // Try rotating
          glMatrixMode(GL_MODELVIEW);
          static GLfloat rotX = 0.0;
          static GLfloat rotY = 0.0;
          glRotatef(rotX, 1.0, 0.0, 0.0);
          glRotatef(rotY, 0.0, 1.0, 0.0);
          rotX += 1.0;
          rotY -= 1.0;

          drawPoly(tindices,vdata,NFACE);
          glutSwapBuffers(); // If double buffering
}

void Test3()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0,1.0,-1.0,1.0);   //x1 x2    y1 y2
//    glViewport(0.0,0.0,200.0,200.0);   //left bottom x y w h
//    glTranslatef(200, 200, 0);
//    glScalef(200.0, 200.0, 0);
    drawPolyDepth(tindices,vdata,NFACE,NVERTEX,1);

    glutSwapBuffers();

}

void Test4()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    // Clear the matrix
    glLoadIdentity();
    // Set the viewing transformation
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glTranslatef(250, 250, 0);
    glScalef(200.0, 200.0, 0);

    // Try rotating
    glMatrixMode(GL_MODELVIEW);
    static GLfloat rotX = 0.0;
    static GLfloat rotY = 0.0;
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    rotX += 1.0;
    rotY -= 1.0;

    drawPolyDepth(tindices,vdata,NFACE,NVERTEX,1);
    glutSwapBuffers(); // If double buffering
}

void Test5()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0,1.0,-1.0,1.0);   //x1 x2    y1 y2
//    glViewport(0.0,0.0,200.0,200.0);   //left bottom x y w h
//    glTranslatef(200, 200, 0);
//    glScalef(200.0, 200.0, 0);
    drawPolyDepth(tindices,vdata,NFACE,NVERTEX,depth);

    glutSwapBuffers();

}

void Test6()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    // Clear the matrix
    glLoadIdentity();
    // Set the viewing transformation
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glTranslatef(250, 250, 0);
    glScalef(200.0, 200.0, 0);

    // Try rotating
    glMatrixMode(GL_MODELVIEW);
    static GLfloat rotX = 0.0;
    static GLfloat rotY = 0.0;
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    rotX += 1.0;
    rotY -= 1.0;

    drawPolyDepth(tindices,vdata,NFACE,NVERTEX,depth);
    glutSwapBuffers(); // If double buffering
}


//-------------------imitation-------------------

void init()
{
  //select clearing (background) color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}


void reshape(int w, int h)
{
  glViewport(0,0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)w, (GLdouble)0.0, h, (GLdouble)-w, (GLdouble)w);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void timer(int)
{
  glutPostRedisplay();
  glutTimerFunc(1000.0 / updateRate, timer, 0);
}

//--------------------------------------------------



int main(int argc, char** argv)
{
    std::cout<<endl;
  if (argc < 2)
    {
      std::cout << "Usage: icosahedron depth argv[0] and testnumber argv[1]" << endl;
      exit(1);
    }

  // Set the global test number
  testNumber = atol(argv[1]);
  depth = atol(argv[2]);
  std::cout<<testNumber<<"##"<<endl;
  std::cout<<depth<<"**"<<endl;

  // Initialize glut  and create your window here
    std::cout<<endl<<endl<<endl<<endl<<endl;
    std::cout<<endl<<endl<<endl<<endl<<endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("icosahedron");
  init();
  glEnable(GL_DEPTH_TEST);
  //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,0/1);

  // Set your glut callbacks here
  switch (testNumber) {
  case 1:
      glutDisplayFunc(Test1);
      break;
  case 2:
      glutDisplayFunc(Test2);
      break;
  case 3:
      glutDisplayFunc(Test3);
      break;
  case 4:
      glutDisplayFunc(Test4);
      break;
  case 5:
      glutDisplayFunc(Test5);
      break;
  case 6:
      glutDisplayFunc(Test6);
      break;
  default:
      glutDisplayFunc(Test6);
      break;
  }


  glutReshapeFunc(reshape);
  updateRate=10;
  glutTimerFunc(1000.0 / updateRate, timer, 0);

  // Enter the glut main loop here
  glutMainLoop();


  return 0;
}






