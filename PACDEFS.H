#define CLRSCR 12
#define CURSORON 130
#define CURSOROFF 131
#define ESC 27
#define TEXTPOS 41      /* X-Position der Bildschirm-Texte */
#define GAME1	'1'
#define GAME2	'2'
#define GAME3	'3'
#define GAME4	'4'
#define GAME5	'5'
#define GAME6	'6'
#define GAME7	'7'
#define GAME8	'8'
#define MGTYPE	4	    /* maximum game types */
#define	MAXPAC	3	    /* maximum number of pacmen to start */
#define	MAXMONSTER	4	/* max number of monsters */
#define EMPTY	'E'
#define FULL	'F'
#define LEFT	8
#define RIGHT	9
#define	NORTH	11
#define	DOWN	10
#define LEFT2	19		/* ^S */
#define RIGHT2	4		/* ^D */
#define	NORTH2	5		/* ^E */
#define	DOWN2	24		/* ^X */
#define HALT	'H'
#define HALT2	'h'
#define ABORT	'\3'
#define QUIT	'Q'
#define QUIT2	'q'
#define BUF_SIZE 32
#define	TRUE    1
#define	FALSE   0
#define	UPINT   (-1)
#define	DOWNINT 1
#define	LEFTINT (-2)
#define	RIGHTINT 2
#define	PACMAN   '@'
#define	MONSTER  'M'
#define	RUNNER   'S'
#define	TREASURE '$'
#define	CHOICE   '*'
#define	GOLD     '.'
#define	POTION   '%'
#define	VACANT   ' '    /* space */
#define	WALL     '#'
#define	GATE     '-'
#define	START	0
#define	RUN	1
#define	FAST	1
#define	SLOW	0
#define	PSTARTX	18
#define	PSTARTY	17
#define	MSTARTX	16	/* monster starting position */
#define	MSTARTY	10	/* monster starting position */
#define	MBEGINX	18	/* monster beginning position */
#define	MBEGINY	7	/* monster beginning position */
#define	TRYPOS	13
#define	TRXPOS	20
#define	GOTONE	1
#define	TURKEY	(-1)
#define	DUP	1
#define	DDOWN	4
#define	DRIGHT	3
#define	DLEFT	2
#define	DNULL	0
#define	BRDX	40
#define	BRDY	23
#define XWRAP	38
#define	TREASVAL	20
#define	KILLSCORE	10
#define	BEEP		'\07'  /* ctrl-g */
#define	MSTARTINTVL	10
#define	POTINTVL	25
#define GOLDCNT	185
#define PUP	'^'
#define PDOWN	'v'
#define PLEFT	'<'
#define PRIGHT	'>'

#define col_gray    71
#define col_black   72
#define col_red     73
#define col_green   74
#define col_yellow  75
#define col_blue    76
#define col_magenta 77
#define col_cyan    78
#define col_white   79

struct	pactyp
{
	int	xpos;	/* horizontal position */
	int	ypos;	/* vertical   position */
	int	dirn;	/* direction of travel */
	int	speed;	/* FAST/SLOW	       */
	int	danger;	/* TRUE if can eat     */
	int	stat;	/* status		*/
	int	col;	/* color		*/
};

int	rscore[MAXMONSTER];

struct pactyp monst[MAXMONSTER];

int	pacsymb,
	rounds,		/* time keeping mechanism */
	killflg,
	delay,
	potion,
	goldcnt,	/* no. of gold pieces remaining */
	potioncnt;

struct pactyp pac;

struct pactyp *pacptr;

/*
 * initbrd is used to re-initialize the display
 * array once a new game is started.
 */
char initbrd[BRDY][BRDX];

/* 
 * brd is kept for historical reasons.
 * It should only be used in the routine "which"
 * to determine the next move for a monster or
 * in the routine "monster" to determine if it
 * was a valid move. Admittedly this is redundant
 * and could be replaced by initbrd, but it is kept
 * so that someday additional intelligence or
 * optimization could be added to the choice of
 * the monster's next move. Hence, note the symbol
 * CHOICE at most points that a move decision
 * logically HAS to be made.
 */
char brd[BRDY][BRDX];

/*
 * display reflects the screen on the player's
 * terminal at any point in time.
 */
char display[BRDY][BRDX];

char message[81];	/* temporary message buffer */

int	game, killcnt, colorbws;

unsigned pscore;

