////////////////////////////////////////////////////////////////////////////////
//
// Game GUI - A Simple GUI with skin-support for OpenGL Games
//
// (C) by Sven Forstmann in 2015
//
// License : MIT
// http://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////////////////////////
// Mathlib included from 
// http://sourceforge.net/projects/nebuladevice/
////////////////////////////////////////////////////////////////////////////////
#include "gui.h"
////////////////////////////////////////////////////////////////////////////////
void init_gui()
{
	gui.init( Gui::Flags::CONTEXT_MENU | Gui::Flags::CUSTOM_MOUSE ,
				"../data/gui_global.txt" , 
				"../data/gui_skin.txt");

	// Main Window

	Gui::Window w=Gui::Window("Hello World",100,100,350,250);	

	// Add Simple Label to Window

	w.label["l"]=Gui::Label("Label",20,70,100,20);

	// Add Simple Button to Window

	w.button["Ok1"]=Gui::Button("OK",20,100,60,20);
	w.button["Ok1"].callback_pressed=
		[](Gui::Window *window,Gui::Button* control,int index)
		{
			window->close();
		};

	w.button["Ok2"]=Gui::Button("",220,100,100,100);
	w.button["Ok2"].skin=Skin(	"../data/smiley.png",
								"../data/smileybw.png",
								"../data/smiley2.png");

	// Add Simple Combo to Window

	w.combo ["cb"]=Gui::Combo(120,100,60,20);
	w.combo ["cb"].add_item("test");
	w.combo ["cb"].add_item(L"東京");
	w.combo ["cb"].callback_selected=
		[](Gui::Window *w,Gui::Button* control,int index) // text entered callback example
		{
			Gui::Combo &c=*(Gui::Combo*)control;
			w->label["l"].text=Gui::String( c.selected ) + "  selected";
			w->label["l"].textcolor=vec4f(1,0,0,1);
		};

	// Add Text Edit to Window

	w.textedit["txt"]=Gui::TextEdit(10,"text",20,150,160,20);
	w.textedit["txt"].callback_text_entered=		
		[](Gui::Window *w,Gui::Button* control,int index) // text entered callback example
		{
			Gui::TextEdit &t=*(Gui::TextEdit*)control;
			w->title.text=t.text;
		};

	// Add Radio

	w.radio["rad"]=Gui::Radio(20,190,20,20);	// first button
	w.radio["rad"].add_item(50,190);			// second button
	w.radio["rad"].add_item(80,190);			// third button
	w.radio["rad"].callback_pressed=
		[](Gui::Window *w,Gui::Button* control,int index)
		{	
			w->x+=10;
		};

	// Add Slider

	w.slider["s1"]=Gui::Slider(	0,200,100, 		/* min,max,default*/	
								20,220,160,20);	/* window x,y,sx,sy */ 
	w.slider["s1"].callback_pressed=
	[](Gui::Window *w,Gui::Button* control,int index)
	{
		Gui::Slider &b=*((Gui::Slider*) control);	
		w->button["Ok1"].text=Gui::String(int(b.val));	
	};

	w.slider["s2"]=Gui::Slider(	0,200,100, 		/* min,max,default*/	
								350,100,20,160,	/* window x,y,sx,sy */
								Gui::Slider::VERTICAL );

	// Add File Menu to Window

	Gui::Menu m=Gui::Menu("File",/* x,y,sx,sy */ 9,39,50,20, /* menuwidth */ 100);
	m.add_item("Load",
		[](Gui::Window *window,Gui::Button* control,int index) // menu button callback
		{
			gui.screen[0].window["filebrowser"]=  // open file dialog
				gui_file_dialog( "Load SaveGame" , "Load" , "Cancel" ,
					/*get_current_dir()*/ "..\\data\\win8",".png",	100,100,

					// file dialog ok button callback
					[](Gui::Window *w,Gui::Button* b,int index) 
					{		
						MessageBoxA(0, 
							w->textedit["Filename"].text.c_str() , 
							w->label["dir"].text.c_str() ,0);		
						w->close(); 			
					}
			);
		});

	m.add_item("Close",
		[](Gui::Window *w,Gui::Button* control,int index) // menu button callback
		{
			w->parent->close(); // close window
		});

	m.add_menu("submenu");
	m.window.menu[0].add_item("test1");
	m.window.menu[0].add_item("test2");

	w.menu["menu"]=m;

	// Add new Window to Screen 0 (default)

	gui.screen[0].window["hello1"]=w; // finally put window on the screen (1st copy)

	// Modify and Add new Window to Screen 0 (default)

	w.move(500,100);
	w.resize(400,300);
	w.minsize(150,150);

	gui.screen[0].window["hello2"]=w; // finally put window on the screen (2nd copy)

	// -------------------------------------------------------------------------

	// Add Button to Background

	gui.dialog["sample"]=w; // store for later use in the callback

	gui.screen[0].button["more"]=Gui::Button("More Windows Plz!!",50,50,200,20);
	gui.screen[0].button["more"].callback_pressed=
		[](Gui::Window *w,Gui::Button* control,int index) // menu button callback
		{
			Gui::Window w1,w2;
			w1=gui.dialog["sample"];
			w2=gui.dialog["3D"];

			w1.x= timeGetTime() % (int)gui.screen_resolution_x;
			w1.y= timeGetTime() % (int)gui.screen_resolution_y;
			
			w2.x=(timeGetTime()+500) % (int)gui.screen_resolution_x;
			w2.y=(timeGetTime()+300) % (int)gui.screen_resolution_y;

			gui.screen[0].window.add(w1);
			gui.screen[0].window.add(w2);
		};

	// Add Tabbed Window to Background

	Gui::Tab t=Gui::Tab("Win1",350,150,300,200,50,20);;
	t.add_tab("Win2");
	t.add_tab("Win3");
	t.add_tab("Win4");
	loopi(0,4)
	{
		t.window[i].button.add(Gui::Button("OK",20+i*10,20,60));
		t.window[i].label["lab"] =Gui::Label("some text",20+i*20,90,100);
		t.window[i].button["test2"]=Gui::Button("OK",100+i*10,20,60);
		t.window[i].button["test3"]=Gui::Button("OK",50+i*10,50,60);
	}
	t.flags=Gui::Tab::MOVABLE; // Make it movable
	gui.screen[0].tab["mytab"]=t;

	// -------------------------------------------------------------------------
	// Simple 3D Viewer

	w=Gui::Window("3D View",150,150,400,400);	
	w.menu["Menu"]=m;

	// Add simple Renderer
	{
		// render callback function
		auto render_func=[](Gui::Window *window,Gui::Button* control,int index)
		{	
			if((!control) || (!window) || window->get_toggled() ) return;

			Gui::Window &w=*((Gui::Window*) window);
			Gui::Button &b=w.button["canvas"];

			// resize button to window client area
			b.x=w.pad_left;
			b.y=w.pad_up;
			b.sx=w.sx-w.pad_left-w.pad_right;
			b.sy=w.sy-w.pad_up-w.pad_down;

			bool draw= (b.hover||b.pressed) && (gui.mouse.button[0] || gui.mouse.button[1] || gui.mouse.wheel!=0);
			if(control!=&b)draw=1;
		
			FBO *fb=(FBO*)b.var.ptr["fbo"]; if(!fb) return;
			FBO &fbo=*(FBO*)fb;

			if(b.sx!=fbo.width || b.sy!=fbo.height )
			{
				fbo.clear();
				fbo.init(b.sx,b.sy);
				b.skin.tex_normal=b.skin.tex_hover=b.skin.tex_selected=fbo.color_tex;
				draw=1;
			}
			if (!draw) return;

			quaternion q(b.var.vec4["rotation"]);
			if(gui.mouse.button[0] & b.pressed)
			{
				quaternion qx,qy;
				qx.set_rotate_y( (float)gui.mouse.dx/100);
				qy.set_rotate_x(-(float)gui.mouse.dy/100);
				q=qy*qx*q;
				b.var.vec4["rotation"]=vec4f(q.x,q.y,q.z,q.w);
			}
			double z=b.var.number["zoom"];
			if(b.hover)
			{
				z=clamp( z-gui.mouse.wheel*2 , 2,120 );
				b.var.number["zoom"]=z;
				gui.mouse.wheel=0;
			}
			vec4f pos=b.var.vec4["position"];
			if(gui.mouse.button[1] && (b.pressed||b.hover))
			{
				pos=pos+vec4f((float)gui.mouse.dx*z/40000.0f,(float)gui.mouse.dy*z/40000.0f,0,0);
				b.var.vec4["position"]=pos;
			}

			// render to fbo
			fbo.enable();
			glClearColor(0.7,0.7,0.7,1);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();
			gluPerspective(z, (GLfloat)b.sx/(GLfloat)b.sy, 0.01 , 10.0);
			glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();
			glTranslatef(pos.x,pos.y,-pos.z);
			matrix44 m(q);
			glMultMatrixf(&m.m[0][0]);

			glColor4f(0,0,0,1);
			glutWireTeapot(1);

			glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();
			fbo.disable();
		};
		w.button["canvas"]=Gui::Button("",20,250);
		w.button["canvas"].skin=Skin("");
		w.button["canvas"].var.vec4["position"]=vec4f(0,0,2.5,0); // vec4
		w.button["canvas"].var.vec4["rotation"]=vec4f(1,0,0,0); // quaternion
		w.button["canvas"].var.number["zoom"]=50; // fov
		w.button["canvas"].var.ptr["fbo"]=0;
		w.button["canvas"].callback_all=render_func;
		w.button["canvas"].callback_init=[](Gui::Window *w,Gui::Button* b,int i) // call before drawing the first time
			{
				if(w && b){b->var.ptr["fbo"]=new FBO(100,100);}
			};
		w.button["canvas"].callback_exit=[](Gui::Window *w,Gui::Button* b,int i) // called from the button's destructor
			{
				if(w && b)if(b->var.ptr["fbo"]){delete(((FBO*)b->var.ptr["fbo"]));}
			};
	}
	gui.dialog["3D"]=w;
	gui.screen[0].window["3D View win1"]=w;
	// -------------------------------------------------------------------------

	// also use our previous menu as context menu and file menu
	// Note : "context_menu" is reserved for the context menu
	//		  all other id names are common menus on the background

	gui.screen[0].menu["context_menu"]=m;

	gui.screen[0].menu["file"]=m;
	gui.screen[0].menu["file"].y=5;

}
////////////////////////////////////////////////////////////////////////////////
void draw_gui()
{
	gui.draw();
}
////////////////////////////////////////////////////////////////////////////////
void exit_gui()
{
	gui.exit();
}
////////////////////////////////////////////////////////////////////////////////
// Glut Callbacks
void KeyDown1Static(int key, int x, int y)           { gui.keyb.key[ key&255 ] =true;  }
void KeyDown2Static(unsigned char key, int x, int y) { gui.keyb.key[ key&255 ] =true;  }
void KeyUp1Static(int key, int x, int y)             { gui.keyb.key[ key&255 ] =false; }
void KeyUp2Static(unsigned char key, int x, int y)   { gui.keyb.key[ key&255 ] =false; }
void MouseMotionStatic (int x,int y){	gui.mouse.x = x;gui.mouse.y = y;}
void MouseButtonStatic(int button_index, int state, int x, int y)
{
	if(button_index<=2) gui.mouse.button[button_index] =  ( state == GLUT_DOWN ) ? true : false;
	if(button_index>=3) gui.mouse.wheel_update+= button_index == 3 ? -1 : 1;
	MouseMotionStatic (x,y);	
}
void OnWindowClose( void ) { exit_gui(); }
////////////////////////////////////////////////////////////////////////////////

void disp(void);
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);

int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 768);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("GAME GUI - BY SVEN FORSTMANN in 2015");

	glutIdleFunc(disp);
	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);

	//glutSpecialFunc(&KeyDown1Static);
	//glutSpecialUpFunc(&KeyUp1Static);
	glutKeyboardFunc(KeyDown2Static);
	glutKeyboardUpFunc(KeyUp2Static);
	glutMotionFunc(MouseMotionStatic);
	glutPassiveMotionFunc(MouseMotionStatic);
	glutMouseFunc (MouseButtonStatic);
	glutCloseFunc (OnWindowClose);

	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	init_gui();

	glutMainLoop();

	

	return 0;
}


void disp(void)
{
	glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	draw_gui();
	
	glutSwapBuffers();
}

void reshape(int x, int y)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x, y, 0, -1, 1);
	gui.screen_resolution_x=x;
	gui.screen_resolution_y=y;
}






