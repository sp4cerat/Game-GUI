### Game GUI - A Simple GUI with skin-support for OpenGL Games

#### License : MIT
#### http://opensource.org/licenses/MIT

**Summary** 

After trying several gui's more or less suited for games (LibRocket, Qt, NVWidgets, CEgui, etc), I found that none of them was really light weight, easy to use and fully skinable at the same time. This was the reason I started to write my own GUI / C++. The project was completed in about one month.

![Screenshot1](http://i.imgur.com/R8DwPzI.png)

**Features**

* Advanced skin scaling: define how the inner and borders of a texture shall be scaled ( simple scale, repeating inner pattern or constant outer frame )
* UTF8 Support
* TTF Support
* Easy skinning support 
* Alpha support (RGBA), so you can have transparent windows/widgets etc
* direct access to all controls and their members
* Non-blocking visualization
* Callback support
* Different from most GUIs, Widgets are stored directly inside the GUI rather having local variables pointing to instances of an overwritten class.
* Directory: Controls are stored and accessed by a tree structure that is rendered every time like a scene graph
* No event loop, show function or update function
* No need to have a timer that calls an update function
* Lightweight: Only Depends on GL and libFreetype and DevIL
* Can easily be used in SDL, Qt and other environments. Just plug in the keyboard and mouse events and call the gui's draw function from your main render loop.
* Config file support for default variables (font,padding etc) and skin textures+scaling parameters
* Only 2000 lines of code for the entire Gui class including all widgets. Therefore easy to modify and extend. 
* Comes with a simple file-browser (See screenshot1)
* MIT license
* Multi-screen support (same as multiple Desktops). You can have a title screen, loading screen, ingame screen etc. Simply create them at the beginning and the switch between by setting gui.active_screen=number;
* Window manager 
* Context menu
* Custom mouse pointer
* Widget Classes : Window, Menu,  Label, Button, CheckBox, Tab, Combo, Textedit, Radio
* Controls have support for custom user variables like control.var.string["myvar"]="hello"

Most important is the access by the directory structure and using simple assigns '=' to create a new instance of a widget or window.


The usage is basically as follows:


Create (and instantly show) a window:

    gui.window["id1"]=Gui::Window("Title",100,100,250,350);

Adding a button :

    gui.window["id1"].button["load"] = Gui::Button("Load",20,20,50,30); 

Adding a simple callback

    gui.window["id1"].button["load"].callback_pressed=
        [](Gui::Window *window,Gui::Button* control,int index)
        {
            window->close();
        };

Setting a custom skin:

    gui.window["id1"].button["load"].skin=Skin("normal.png","hover.png","pressed.png");

Also you have direct access to a skin's texture to use it as output of a FBO e.g.
The 3D viewer of the screenshot below uses that.

    gui.window["id1"].button["load"].skin.texture_normal=fbo.texture_id;

Duplicating a window is easy: The duplicate is instantaneously visible 
(works also for controls)
 
    gui.window["id2"] = gui.window["id1"]


The GUI is not fully optimized, yet you have like 700 fps with 20 simple windows on the screen on a notebook PC. Controls are stored in a std vector as share ptr. For rendering, it is not necessary to evaluate the string keys.


Mathlib included from 
http://sourceforge.net/projects/nebuladevice/

(C) by Sven Forstmann in 2015

![Screenshot1](http://i.imgur.com/piMots7.png)  
![Screenshot2](http://i.imgur.com/RXKUZ6G.png)
