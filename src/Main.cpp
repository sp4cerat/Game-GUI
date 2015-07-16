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

	// also use our previous menu as context menu and file menu
	// Note : "context_menu" is reserved for the context menu
	//		  all other id names are common menus on the background

	gui.screen[0].menu["context_menu"]=m;

	gui.screen[0].menu["file"]=m;
	gui.screen[0].menu["file"].y=5;

	// -------------------------------------------------------------------------

	// Add Button to Background

	gui.dialog["sample"]=w; // store for later use in the callback

	gui.screen[0].button["more"]=Gui::Button("More Windows Plz!!",50,50,200,20);
	gui.screen[0].button["more"].callback_pressed=
		[](Gui::Window *w,Gui::Button* control,int index) // menu button callback
		{
			int id=gui.screen[0].window.add(gui.dialog["sample"]);
			gui.screen[0].window[id].x= timeGetTime() % (int)gui.screen_resolution_x;
			gui.screen[0].window[id].y= timeGetTime() % (int)gui.screen_resolution_y;
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
}
////////////////////////////////////////////////////////////////////////////////
void draw_gui()
{
	gui.draw();
}
////////////////////////////////////////////////////////////////////////////////
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
	glutKeyboardFunc(&KeyDown2Static);
	glutKeyboardUpFunc(&KeyUp2Static);
	glutMotionFunc(&MouseMotionStatic);
	glutPassiveMotionFunc(&MouseMotionStatic);
	glutMouseFunc (&MouseButtonStatic);

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






