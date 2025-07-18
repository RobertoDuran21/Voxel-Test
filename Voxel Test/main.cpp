#include <GL/gl.h>
#include <GL/glut.h>
//#include <cstdlib>
#include <iostream>
#include <math.h>
//Initialize all variables and define PI for rotations
float PX = 7;     //PX stands for Player X Position
float PY = 7;     //PY stands for Player Y Position
float PZ = 7;     //PZ stands for Player Z Position

float XR = 0;   //XR stands for camera X Rotation
float YR = 0;     //YR stands for camera Y Rotation
float ZR = 0;     //ZR stands for camera Z Rotation

float OX;         //Mouse Last X position
float OY;         //Mouse Last Y position

double PI = 3.1415926535897932;   //Define PI in a float variable.
float RY = 0;     //The Y rotation value in Degrees CONVERTED to radians.

int CW = 25;      //Chunk Max Width
int CD = 25;      //Chunk Max Depth
int CH = 25;      //Chunk Max Height

int XO = 0;       //Block X Offset
int YO = 0;       //Block Y Offset
int ZO = 0;       //Block Z Offset

int CXO = 0;      //Chunk X Block Offset
int CYO = 0;      //Chunk Y Block Offset
int CZO = 0;      //Chunk Z Block Offset

//Region
float RXO = 0;
float RYO = 0;
float RZO = 0;

//Cube surface bools
bool Top = 0;     //When toggled this loads the top of the current cube loading.
bool Left = 0;    //When toggled this loads the left of the current cube loading.
bool Front = 0;   //When toggled this loads the front of the current cube loading.
bool Right = 0;   //When toggled this loads the right of the current cube loading.
bool Back = 0;    //When toggled this loads the back of the current cube loading.
bool Bottom = 0;  //When toggled this loads the bottom of the current cube loading.
int Block = 0;
//Map Array
int Blocks[40][40][40] = {0};
bool DB = 0;      //Stands for Detect Block it is for placing blocks in front of the player.
bool DBIZ = 0;    //Stands for 

float RayLength = 0;   //The length of the ray in the raycast.

float RayXr = 0;  //The camera ray's X rotation, it is the saame but in reverse so the mouse input to it is inverted.
float RayYr = 0;  //The camera ray's Y rotation(its also the same as the camera and inverted).

//Place and Break Block Raycast variables
//Convert The Cameras Degrees Into Radians In This Variable
float XRa = 0;    //Raycast X Rotation in Radians.
float YRa = 0;    //Raycast Y Rotation in Radians.
float ZRa = 0;    //Raycast Z Rotation in Radians.

float TDX = 0;    //The origin of increments of the raycast.
float TDY = 0;    //This is the unrotated point before the
float TDZ = 0;    //rotations are applied to it.

//ROX,ROY,ROZ are the points position after the first rotation.
float ROX = 0;    //Rotation One X' (Delta)
float ROY = 0;    //Rotation One Y' (Delta)
float ROZ = 0;    //Rotation One Z' (Delta)

//RTX,RTY,RTZ are the coordinates after the second rotation. The
//third Rotation (Z) Axis is not currently used.
float RTX = 0;    //Rotation Two X' (Delta)
float RTY = 0;    //Rotation Two Y' (Delta)
float RTZ = 0;    //Rotation Two Z' (Delta)

//Then check this loactions integer value to check if it is inside a axis aligned bounding box.
int CPX = 0;     //Check Voxel X value.
int CPY = 0;     //Check Voxel Y value.
int CPZ = 0;     //Check Voxel Z value.

//This is the position to break blocks when left clicking
int BBX = 0;     //Selected Block (in map) X value.
int BBY = 0;     //Selected Block (in map) Y value.
int BBZ = 0;     //Selected Block (in map) Z value.

//This is the position to place a block when the player right clicks.
int PBX = 0;     //Position before collision X value.
int PBY = 0;     //Position before collision Y value.
int PBZ = 0;     //Position before collision Z value.

bool MFB = 0;   //This bool is toggled once to make the floor of the map.

bool PlaceBlock = 0;   //This bool is toggled when the player right clicks to place a block into the world.
bool BreakBlock = 0;   //This bool is toggled when the player left clicks to break a block in the world.

int refreshMills = .1;

//Define colors for the GLubyte array to make texture(s).
#define White {0x00, 0x00, 0x00}
#define Black {0xff, 0xff, 0xff}
#define Red {0xff, 0x00, 0x00}
#define Green {0x00, 0xff, 0x00}
#define Blue {0x00, 0x00, 0xff}

//Create a texture
GLubyte Grass[][3] = 
{
    Black, White,
    Blue, Red,
};

//Make a texture by using the colors in a GLubyte array named 'texture'.
GLubyte texture[][3] = 
{
    Black, White, Black, White, Black, White, Black, White, Black, White, Black, White,
    White, Black, White, Black, White, Black, White, Black, White, Black, White, Black,
    Black, White, Black, White, Black, White, Black, White, Black, White, Black, White,
    White, Black, White, Black, White, Black, White, Black, White, Black, White, Black,
    Black, White, Black, White, Black, White, Black, White, Black, White, Black, White,
    White, Black, White, Black, White, Black, White, Black, White, Black, White, Black,
    Black, White, Black, White, Black, White, Black, White, Black, White, Black, White,
    White, Black, White, Black, White, Black, White, Black, White, Black, White, Black,
    Black, White, Black, White, Black, White, Black, White, Black, White, Black, White,
    White, Black, White, Black, White, Black, White, Black, White, Black, White, Black,
    Black, White, Black, White, Black, White, Black, White, Black, White, Black, White,
    White, Black, White, Black, White, Black, White, Black, White, Black, White, Black,
};

//Makes a floor for the level for now...
void MakeFloor()
{
   if(MFB == 0)
   {
      for(int FX = 0; FX < CW; FX++)
      {
         for(int FZ = 0; FZ < CD; FZ++)
         {
            //Set Block to 1
            Blocks[FX][2][FZ] = {1};
         }
      }
   MFB = 1;
   Blocks[4][4][3] = 1;
   }
   
}

//This is the raycast function that will calculate where the center
//of the screen is inside the game to break and place blocks there.
void DBPB()
{
   
   //if(XR < 0){XR = 360;}
   //if(XR > 360){XR = 0;}
   //if(YR < 0){YR = 360;}
   //if(YR > 360){YR = 0;}
   //if(ZR < 0){ ZR = 360;}
   //if(ZR > 360){ZR = 0;}
   //Convert current rotation values into Radian values.
   XRa = (RayXr / 180 * PI);
   YRa = (RayYr / 180 * PI);
   //ZRa = (ZR / 180 * PI);   not currently used.
   RayLength = 0;
   TDX = 0;
   TDY = 0;
   TDZ = RayLength;
   ROX = 0;
   ROY = 0;
   ROZ = 0;
   RTX = 0;
   RTY = 0;
   RTZ = 0;
   
   DB = 0;
   DBIZ = 0;
   //Raycast length is stored in the variable named RayLength which gets longer every time the loop runs.
   for(RayLength = 0; RayLength > -9; RayLength = RayLength - .000833333)
   {
      //Rotate X axis
      TDZ = RayLength;
      ROX = TDX;
      ROY = (TDY*cos(XRa)) - (TDZ*sin(XRa));
      ROZ = (TDY*sin(XRa)) + (TDZ*cos(XRa));

      //Rotate Y axis
      RTX = (ROZ*sin(YRa)) + (ROX*cos(YRa));
      RTY = ROY;
      RTZ = (ROZ*cos(YRa)) - (ROX*sin(YRa));

      //Rotate in Z axis(Not used)
      //X' = X*cos(theta) - y*sin(theta)
      //Y' = X*Sin(theta) + y*cos(theta)
      //Z' = Z
      
      //CPX,CPY,CPZ is the X, Y, Z values to check
      //the position possibly being looked at for a
      //solid block in the map in game while playing.
      CPX = (int)(RTX+PX);
      CPY = (int)(RTY+PY);
      CPZ = (int)(RTZ+PZ);
      
      //When not inside a block in creative, this
      //variable will hold the position of the
      //block before the selected block to place
	  //blocks before it's location in the array.
	  //This usually places blocks to the side or
	  //top or bottom correctly.
      if(Blocks[CPX][CPY][CPZ] == 0 && DBIZ == 0)
      {
         //PBX,PBY,PBZ variables hold the position the player places blocks at when the raycsat intersecsts a block.
		 PBX = CPX;
         PBY = CPY;
         PBZ = CPZ;
      }
      if(Blocks[CPX][CPY][CPZ] != 0 && DB == 0)
      {
         //Toggle bool DB
         //std::cout << "Rotateded X :" << CPX << " Y:" << CPY << " Z :" << CPZ << std::endl;
         //When a collision is detected BBX, BBY, BBZ are equal to where to break a block if they click to break blocks.
         BBX = CPX;
         BBY = CPY;
         BBZ = CPZ;
         RayLength = -9;
         DBIZ = 1;
		 DB = 1;
      }
      glBegin(GL_LINES);
	     glVertex3f(PX,PY+1,PZ);
         glVertex3f(CPX+.5,CPY+.5,CPZ+.5);
      glEnd();
   }
}

//Capture mouse clicks in GLUT using the following function for the mouse.
void MFC(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      //Left mouse button clicked at (x, y, z)
      //Break Blocks if player left clicks on a block.
      if(BBX > 0 && BBX < CW && BBY > 0 && BBY < CH && BBZ > 0 && BBZ << CD && DB == 1)
      {
         DBPB();
		 Blocks[BBX][BBY][BBZ] = 0;
         //BreakBlock = 1;
      }
   }
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
   {
      //Right mouse button clicked at (x, y, z)
      //Place Block if player right clicks on a block.
      if(BBX > 0 && BBX < CW && BBY > 0 && BBY < CH && BBZ > 0 && BBZ << CD && DBIZ == 1)
      {
         DBPB();
		 Blocks[PBX][PBY][PBZ] = 1;
         //PlaceBlock = 1;
      }
   }
}

//LC stands for load chunk. It has 3 loops that go through every x and y and z coordinate to load that blocks textures if it is visible(next to air). 
void LC()
{
   for(XO = 0; XO <= CW; XO++)
   {
      for(YO = 0; YO <= CH; YO++)
      {
         for(ZO = 0; ZO <= CD; ZO++)
         {
            //Set the current block to the Block ID equal to which block is present at the location.
            Block = Blocks[XO][YO][ZO];
            //Select the texture that matches the current block ID.
            if(Block == 1){ glTexImage2D(GL_TEXTURE_2D, 0, 3, 12, 12, 0, GL_RGB, GL_UNSIGNED_BYTE, texture); }//Set texture to default
            //if(Block == 2){

            //First check to make sure it is not the chunk edge because we dont automatically load the edges, they will be put in a cache per chunk.
			//Check if every surface of the block is against air. If a surface is adjacent to air then it is loaded. If the block is at the chunk edge
			//it has to check a cache file for whether or not there is a block adjacent to it or not.
            //If a block is not equal to zero(air), load its textures data onto its surfaces.
            if(Block != 0)
            {
               //Then reset all sides to zero from the last time the loop has executed this
			   //functions lines of code or a previously loaded cube will cause problems with
			   //the function displaying all the sides of every cube and blocks that are made
			   //of air will be drawn as a previously loaded block.
               Top = 0;       //Reset Top bool
               Left = 0;      //Reset Left bool
               Front = 0;     //Reset Front bool
               Right = 0;     //Reset Right bool
               Back = 0;      //Reset Back bool
               Bottom = 0;    //Reset Bottom bool

               //Check Top side(Y+1)
               if(YO + 1 < CH && Blocks[XO][YO+1][ZO] == 0) { Top = 1; }
               //Next Check the cache for the edges of the chunks data.
               //Check the Left side (X-1)
               if(XO - 1 > 0 && Blocks[XO-1][YO][ZO] == 0) { Left = 1; }
               //Next Check the cache for the edges of the chunks data.
               //Check the Front side (Z-1)
               if(ZO-1 > 0 && Blocks[XO][YO][ZO-1] == 0) { Front = 1; }
               //Next Check the cache for the edges of the chunks data.
               //Check the Right side (X+1)
               if(XO + 1 < CW && Blocks[XO+1][YO][ZO] == 0) { Right = 1; }
               //Next Check the cache for the edges of the chunks data.
               //Check the Back side (Z+1)
               if(ZO + 1 < CD && Blocks[XO][YO][ZO+1] == 0) { Back = 1; }
               //Next Check the cache for the edges of the chunks data.
               //Check the bottom(Y-1)
               if(YO - 1 > 0 && Blocks[XO][YO-1][ZO] == 0) { Bottom = 1; }
               //Next Check the cache for the edges of the chunks data.
               

               //Load Blocks surfaces (textures)
               if(Top == 1)
               {
                  //Top Surface(Back left triangle then Front right triangle)
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO,YO+1.0,ZO);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO+1.0);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO+1.0);
                  glEnd();
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(-1.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO);
                     glTexCoord2f(0.0f,-1.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO+1.0);
                  glEnd();
                  }
               if(Left == 1)
               {
                  //Left Surface(Top left triangle then Bottom right triangle)
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO+1.0);
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO,YO,ZO+1.0);
                  glEnd();
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO);
                     glTexCoord2f(1.0f,1.0f);
                     glVertex3f(XO,YO,ZO);
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO,YO,ZO+1.0);
                  glEnd();
               }
               if(Front == 1)
               {
                  //Front Surface(Top left triangle then Bottom right triangle)
                  glBegin(GL_TRIANGLES);//done
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO,YO,ZO);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO);
                  glEnd();
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(-1.0f,0.0f);
                     glVertex3f(XO,YO,ZO);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO+1.0,YO,ZO);
                     glTexCoord2f(0.0f,-1.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO);
                  glEnd();
               }
               if(Right == 1)
               {
                  //Right Surface(Top left triangle then Bottom right triangle)
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(-1.0f,0.0f);
                     glVertex3f(XO+1.0,YO,ZO);
                     glTexCoord2f(-1.0f,-1.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO);
                     glTexCoord2f(0.0f,-1.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO+1.0);
                  glEnd();
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(-1.0f,0.0f);
                     glVertex3f(XO+1.0,YO,ZO);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO+1.0,YO,ZO+1.0);
                     glTexCoord2f(0.0f,-1.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO+1.0);
                  glEnd();
               }
               if(Back == 1)
               {
                  //Back Surface(Top left triangle then Bottom right triangle)
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO+1.0);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO+1.0,YO+1.0,ZO+1.0);
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO+1.0,YO,ZO+1.0);
                  glEnd();
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO,YO+1.0,ZO+1.0);
                     glTexCoord2f(1.0f,1.0f);
                     glVertex3f(XO,YO,ZO+1.0);
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO+1.0,YO,ZO+1.0);
                  glEnd();
               }
               if(Bottom == 1)
               {
                  //Bottom Surface(Back left triangle then Front right triangle)
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO,YO,ZO);
                     glTexCoord2f(0.0f,0.0f);
                     glVertex3f(XO,YO,ZO+1.0);
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO+1.0,YO,ZO+1.0);
                  glEnd();
                  glBegin(GL_TRIANGLES);
                     glTexCoord2f(1.0f,0.0f);
                     glVertex3f(XO,YO,ZO);
                     glTexCoord2f(1.0f,1.0f);
                     glVertex3f(XO+1.0,YO,ZO);
                     glTexCoord2f(0.0f,1.0f);
                     glVertex3f(XO+1.0,YO,ZO+1.0);
                  glEnd();
               }
            }
         }
      }
   }
}

//Camera function
void Camera()
{
   glRotatef(XR, 1.0, 0.0, 0.0);
   glRotatef(YR, 0.0, 1.0, 0.0);
   glRotatef(ZR, 0.0, 0.0, 1.0);
   glTranslated(-PX, -PY, -PZ);
}

//Display function
void display()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   Camera();   //Load the camera
   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_LIGHTING);
   //glEnable(GL_LIGHT1);
   //glShadeModel(GL_SMOOTH);
   glEnable(GL_TEXTURE_2D);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   LC();       //Loads current chunk
   DBPB();     //Raycast function to break and place blocks.
   glutSwapBuffers();
}

void timer(int value)
{
   glutPostRedisplay();
   glutTimerFunc(refreshMills, timer, 0);
}

//Reshape function
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
   glMatrixMode(GL_MODELVIEW);
}


//Keyboard controls
void keyboard(unsigned char key, int x, int y)
{
   if (key == 'w')
   {
      //Move forward
      RY = (YR / 180 * PI);
      PX += float(sin(RY));
      PZ -= float(cos(RY));
   }
   if (key == 'a')
   {
      //Strafe left
      RY = (YR / 180 * PI);
      PX -= float(cos(RY)) * 0.2;
      PZ -= float(sin(RY)) * 0.2;
   }
   if (key == 's')
   {
      //Move backward
      RY = (YR / 180 * PI);
      PX -= float(sin(RY));
      PZ += float(cos(RY));
   }
   if (key == 'd')
   {
      //Strafe right
      RY = (YR / 180 * PI);
      PX += float(cos(RY)) * 0.2;
      PZ += float(sin(RY)) * 0.2;
   }
   if (key == 'q')
   {
      //Move Up
      PY++;
   }
   if (key == 'z')
   {
      //Move Down
      PY--;
   }
   if (key == 27)
   {
      //Exit program
      exit(0);
   }
}

//MM stands for Mouse motion function
void MM(int x, int y)
{
   int fx = x - OX;
   int fy = y - OY;
   OX = x;
   OY = y;
   XR += fy;
   YR += fx;
   //RayXr and RayYr is the rotation for the raycast, it has to be inverted like OpenGls rotations normally. 
   RayXr -= fy;
   RayYr -= fx;
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(600, 400);
   glutCreateWindow("Voxel Test");
   MakeFloor();   //Creates the floor for now
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);
   glutPassiveMotionFunc(MM);
   glutMotionFunc(MM);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(MFC);
   //glutTimerFunc(0, timer, 0);
   glutMainLoop();
   return 0;
}
