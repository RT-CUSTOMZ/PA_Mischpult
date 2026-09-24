/*        _         _             _       _                  _          _            _            _   _            _
*         /\ \      /\ \         /\ \     /\_\               / /\       /\ \         /\ \         /\_\/\_\ _      /\ \
*        /  \ \     \_\ \       /  \ \   / / /         _    / /  \      \_\ \       /  \ \       / / / / //\_\   /  \ \
*       / /\ \ \    /\__ \     / /\ \ \  \ \ \__      /\_\ / / /\ \__   /\__ \     / /\ \ \     /\ \/ \ \/ / /__/ /\ \ \
*      / / /\ \_\  / /_ \ \   / / /\ \ \  \ \___\    / / // / /\ \___\ / /_ \ \   / / /\ \ \   /  \____\__/ //___/ /\ \ \
*     / / /_/ / / / / /\ \ \ / / /  \ \_\  \__  /   / / / \ \ \ \/___// / /\ \ \ / / /  \ \_\ / /\/________/ \___\/ / / /
*    / / /__\/ / / / /  \/_// / /    \/_/  / / /   / / /   \ \ \     / / /  \/_// / /   / / // / /\/_// / /        / / /
*   / / /_____/ / / /      / / /          / / /   / / /_    \ \ \   / / /      / / /   / / // / /    / / /        / / /    _
*  / / /\ \ \  / / /      / / /________  / / /___/ / //_/\__/ / /  / / /      / / /___/ / // / /    / / /         \ \ \__/\_\
* / / /  \ \ \/_/ /      / / /_________\/ / /____\/ / \ \/___/ /  /_/ /      / / /____\/ / \/_/    / / /           \ \___\/ /
* \/_/    \_\/\_\/       \/____________/\/_________/   \_____\/   \_\/       \/_________/          \/_/             \/___/_/
*
*
* Main Project: PA Mischpult
* Based on ATxMega256A3U µController
*
* Module: display.h
* Based on library by Electronic Assembly for TFT Touchscreen
* Modified to specific eDIP320-8 commands
*
*/


#ifndef _DISPLAY_H
#define _DISPLAY_H

//Choose your display
#define _eDIPTFT32
//#define _eDIPTFT43

//define the max length of chars in strings you want to output (take care of your stacksize)
#define MAX_STRING 48

//---------------define constants---------------//
//Dimension
#ifdef _eDIPTFT32
	#define XPIXEL  320
	#define YPIXEL 	240
	#define XMAX    (XPIXEL - 1)
	#define YMAX    (YPIXEL - 1)
#endif
#ifdef _eDIPTFT43
	#define XPIXEL  480
	#define YPIXEL 	272
	#define XMAX    (XPIXEL - 1)
	#define YMAX    (YPIXEL - 1)
#endif

//Some general definitions
#define TRUE	1
#define FALSE	0

//control character
#define ESC  27
#define CR   13
#define LF   10
#define FF   12
//Orientation
#define W0    0
#define W90   1
#define W180  2
#define W270  3

//color constants
#define TRANSPARENT  0
#define BLACK        1
#define BLUE         2
#define RED          3
#define GREEN        4
#define MAGENTA      5
#define CYAN         6
#define YELLOW       7
#define WHITE        8
#define NOCHANGE     255

//constants for animations
#define NONE          0
#define ONCE          1
#define CYCLIC        2
#define PINGPONG      3
#define ONCEBACK      4
#define CYCLICBACK    5
#define PINGPONGBACK  6
#define TOFRAME       7
#define DEFAULTTIME   255

//fonts (standardfonts)
#define FONT4x6    1
#define FONT6x8    2
#define FONT7x12   3
#define GENEVA10   4
#define CHICAGO14  5
#define SWISS30B   6
#define BIGZIF50   7
#define BIGZIF100  8


//------------------Functions-------------------//
	//Terminal functions
#define eDIP_TA() eDIP_command("TA")	//#TA=Terminal off: #TA  (switch Terminal off outputs are ignored)
#define eDIP_TE() eDIP_command("TE")	//#TE=Terminal on: #TE  (switch Terminal on)

	//graphic commands
		//display commands (effect entire display)
#define eDIP_FD(fg, bg)	eDIP_command2("FD", fg, bg)	//#FD=Set display color: #FD fg,bg  (foreground color fg=1..32; background color bg=1..32)
#define eDIP_DO(n1)		eDIP_command1("DO",n1);		//#DO=Set display orientation: #DO n1  (Display orientation n1: 0=0°; 1=90°; 2=180°; 3=270°)
#define eDIP_DL() 		eDIP_command("DL")			//#DL=Delete display: #DL  (Fill display contents with display background color)
#define eDIP_DS() 		eDIP_command("DS")			//#DS=Fill display: #DS  (Fill display contents with display foreground color)
#define eDIP_DF(c)		eDIP_command1("DF", c)		//#DF=Fill display with color: #DF fg  (Fill display contents with color fg=1..32)
#define eDIP_DI() 		eDIP_command("DI")			//#DI=Invert display: #DI  (Invert display contents / invert all pixels)
		//oututting strings
#define eDIP_FZ(fg, bg)	eDIP_command2("FZ", fg, bg)	//#FZ=Set string color: #FZ fg,bg  (fg=foreground color; bg=background color; fg/bg=palette 1..32 or 0=transparent)
#define eDIP_ZF(n1)		eDIP_command1("ZF",n1)		//#ZF=Set font: #ZF n1  (Set font with the number n1=0..255)
#define eDIP_ZZ(x, y)	eDIP_command2("ZZ", x, y)	//#ZZ=Font zoom factor: #ZZ n1,n2  (n1=X-Zoom factor 1x..8x;  n2=Y-Zoom factor 1x..8x)
#define eDIP_ZY(lr, tb)	eDIP_command2("ZY", lr, tb) //#ZY=Add char width/height: #ZY n1,n2  (adds n1=0..15 dots at left/right and n2=0..15 dots at top/bottom of each character)
#define eDIP_ZJ(x)		eDIP_command1("ZJ",x);		//#ZJ=Space width: #ZJ n1  (n1=0: use original space from font; n1=1: same width as numbers; n1>=2: width in dots)
#define eDIP_ZW(x)		eDIP_command1("ZW",x);		//#ZW=Text angle: #ZW n1  (Text output angle n1: 0=0°; 1=90°; 2=180°; 3=270°)
#define eDIP_ZL(x,y,string,len)	eDIP_string("ZL", x, y, string, len) //#ZL=Output string left justified: #ZL xx1,yy1,"Text"  (A string is output to xx1,yy1 several lines are separated by '|')
#define eDIP_ZC(x,y,string,len)	eDIP_string("ZC", x, y, string, len) //#ZC=Output string centered: #ZC xx1,yy1,"Text"  (A string is output to xx1,yy1 several lines are separated by '|')
#define eDIP_ZR(x,y,string,len)	eDIP_string("ZR", x, y, string, len) //#ZR=Output string right justified: #ZR xx1,yy1,"Text"  (A string is output to xx1,yy1 several lines are separated by '|')
#define eDIP_ZT(string, len)	eDIP_string2("ZT",string,len)		 //#ZT=String for terminal: #ZT "Text"  (Command for outputting a string in a macro to the terminal)
		//draw straight lines and points
#define eDIP_FG(fg,bg)	eDIP_command2("FG", fg, bg)	//#FG=Set line color: #FG fg,bg  (foreground/line color fg=1..32; pattern background color bg=1..32 or 0=transparent)
#define eDIP_GR(x1,y1,x2,y2)	eDIP_draw("GR", x1, y1, x2, y2, 0, 0, 4*2) //#GR=Draw rectangle: #GR xx1,yy1,xx2,yy2  (Draw four straight lines as a rectangle from xx1,yy1 to xx2,yy2)
#define eDIP_GD(x1,y1,x2,y2)	eDIP_draw("GD", x1, y1, x2, y2, 0, 0, 4*2) //#GD=Draw straight line: #GD xx1,yy1,xx2,yy2  (Draw straight line from xx1,yy1 to xx2,yy2)
#define eDIP_GW(x1,y1)			eDIP_draw("GW", x1, y1, 0, 0, 0, 0, 2*2)   //#GW=Continue straight line: #GW xx1,yy1  (Draw a straight line from last end point to xx1,yy1)
#define eDIP_GP(x1,y1)			eDIP_draw("GP", x1, y1, 0, 0, 0, 0, 2*2)   //#GP=Draw point: #GP xx1,yy1  (Set one dot at coordinates xx1,yy1)
#define eDIP_GZ(n1, n2)			eDIP_command2("GZ", n1, n2)	//#GZ=Point size/line thickness: #GZ n1,n2  (n1=X-dotsize 1..15;  n2=Y-dotsize 1..15)
#define eDIP_GM(n1)				eDIP_command1("GM",n1);		//#GM=Line pattern: #GM n1  (n1=0: no pattern, draw a solid line; n1=1..255: use a pattern for points/lines)
		//change draw rectangular areas
#define eDIP_RL(x1,y1,x2,y2)		eDIP_draw("RL", x1, y1, x2, y2, 0, 0, 4*2)	 //#RL=Delete area: #RL xx1,yy1,xx2,yy2  (Fill an area from xx1,yy1 to xx2,yy2 with display background color)
#define eDIP_RS(x1,y1,x2,y2)		eDIP_draw("RS", x1, y1, x2, y2, 0, 0, 4*2)	 //#RS=Fill area: #RS xx1,yy1,xx2,yy2  (Fill an area from xx1,yy1 to xx2,yy2 with display foreground color)
#define eDIP_RF(x1,y1,x2,y2,n1)		eDIP_draw("RF", x1, y1, x2, y2, n1, 0, 4*2+1)//#RF=Fill area with color: #RF xx1,yy1,xx2,yy2,fg  (Fill an area from xx1,yy1 to xx2,yy2 with color fg=1..32)
#define eDIP_RI(x1,y1,x2,y2)		eDIP_draw("RI", x1, y1, x2, y2, 0, 0, 4*2)	 //#RI=Invert area: #RI xx1,yy1,xx2,yy2  (Invert an area from xx1,yy1 to xx2,yy2 / invert all pixels)
#define eDIP_RC(x1,y1,x2,y2,x3,y3)	eDIP_draw("RC", x1, y1, x2, y2, x3, y3, 6*2) //#RC=Copy area: #RC xx1,yy1,xx2,yy2,xx3,yy3  (copy the area from xx1,yy1 to xx2,yy2 to new upper left corner xx3,yy3)
#define eDIP_FM(fg,bg)			eDIP_command2("FM", fg, bg)					 //#FM=Set pattern color: #FM fg,bg  (fg=foreground color; bg=background color; fg/bg=palette 1..32 or 0=transparent)
#define eDIP_RM(x1,y1,x2,y2,n1)	eDIP_draw("RM", x1, y1, x2, y2, n1, 0, 4*2+1)//#RM=Area with fill pattern: #RM xx1,yy1,xx2,yy2,n1  (Draw an area from xx1,yy1 to xx2,yy2 with pattern n1)
#define eDIP_RO(x1,y1,x2,y2,n1)	eDIP_draw("RO", x1, y1, x2, y2, n1, 0, 4*2+1)//#RO=Draw box: #RO xx1,yy1,xx2,yy2,n1  (Draw a rectangle xx1,yy1 to xx2,yy2 with fill pattern n1)
#define eDIP_FR(c1,c2,c3)		eDIP_command3("FR", c1, c2, c3)				 //#FR=Set border colors: #FR f1,f2,f3  (f1..f3=1..32 or 0=transparent; f1=frame outside; f2=frame inside; f3=filling)
#define eDIP_RE(n1, n2)			eDIP_command2("RE", n1, n2)					 //#RE=Set frametype: #RE n1,n2  (n1=1..255: set frame type; n2=frameangle 0=0°; 1=90°; 2=180°; 3=270°)
#define eDIP_RR(x1,y1,x2,y2,nr)	eDIP_draw("RR", x1, y1, x2, y2, nr, 0, 4*2+1)	 //#RR=Draw frame: #RR xx1,yy1,xx2,yy2  (Draw a frame from xx1,yy1 to xx2,yy2)

	//Bitmap/Animation commands
		//Bitmap commands
#define eDIP_FU(fg,bg)	eDIP_command2("FU", fg, bg) //#FU=Set bitmap color: #FU fg,bg  (fg=foreground- bg=background-color for monochrome bitmaps; fg/bg=palette 1..32 or 0=transparent)
#define eDIP_UZ(x,y)	eDIP_command2("UZ", x, y)	//#UZ=Image zoom factor: #UZ n1,n2  (n1=X-Zoom factor 1x..8x;  n2=Y-Zoom factor 1x..8x)
#define eDIP_UW(n1)		eDIP_command1("UW", n1)		//#UW=Image angle: #UW n1  (Image output angle n1: 0=0°; 1=90°; 2=180°; 3=270°)
#define eDIP_UX(n1)		eDIP_command1("UX", n1)		//#UX=Image mirror: #UX n1  (mirror Image: n1=0 normal output; n1=1 mirror image horizontal)
#define eDIP_UT(n1)		eDIP_command1("UT", n1)		//#UT=Transparency bitmap color: #UT n1 (n1=0 no transparency; n1=1 top left dot; n1=2 use defined color; n1=3 replace by background)
#define eDIP_UI(x1,y1,nr)	eDIP_draw("UI", x1, y1, nr, 0, 0, 0, 2*2+1) //#UI=Load internal image: #UI xx1,yy1,n1  (Load internal image number n1=0..255 from DATA-FLASH to xx1,yy1)
		//Animation coomands

	//Bargraph commands
#define eDIP_FB(fg,bg,fc)					eDIP_command3("FB", fg, bg, fc) //#FB=Set bargraph color: #FB fg,bg,fr  (fg=foreground color; bg=background color; fr=frame color; fg/bg/fr=palette 1..32)
#define eDIP_BM(n1)							eDIP_command1("BM", n1) //#BM=Bar pattern: #BM n1  (n1=1..255: use a pattern for bartype 0..3; n1=0: no pattern, use foreground color)
#define eDIP_BE(n1)							eDIP_command1("BE", n1) //#BE=Bar frametype: #BE n1  (n1=1..255: set a frame for bartype 4..7)
#define eDIP_BB(n1)							eDIP_command1("BB", n1) //#BB=Bar dotwidth: #BB n1  (n1=1..255: for dot with pattern bartype 2+3; n1=16..255: for dot with frame bartype 6+7; n1=0:automatic)
#define eDIP_BR(nr,x1,y1,x2,y2,sv,ev,type,mst)	eDIP_bargraph("BR", nr, x1, y1, x2, y2, sv, ev, type, mst)//#BR=Bar Right: #BR n1,xx1,yy1,xx2,yy2,sv,ev,typ  (n1=1..20; xx1..yy2=rectangle; sv,ev=start/end value; typ:0..7)
#define eDIP_BL(nr,x1,y1,x2,y2,sv,ev,type,mst)	eDIP_bargraph("BL", nr, x1, y1, x2, y2, sv, ev, type, mst)//#BL=Bar Left: #BL n1,xx1,yy1,xx2,yy2,sv,ev,typ  (n1=1..20; xx1..yy2=rectangle; sv,ev=start/end value; typ:0..7)
#define eDIP_BO(nr,x1,y1,x2,y2,sv,ev,type,mst)	eDIP_bargraph("BO", nr, x1, y1, x2, y2, sv, ev, type, mst)//#BO=Bar Up: #BO n1,xx1,yy1,xx2,yy2,sv,ev,typ  (n1=1..20; xx1..yy2=rectangle; sv,ev=start/end value; typ:0..7)
#define eDIP_BU(nr,x1,y1,x2,y2,sv,ev,type,mst)	eDIP_bargraph("BU", nr, x1, y1, x2, y2, sv, ev, type, mst)//#BU=Bar Down: #BU n1,xx1,yy1,xx2,yy2,sv,ev,typ  (n1=1..20; xx1..yy2=rectangle; sv,ev=start/end value; typ:0..7)
#define eDIP_BA(n1, val)					eDIP_command2("BA", n1, val)	//#BA=Update Bargraph: #BA n1,n2  (Set and draw the bar graph with the number n1 to the new user value n2)
#define eDIP_BN(no)							eDIP_command1("BN", no)			//#BN=Draw new Bargraph: #BN n1  (Draw the bar graph with the number n1 completely)
#define eDIP_BS(no)							eDIP_command1("BS", no)			//#BS=Send Bargraph value: #BS n1  (Send the current value of bargraph n1 on the serial interface)
#define eDIP_BD(no, n2)						eDIP_command2("BD", no, n2)		//#BD=Delete Bargraph: #BD n1,n2  (Makes definition of bar n1 invalid. n2=0: bar keeps visible; n2=1: bar will be cleared)

	//Macro instructions
		//Macro commands
#define eDIP_MN(no)				eDIP_command1("MN", no) //#MN=Run macro: #MN n1  (Call the macro with the number n1=0..255; max. 7 levels)
#define eDIP_MT(no)				eDIP_command1("MT", no)	//#MT=Run touch macro: #MT n1  (Call the touch-macro with the number n1=0..255; max. 7 levels)
#define eDIP_MP(no)				eDIP_command1("MP", no)	//#MP=Run port macro: #MP n1  (Call the port-macro with the number n1=0..255; max. 7 levels)
#define eDIP_MB(no)				eDIP_command1("MB", no)	//#MB=Run bit macro: #MB n1  (Call the bit-macro with the number n1=0..255; max. 7 levels)
#define eDIP_MX(no)				eDIP_command1("MX", no)	//#MX=Run matrix macro: #MX n1  (Call the matrix-macro with the number n1=0..255; max. 7 levels)
#define eDIP_MC(no)				eDIP_command1("MC", no)	//#MC=Run process macro: #MC n1  (Call the process-macro with the number n1=0..255; max. 7 levels)
#define eDIP_MV(no)				eDIP_command1("MV", no)	//#MV=Run anlog macro: #MV n1  (Call the analog-macro with the number n1=0..255; max. 7 levels)
#define eDIP_ML(type,n1,n2)		eDIP_command3("ML", type, n1, n2) //#ML=Inhibit macro: #ML type,n1,n2  (Inhibit execution for macro type='N,T,M,P,B,X,C' or 'A'=all types from macro number n1 to n2)
#define eDIP_MU(type,n1,n2)		eDIP_command3("MU", type, n1, n2) //#MU=Enable macro: #MU type,n1,n2  (Enables execution for macro type='N,T,M,P,B,X,C' or 'A'=all types from macro number n1 to n2)
#define eDIP_MK(n1)				eDIP_command1("MK", n1)	//#MK=Select page: #MK n1  (Select pagenumber n1=0..15 for macros/pictures)
#define eDIP_MW() 				eDIP_command("MW")	//#MW=Save page: #MW  (Save actually macros/pictures pagenumber)
#define eDIP_MR() 				eDIP_command("MR")	//#MR=Restore page: #MR  (Restore last macros/pictures pagenumber)
		//automatic (normal-) macro
#define eDIP_MG(n1, n2)			eDIP_command2("MG", n1, n2)	   //#MG=Macro with delay: #MG n1,n2  (macro n1=0..255 runs after delay of n2/10s. May be stopped/prevented by any command)
#define eDIP_ME(n1,n2,n3)		eDIP_command3("ME", n1, n2, n3)//#ME=Autom. macro once: #ME n1,n2,n3  (Automatic once only macros n1..n2; n3=pause in 1/10s. Will be stopped by any command)
#define eDIP_MA(n1,n2,n3)		eDIP_command3("MA", n1, n2, n3)//#MA=Autom. macro cyclical: #MA n1,n2,n3  (Automatic cyclically macros n1..n2; n3=pause in 1/10s. Will be stopped by any command)
#define eDIP_MJ(n1,n2,n3)		eDIP_command3("MJ", n1, n2, n3)//#MJ=Autom. macro pingpong: #MJ n1,n2,n3  (Automatic pingpong macros n1..n2..n1; n3=pause in 1/10s. Will be stopped by any command)
		//macro processes
#define eDIP_MD(no,type,n3,n4,zs)	eDIP_command5("MD", no, type, n3, n4, zs)//#MD=Define process macro: #MD n1,typ,n3,n4,n5  (macro process n1=1..4; typ: 1=once; 2=cyclical; 3=pingpong; n3..n4=range; n5=delay)
#define eDIP_MZ(n1, zs)				eDIP_command2("MZ", n1, zs)//#MZ=Process macro speed: #MZ n1,n2  (Assign a new delay for process n1=1..4 with n2/10s. n2=0 will stop the automatic)
#define eDIP_MS(n1)					eDIP_command1("MS", n1)	   //#MS=Stop process/animation: #MS n1  (All process macros/animations will be stopped with n1=0 and continued with n1=1)

	//Touch Panel
		//touch presets
#define eDIP_FE(n1,n2,n3,s1,s2,s3)	eDIP_command6("FE", n1, n2, n3, s1, s2, s3,6)//#FE=Set touchborder colors: #FE n1,n2,n3,s1,s2,s3  (n1..n3=normal s1..s3=for selection (0..32); 1=frame outside;2=inside;3=filling)
#define eDIP_AE(n1, n2)				eDIP_command2("AE", n1, n2)		   //#AE=Touch frame: #AE n1,n2  (n1=1..255: frame type for touch keys/switches; n1=0:no frame; n2=frameangle 0=0°;1=90°;2=180°;3=270°)
#define eDIP_FC(nf,nb,sf,sb)		eDIP_command4("FC", nf, nb, sf, sb)//#FC=Set touchbutton colors: #FC nf,nb,sf,sb  (monochrome: n.=normal; s.=for selection .f=foreground-; .b=background-color 0..32)
#define eDIP_AC(n1,n2,n3,n4)		eDIP_command4("AC", n1, n2, n3, n4)//#AC=Touch button: #AC n1,n2,n3,n4  (button number n1=0..255; n2=angle 0=0°;1=90°;2=180°;3=270°; n1=X-zoom 1x..8x; n2=Y-zoom 1x..8x)
#define eDIP_AR(no)					eDIP_command1("AR", no)	//#AR=Define radiogroup: #AR n1  (n1=1..255: next switch definitions will join to group n1; n1=0 next def. are free of all groups)
		//label font presets
#define eDIP_FA(nf, sf)				eDIP_command2("FA", nf, sf)	//#FA=Set touchstring color: #FA n1,s2  (n1=normal textcolor; s2=textcolor for selection; n1/s2=palette 1..32)
#define eDIP_AF(no)					eDIP_command1("AF", no)		//#AF=Touchlabel font: #AF n1  (Set font with the number n1=0..255 for touch key label)
#define eDIP_AZ(x, y)				eDIP_command2("AZ", x, y)	//#AZ=Touchlabel zoom: #AZ n1,n2  (n1=X-zoom factor 1x..8x; n2=Y-zoom factor 1x..8x)
#define eDIP_AY(x, y)				eDIP_command2("AY", x, y)	//#AY=Touchlabel char width/height: #AY n1,n2  (adds n1=0..15 dots at left/right and n2=0..15 dots at top/bottom of each character)
#define eDIP_AW(n1)					eDIP_command1("AW", n1)		//#AW=Touchlabel angle: #AW n1  (Touchtext output angle n1: 0=0°; 1=90°; 2=180°; 3=270°)
#define eDIP_AO(n1, n2)				eDIP_command2("AO", n1, n2)	//#AO=Touchlabel offset for selection: #AO n1,n2  (n1=X-offset; n2=Y-offset; n1,n2=0..7 +8 for negative direction)
		//define touch areas
#define eDIP_AT(x1,y1,x2,y2,dc,uc,string, len)	eDIP_touch1("AT", x1, y1, x2, y2, dc, uc, string, len)	//#AT=Define touch key: #AT xx1,yy1,xx2,yy2,down,up,"text"  (xx1,yy1,xx2,yy2=rectangle; down:ONcode; up:OFFcode; down/up=0 ignore)
#define eDIP_AU(x1,y1,no,dc,uc,string, len)		eDIP_touch2("AU", x1, y1, no, dc, uc, string, len)			//#AU=Define touch key: #AU xx1,yy1,down,up,"text"  (actually button at xx1,yy2; down:presscode; up:releasecode; down/up=0 ignore)
#define eDIP_AK(x1,y1,x2,y2,dc,uc,string, len)	eDIP_touch1("AK", x1, y1, x2, y2, dc, uc, string, len)	//#AK=Define touch switch: #AK xx1,yy1,xx2,yy2,down,up,"text"  (xx1,yy1,xx2,yy2=rectangle; down:ONcode; up:OFFcode; down/up=0 ignore)
#define eDIP_AJ(x1,y1,no,dc,uc,string, len)		eDIP_touch2("AJ", x1, y1, no, dc, uc, string, len)			//#AJ=Define touch switch: #AJ xx1,yy1,down,up,"text"  (actually button xx1,yy2; down: ON code; up: OFF code; down/up=0 ignore)
#define eDIP_AD(x1,y1,x2,y2,n1,fg)				eDIP_draw("AD", x1, y1, x2, y2, ((fg<<8)&0xFF00)+(n1&0x00FF), 0, 5*2) //#AD=Define drawing area: #AD xx1,yy1,xx2,yy2,n1,fg  (Draw with a linewidth n1=1..15 and color fg=1..32 within xx1,yy1<->xx2,yy2)
#define eDIP_AH(x1,y1,x2,y2)					eDIP_draw("AH", x1, y1, x2, y2, 0, 0, 4*2)	//#AH=Define free touch area: #AH xx1,yy1,xx2,yy2  (Actions down,up,drag within the rectangle xx1,yy1<->xx2,yy2 are sent)
#define eDIP_AB(n1)								eDIP_command1("AB", n1)	//#AB=Set bargraph by touch: #AB n1  (The bargraph with the n1=1..20 is defined for input by touch panel)
#define eDIP_APl(n1)							eDIP_command1("A+", n1) //#A+=Set instrument by touch: #A+ n1  (The instrument with the n1=1..4 is defined for input by touch panel)
		//global settings
#define eDIP_AA(n1)				eDIP_command1("AA", n1)	//#AA=Touch query on/off: #AA n1  (n1=0 Touch query is deactivated or n1=1 activated)
#define eDIP_AI(n1)				eDIP_command1("AI", n1)	//#AI=Touch key inverse: #AI n1  (Automatic inversion when touch key touched: n1=0=OFF; n1=1=ON)
#define eDIP_AS(n1)				eDIP_command1("AS", n1)	//#AS=Touch key buzzer: #AS n1  (beeper output is going on when a touch key is touched: n1=0=OFF; n1=1=ON)
#define eDIP_AQ(n1)				eDIP_command1("AQ", n1) //#AQ=Send bar value: #AQ n1  (n1=0 Transmission of a new bar value is deactivated; n1=1 activated; n1=2 continous send)
		//other functions
#define eDIP_AN(code)			eDIP_command1("AN", code)	  //#AN=Invert touch key: #AN code  (The touch key with the assigned by code is inverted manually)
#define eDIP_AP(code,n1)		eDIP_command2("AP", code, n1) //#AP=Set touch switch: #AP code,n1  (The status of the switch is changed by means of a command n1=0=off; n1=1=on)
#define eDIP_AX(code)			eDIP_command1("AX", code)	  //#AX=Query touch switch: #AX code  (The status of the switch off=0 on=1 is sent via the serial interface)
#define eDIP_AG(n1)				eDIP_command1("AG", n1)		  //#AG=Query radiogroup: #AG n1  (The code of activ switch in radiogroup n1 is sent via the serial interface)
#define eDIP_AL(code,n1)		eDIP_command2("AL", code, n1) //#AL=Delete touch area: #AL code,n1  (Remove area from query; code=1..255; 0: all areas; n1: 0=area remains visible; 1=delete area)
#define eDIP_AV(x1,y1,n1)		eDIP_draw("AV", x1, y1, n1, 0, 0, 0, 2*2+1) //#AV=Delete touch area: #AV xx1,yy1,n1  (Remove area witch includes point xx1,yy1 from query; n1:0=area remains visible; 1=del.area)

	//Commands for backlight, I/O-Port and misc
		//Backlight
#define eDIP_YH(n1)			eDIP_command1("YH", n1)	//#YH=Backlight brightness: #YH n1  (Adjust brightness of backlight n1=0..100% non linear)
#define eDIP_YN()			eDIP_command("YN")		//#YN=next backlight brightness: #YN  (increase the brightness of the backlight)
#define eDIP_YP()			eDIP_command("YP")		//#YP=previous backlight brightness: #YP  (decrease the brightness of the backlight)
#define eDIP_YZ(n1)			eDIP_command1("YZ", n1)	//#YZ=brightness changetime: #YZ n1  (n1=0..31 time in 1/10s for changing brightness of the backlight from 0 to 100%)
#define eDIP_YL(n1)			eDIP_command1("YL", n1)	//#YL=Backlight on/off: #YL n1  (LED backlight n1: 0=OFF; 1=ON; 2..255=switches backlight for n1/10s on and then off)
#define eDIP_YB(no)			eDIP_command1("YB", no)	//#YB=assign bar with backlight: #YB n1  (assign bar n1=1..20 for changing brightness of the backlight)
#define eDIP_YPl(no)		eDIP_command1("Y+", no)	//#Y+=assign instrument with backlight: #Y+ n1  (assign instrument n1=1..4 for changing brightness of the backlight)
#define eDIP_YAt()			eDIP_command("Y@")		//#Y@=Save backlight parameter: #Y@  (Save backlight start-brightness/-time in internal EEPROM)
		//port commands
#define eDIP_YW(n1,n2)			eDIP_command2("YW", n1, n2)	//#YW=Write output port: #YW n1,n2  (n1=0: Set all ports to n2=8-bit binary value; n1=1..8: n2=0 Reset; n2=1 Set; n2=2 Invert port)
#define eDIP_YR(n1)				eDIP_command1("YR", n1)		//#YR=Read input port: #YR n1  (n1=0: Read all ports as 8-bit binary value; n1=1..8: Read port n1 and return 0 or 1)
#define eDIP_YA(n1)				eDIP_command1("YA", n1)		//#YA=Port scan on/off: #YA n1  (The automatic scan of the input port is n1=0: deactivated; n1=1: activated)
#define eDIP_YI(n1)				eDIP_command1("YI", n1)		//#YI=Invert input port: #YI n1  (The input port is n1=0: normal; n1=1: evaluated inverted)
#define eDIP_YM(n1,n2,n3)		eDIP_command3("YM", n1, n2, n3)	//#YM=Matrix keyboard: #YM n1,n2,n3  (Specifies a matrix keyboard. n1=number of inputs 1..8; n2=outputs 0..8; n3=debouncing 0..7)
#define eDIP_YD(n1,n2,n3)		eDIP_command3("YD", n1, n2, n3)	//#YD=Redefine input Bitmacro: #YD n1,n2,n3  (input port n1=1..8; falling-/rising- edge n2=0/1; new BitMacro number n3=0..255)
#define eDIP_YX(n1,n2)			eDIP_command2("YX", n1, n2)		//#YX=Redefine Matrixmacro: #YX n1,n2  (assign matrix keynumber n1=0..65 with new MatrixMacro number n2=0..255)
		//other commands
#define eDIP_FP(no,R5,G5,B5)	eDIP_command4("FP", no, R5, G5, B5) //#FP=Set color palette: #FP n1,r,g,b  (palette entry n1=1..32; r=red valid bits7..3; g=green valid bits7..2; b=blue valid bits7..3)
#define eDIP_YS(n1)				eDIP_command1("YS", n1)	//#YS=Beep on/off: #YS n1  (Switch beeper output n1=2..255 for n1/10s to high; n1=0 set permanent low; n1=1 set permanent high)
#define eDIP_SV()				eDIP_command("SV")		//#SV=Send version: #SV  (Software version will be sent as a string e.g. "EA eDIPTFT43-A V1.0 Rev.A")


//-----------react on display answers----------//
//Use this funciton only as example, you should manipulate or change them for
//your purpose.
_Bool answerdecode	(void); //called everytime if any answer is available
_Bool react_A		(void); //called if touch button is activated
_Bool react_B		(void); //called if bar graph is touched
_Bool react_V		(void); //called if version is send

//--------------internal function--------------//
// do not use directly, use instead macro functions listed above
#define eDIP_command(ins)					eDIP_command6(ins,0,0,0,0,0,0,0)
#define eDIP_command1(ins,n1)				eDIP_command6(ins,n1,0,0,0,0,0,1)
#define eDIP_command2(ins,n1,n2)			eDIP_command6(ins,n1,n2,0,0,0,0,2)
#define eDIP_command3(ins,n1,n2,n3)			eDIP_command6(ins,n1,n2,n3,0,0,0,3)
#define eDIP_command4(ins,n1,n2,n3,n4)		eDIP_command6(ins,n1,n2,n3,n4,0,0,4)
#define eDIP_command5(ins,n1,n2,n3,n4,n5)	eDIP_command6(ins,n1,n2,n3,n4,n5,0,5)

_Bool eDIP_command6(const char * data, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4, unsigned char data5, unsigned char data6, unsigned char parameter);
_Bool eDIP_string(const char * data, unsigned int x, unsigned int y, const char * string, unsigned char length);
_Bool eDIP_string2(const char * data, const char * string, unsigned char length);
_Bool eDIP_draw(const char * data, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3, unsigned char parameters);
_Bool eDIP_bargraph(const char * data, unsigned char nr, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char sv, unsigned char ev, unsigned char type, unsigned char mst);
_Bool eDIP_touch1(const char * data, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char dc, unsigned char uc, const char * string, unsigned char length);
_Bool eDIP_touch2(const char * data, unsigned int x1, unsigned int y1, unsigned char no, unsigned char dc, unsigned char uc, const char * string, unsigned char length);

#endif


//Look-Up Table for DSP Volume Control



//---------------------EOF---------------------//