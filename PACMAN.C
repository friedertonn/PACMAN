/*
 * PACMAN  - written by Dave Nixon, AGS Computers Inc., July, 1981.
 *
 * Terminal handling for video games taken from aliens
 * the original version of aliens is from
 * Fall 1979   Cambridge   Jude Miller
 *
 * Score keeping modified and general terminal handling (termcap routines
 * from UCB's ex) added by Rob Coben, BTL, June, 1980.
 *
 * Adapted for BDS C by Jeff Martin	Naperville, Il February 1982
 * As distributed, this program is set up for the H19 terminal.
 * All that should be necessary to modify it for another terminal
 * type is to make the appropriate changes to the initPOS and POS
 * functions, located at the very end of PACUTIL.C.  These functions
 * define the cursor-addressing sequence.
 *
 * Angepasst fuer das AC1-CP/M mit 64x32-Terminal im Januar 2024
 * Angepasst fuer AC1-2010 Color-BWS im August 2024
 *
 * To compile:
 *		CC PACMAN.C
 *		CC PACMONST.C
 *		CC PACUTIL.C
 *
 * To link:
 *		CLINK PACMAN PACMONST PACUTIL DEFF
 */

#include "bdscio.h"
#include "pacdefs.h"

main()
{
	register int tmp;       /* temp variables */
	register int pac_cnt;
	register int monstcnt;  /* monster number */
	struct pactyp *mptr;

	init();  /* global init */
	for (pac_cnt = MAXPAC; pac_cnt > 0; pac_cnt--)
	{
	redraw:
		clr();
		sprintf(message, "Punkte: %u", pscore);
		SPLOT(0, TEXTPOS, message);
		sprintf(message, "Gold = %d", goldcnt);
		SPLOT(20, TEXTPOS, message);

		if (potion == TRUE)
		{
			SPLOT(2, TEXTPOS, "Countdown: ");
		};
		pacsymb = PACMAN;
		killflg = FALSE;
		sprintf(message, "Delay = %d", delay);
		SPLOT(22, TEXTPOS, message);

		/*
		 * PLOT maze
		 */
		for (tmp = 0; tmp < BRDY; tmp++)
		{
			SPLOT(tmp, 0, &(display[tmp][0]));
		};

		/* initialize a pacman */
		pac.xpos = PSTARTX;
		pac.ypos = PSTARTY;
		pac.dirn = DNULL;
		pac.speed = SLOW;
		pac.danger = FALSE;
		pac.col = col_yellow;
		colorPLOT(pacptr->ypos, pacptr->xpos, pacsymb, pacptr->col);

		/* display remaining pacmen */
		SPLOT(24, 0, "verbleibende Leben:");
		for (tmp = 0; tmp < pac_cnt - 1; tmp++)
		{
			colorPLOT(24, ((tmp * 2) + 20), PACMAN, pacptr->col);
		};

		/*
		 * Init. monsters
		 */
		for (monstcnt = 0; monstcnt < MAXMONSTER; monstcnt++)
		{
			mptr = (&monst[monstcnt]);
			mptr->xpos = MSTARTX + (2 * monstcnt);
			mptr->ypos = MSTARTY;
			mptr->speed = SLOW;
			mptr->dirn = DNULL;
			mptr->danger = FALSE;
			mptr->stat = START;
			if (monstcnt == 0) mptr->col = col_magenta;
			else if (monstcnt == 1) mptr->col = col_red;
			else if (monstcnt == 2) mptr->col = col_green;
			else mptr->col = col_cyan;
			colorPLOT(mptr->ypos, mptr->xpos, MONSTER, mptr->col);
		};
		rounds = 0;	/* timing mechanism */

		/* 
		 * main game loop
		 */
		do
		{
			if (rounds++ % MSTARTINTVL == 0)
			{
				startmonst();
			};
			pacman();
			if (killflg == TURKEY)
				break;
			for (monstcnt = 0; monstcnt < (MAXMONSTER / 2); monstcnt++)
			{
				monster(monstcnt);	/* next monster */
			};
			if (killflg == TURKEY)
				break;
			if (pacptr->speed == FAST)
			{
				pacman();
				if (killflg == TURKEY)
					break;
			};
			for (monstcnt = (MAXMONSTER / 2); monstcnt < MAXMONSTER; monstcnt++)
			{
				monster(monstcnt);	/* next monster */
			};
			if (killflg == TURKEY)
				break;
			if (potion == TRUE)
			{
				sprintf(message, "%d ", potioncnt);
				SPLOT(2, TEXTPOS + 11, message);
				if (--potioncnt <= 0)
				{
					SPLOT(2, TEXTPOS, "             ");
					potion = FALSE;
					pacptr->speed = SLOW;
					pacptr->danger = FALSE;
					for (monstcnt = 0; monstcnt < MAXMONSTER; monstcnt++)
					{
						monst[monstcnt].danger = TRUE;
					};
				};
			};
			update();	/* score display etc */
			if (goldcnt <= 0)
			{
				reinit();
				goto redraw;
			};
		} while (killflg != TURKEY);
		SPLOT(8, TEXTPOS, "Du wurdest");
		SPLOT(9, TEXTPOS, "gefressen!");
		SPLOT(11, TEXTPOS, "Das hat etwa");
		SPLOT(12, TEXTPOS, "2 Sek. gedauert");
		sleep(20);
	};
	SPLOT(14, TEXTPOS, "Zum Schluss");
	SPLOT(15, TEXTPOS, "gewinnen immer");
	SPLOT(16, TEXTPOS, "die Monster!");
	over();
}

pacman()
{
	register int sqtype;
	register int monstcnt;
	register int tmpx, tmpy;
	int deltat;
	struct pactyp *mptr;

	/* pause; wait for the player to hit a key */
	for (deltat = delay; deltat > 0; deltat--);

	/* get instructions from player, but don't wait */
	poll(0);

	/* remember current pacman position */
	tmpx = pacptr->xpos;
	tmpy = pacptr->ypos;

	/* "eat" any gold */
	/* update display array to reflect what is on terminal */
	display[tmpy][tmpx] = VACANT;

	/* what next? */
	switch (pacptr->dirn)
	{
	case DUP:
		pacsymb = PUP;
		switch (sqtype = display[tmpy + UPINT][tmpx])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->ypos += UPINT;
			break;

		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	case DDOWN:
		pacsymb = PDOWN;
		switch (sqtype = display[tmpy + DOWNINT][tmpx])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->ypos += DOWNINT;
			break;

		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	case DLEFT:
		if(tmpx == 0)
		{
			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos = XWRAP;
			sqtype = VACANT;
			break;
		};
		pacsymb = PLEFT;
		switch (sqtype = display[tmpy][tmpx + LEFTINT])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos += LEFTINT;
			break;
		
		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	case DRIGHT:
		if(tmpx == XWRAP)
		{
			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos = 0;
			sqtype = VACANT;
			break;
		};
		pacsymb = PRIGHT;
		switch (sqtype = display[tmpy][tmpx + RIGHTINT])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos += RIGHTINT;
			break;

		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	};

	/* did the pacman get any points or eat a potion? */
	switch (sqtype)
	{
	case CHOICE:
	case GOLD:
		pscore++;
		if (goldcnt > 0) goldcnt--;
		break;

	case TREASURE:
		pscore += TREASVAL;
		break;

	case POTION:
		SPLOT(2, TEXTPOS, "Countdown: ");
		potion = TRUE;
		potioncnt = POTINTVL;
		pacptr->speed = FAST;
		pacptr->danger = TRUE;

		/* slow down monsters and make them harmless */
		for (monstcnt = 0; monstcnt < MAXMONSTER; monstcnt++)
		{
			mptr = (&monst[monstcnt]);
			if (mptr->stat == RUN)
			{
				mptr->speed = SLOW;
				mptr->danger = FALSE;
			};
		};
		break;
	};

	/* did the pacman run into a monster? */
	for (monstcnt = 0; monstcnt < MAXMONSTER; monstcnt++)
	{
		mptr = (&monst[monstcnt]);
		if ((mptr->xpos == pacptr->xpos) &&
			(mptr->ypos == pacptr->ypos))
		{
			killflg = dokill(monstcnt);
		}
		else
		{
			killflg = FALSE;
		};
	};
	if (killflg != TURKEY)
	{
		colorPLOT(pacptr->ypos, pacptr->xpos, pacsymb, pacptr->col);
	};
}

