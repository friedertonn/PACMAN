#include "bdscio.h"
#include "pacdefs.h"

update()
{
	char	str[10];

	sprintf(str, "%u", pscore);
	SPLOT(0, TEXTPOS + 8, str);
	sprintf(str, "%d ", goldcnt);
	SPLOT(20, TEXTPOS + 7, str);
}

reinit()
{
	register int locx, locy;
	register char tmp;

	for (locy = 0; locy < BRDY; locy++)
	{
		for (locx = 0; locx < BRDX; locx++)
		{
			tmp = initbrd[locy][locx];
			brd[locy][locx] = tmp;
			if ((display[locy][locx] = tmp) == CHOICE)
			{
				display[locy][locx] = GOLD;
			};
		};
	};
	goldcnt = GOLDCNT;
	delay -= (delay / 10);	/* hot it up */
}

errgen(string)
char *string;
{
	SPLOT(20, TEXTPOS, string);
}

dokill(mnum)
int mnum;
{
	register struct pactyp *mptr;

	PLOT(0, 0, BEEP);
	if (pacptr->danger == TRUE)
	{
		if (++killcnt == MAXMONSTER)
		{
			if (display[TRYPOS][TRXPOS] == GOLD)
			{
				if (goldcnt > 0) goldcnt--;
			};
			display[TRYPOS][TRXPOS] = TREASURE;
			PLOT(TRYPOS, TRXPOS, TREASURE);
			killcnt = 0;
		};
		if (killcnt == 0)
		{
			SPLOT(4, TEXTPOS, "         ");
			SPLOT(5, TEXTPOS, "            ");
		}
		else
		{
			SPLOT(4, TEXTPOS, "erledigte");
			sprintf(message, "  Monster: %d", killcnt);
			SPLOT(5, TEXTPOS, message);
		}
		mptr = (&monst[mnum]);
		mptr->ypos = MSTARTY;
		mptr->xpos = MSTARTX + (2 * mnum);
		mptr->stat = START;
		PLOT(mptr->ypos, mptr->xpos, MONSTER);
		pscore += KILLSCORE;
		return(GOTONE);
	};
	return(TURKEY);
}

/*
 * clr -- issues an escape sequence to clear the display
 */
clr()
{
	bios(4, CLRSCR);  /* CONOUT */
}

/*
 *	display initial instructions
 */
instruct()
{
	clr();
	POS(-4, -4);
	printf("\r\n\n           P A C M A N  fuer den AC1 (64x32-Terminal)\r\n\n\n");
	printf("  Achtung: Du befindest dich in einem Verlies\r\n");
	printf("           und wirst von Monstern gejagt!\r\n\n");
	printf("  Im Kerker liegen Goldstuecke verstreut und mit \".\" markiert.\r\n");
	printf("  An den mit \"%%\" gekennzeichneten Stellen ist ein Zaubertrank\r\n");
	printf("  erhaeltlich. Mit jedem Trank kannst du fuer eine begrenzte\r\n");
	printf("  Zeit Monster durch Beruehrung toeten. Er wird sie auch ab-\r\n");
	printf("  schrecken. Wenn du ein Monster toetest, wird es regeneriert,\r\n");
	printf("  aber das braucht Zeit.\r\n");
	printf("  Das Toeten aller Monster fuehrt dazu, dass irgendwo im\r\n");
	printf("  Kerker auf magische Weise ein weiterer Schatz erscheint, der\r\n");
	printf("  mit einem \"$\" markiert ist.\r\n");
	printf("  Es gibt einen magischen Tunnel, der den mittleren linken\r\n");
	printf("  und den mittleren rechten Teil des Kerkers verbindet.\r\n");
	printf("  Die Monster wissen davon! Du hast %d Leben.\r\n\n", MAXPAC);
	printf("  Du kannst Dich mit den Pfeiltasten bewegen.\r\n\n");
	printf("  Leertaste:     Spiel anhalten\r\n");
	printf("  Q oder Ctrl-C: Spiel beenden\r\n\n");
	printf("  Start: 1  -->  normales Spiel\r\n");
	printf("         2  -->  blinkende Monster\r\n");
	printf("         3  -->  intelligente Monster\r\n");
	printf("         4  -->  blinkende intelligente Monster\r\n");
}

/*
 * over -- game over processing
 */
over()
{
	register int i;
	register int line;
	sleep(50);	/* for slow readers */
	poll(0);	/* flush and discard input from player */
	clr();
	/* high score to date processing */
	if (game != 0)
	{
		line = 8;
		POS(line++, 14);
		printf(" ___________________________ ");
		POS(line++, 14);
		printf("|                           |");
		POS(line++, 14);
		printf("| G A M E   O V E R         |");
		POS(line++, 14);
		printf("|                           |");
		POS(line++, 14);
		printf("| Spiel-Typ: %d              |",game);
		POS(line++, 14);
		printf("|                           |");
		POS(line++, 14);
		printf("| Punktestand: %-5u        |", pscore);
		POS(line, 14);
		printf("|___________________________|");
	};
	leave();
}

/*
 * leave -- cursor_on & exit()
 */
leave()
{
	POS(27, -4);
    bios(4, CURSORON);  /* CONOUT */
	exit(0);
}

/*
 * init -- does global initialization and spawns a child process to read
 *         the input terminal.
 */
init()
{
	register int tries;

	tries = 0;
	pacsymb = PACMAN;
	pacptr = &pac;

	strcpy(initbrd[0], "#######################################");
	strcpy(initbrd[1], "# . . . * . . . . ### . . . . * . . . #");
	strcpy(initbrd[2], "# % ### . ##### . ### . ##### . ### % #");
	strcpy(initbrd[3], "# * . . * . * . * . . * . * . * . . * #");
	strcpy(initbrd[4], "# . ### . # . ########### . # . ### . #");
	strcpy(initbrd[5], "# . . . * # . . . ### . . . # * . . . #");
	strcpy(initbrd[6], "####### . ##### . ### . ##### . #######");
	strcpy(initbrd[7], "      # . # . . * . . * . . # . #      ");
	strcpy(initbrd[8], "      # . # . ### - - ### . # . #      ");
	strcpy(initbrd[9], "####### . # . #         # . # . #######");
	strcpy(initbrd[10],"        * . * #         # * . *        ");
	strcpy(initbrd[11],"####### . # . #         # . # . #######");
	strcpy(initbrd[12],"      # . # . ########### . # . #      ");
	strcpy(initbrd[13],"      # . # * . . . . . . * # . #      ");
	strcpy(initbrd[14],"####### . # . ########### . # . #######");
	strcpy(initbrd[15],"# . . . * . * . . ### . . * . * . . . #");
	strcpy(initbrd[16],"# % ### . ##### . ### . ##### . ### % #");
	strcpy(initbrd[17],"# . . # * . * . * . . * . * . * # . . #");
	strcpy(initbrd[18],"### . # . # . ########### . # . # . ###");
	strcpy(initbrd[19],"# . * . . # . . . ### . . . # . . * . #");
	strcpy(initbrd[20],"# . ########### . ### . ########### . #");
	strcpy(initbrd[21],"# . . . . . . . * . . * . . . . . . . #");
	strcpy(initbrd[22],"#######################################");

	strcpy(brd[0], "#######################################");
	strcpy(brd[1], "# . . . * . . . . ### . . . . * . . . #");
	strcpy(brd[2], "# % ### . ##### . ### . ##### . ### % #");
	strcpy(brd[3], "# * . . * . * . * . . * . * . * . . * #");
	strcpy(brd[4], "# . ### . # . ########### . # . ### . #");
	strcpy(brd[5], "# . . . * # . . . ### . . . # * . . . #");
	strcpy(brd[6], "####### . ##### . ### . ##### . #######");
	strcpy(brd[7], "      # . # . . * . . * . . # . #      ");
	strcpy(brd[8], "      # . # . ### - - ### . # . #      ");
	strcpy(brd[9], "####### . # . #         # . # . #######");
	strcpy(brd[10],"        * . * #         # * . *        ");
	strcpy(brd[11],"####### . # . #         # . # . #######");
	strcpy(brd[12],"      # . # . ########### . # . #      ");
	strcpy(brd[13],"      # . # * . . . . . . * # . #      ");
	strcpy(brd[14],"####### . # . ########### . # . #######");
	strcpy(brd[15],"# . . . * . * . . ### . . * . * . . . #");
	strcpy(brd[16],"# % ### . ##### . ### . ##### . ### % #");
	strcpy(brd[17],"# . . # * . * . * . . * . * . * # . . #");
	strcpy(brd[18],"### . # . # . ########### . # . # . ###");
	strcpy(brd[19],"# . * . . # . . . ### . . . # . . * . #");
	strcpy(brd[20],"# . ########### . ### . ########### . #");
	strcpy(brd[21],"# . . . . . . . * . . * . . . . . . . #");
	strcpy(brd[22],"#######################################");

	strcpy(display[0], "#######################################");
	strcpy(display[1], "# . . . . . . . . ### . . . . . . . . #");
	strcpy(display[2], "# % ### . ##### . ### . ##### . ### % #");
	strcpy(display[3], "# . . . . . . . . . . . . . . . . . . #");
	strcpy(display[4], "# . ### . # . ########### . # . ### . #");
	strcpy(display[5], "# . . . . # . . . ### . . . # . . . . #");
	strcpy(display[6], "####### . ##### . ### . ##### . #######");
	strcpy(display[7], "      # . # . . . . . . . . # . #      ");
	strcpy(display[8], "      # . # . ### - - ### . # . #      ");
	strcpy(display[9], "####### . # . #         # . # . #######");
	strcpy(display[10],"        . . . #         # . . .        ");
	strcpy(display[11],"####### . # . #         # . # . #######");
	strcpy(display[12],"      # . # . ########### . # . #      ");
	strcpy(display[13],"      # . # . . . . . . . . # . #      ");
	strcpy(display[14],"####### . # . ########### . # . #######");
	strcpy(display[15],"# . . . . . . . . ### . . . . . . . . #");
	strcpy(display[16],"# % ### . ##### . ### . ##### . ### % #");
	strcpy(display[17],"# . . # . . . . . . . . . . . . # . . #");
	strcpy(display[18],"### . # . # . ########### . # . # . ###");
	strcpy(display[19],"# . . . . # . . . ### . . . # . . . . #");
	strcpy(display[20],"# . ########### . ### . ########### . #");
	strcpy(display[21],"# . . . . . . . . . . . . . . . . . . #");
	strcpy(display[22],"#######################################");

	killcnt = 0;
	srand(0);	/* start rand randomly */
	delay = 2000;

	/*
	 * New game starts here
	 */
	game = 0;
	instruct();
	while (game == 0)
		poll(1);

	goldcnt = GOLDCNT;
	pscore = 0;
	clr();
	bios(4, CURSOROFF);  /* CONOUT */
}

/*
 * poll -- read characters sent by input subprocess and set global flags
 */
poll(sltime)
{
	int stop;
	int command;

	if(bios(2) == 0) return;  /* CONST */

	readin:
	command = bios(3);  /* CONIN */

	switch(command & 0177)
	{
	case LEFT:
	case LEFT2:
		pacptr->dirn = DLEFT;
		break;

	case RIGHT:
	case RIGHT2:
		pacptr->dirn = DRIGHT;
		break;

	case NORTH:
	case NORTH2:
		pacptr->dirn = DUP;
		break;

	case DOWN:
	case DOWN2:
		pacptr->dirn = DDOWN;
		break;

	case HALT:
	case HALT2:
		pacptr->dirn = DNULL;
		break;

	case ABORT:
	case QUIT:
	case QUIT2:
		over();
		break;

	case CNTLS:
		stop = 1;
		goto readin;

	case GAME1:
		game = 1;
		break;

	case GAME2:
		game = 2;
		break;

	case GAME3:
		game = 3;
		break;

	case GAME4:
		game = 4;
		break;

	default:
		goto readin;
	}
}

getrand(range)
int range;
{
	unsigned q;

	q = rand();
	return(q % range);
}

/*
 * The PLOT function is normally defined using a preprocessor macro:
 * #define PLOT(A,B,C)  POS(A,B);putchar(C)
 */
PLOT(row, col, ch)
int row, col;
char ch;
{
	POS(row, col);
	if ((ch >= 32) && (ch < 128)) bios(4, ch);  /* CONOUT */
}

/*
 * The SPLOT function is normally defined as:
 * #define SPLOT(A,B,S) POS(A,B);printf("%s",s)
 */
SPLOT(row, col, str)
int row, col;
char *str;
{
	int i;
	
	POS(row, col);
	for(i = 0; i < strlen(str); i++) bios(4, str[i]);  /* CONOUT */
}

/*
 * The POS function us normally defined, using termcap, as:
 * #define POS(row,col) tputs(tgoto(vs_cm,(col),(row),1,putch)
 */
POS(row, col)
int row, col;
{
	bios(4, ESC);  /* CONOUT */
	bios(4, 132 + row);
	bios(4, 4 + col);
}

