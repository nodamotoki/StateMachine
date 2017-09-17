/**
 * @file Stm.cpp
 * @brief State Machine System
 * @author nodamotoki
 */

#define STM_STATE_MAHINE_TEST_ON
#ifdef STM_STATE_MAHINE_TEST_ON
#include <stdio.h>
#include "Stm.h"

enum {
	ST_Tt,
	ST_Tj,
	ST_Jt,
	ST_Jj,
	ST_Kt,
	ST_Kj,
};

enum {
	EV_BtnT,
	EV_BtnJ,
	EV_RdoT,
	EV_RdoJ,
	EV_Down,
	EV_Up,
	EV_Set,
	EV_Clr,
};

int16_t Nop(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t Ban(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnBtnT(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnBtnJ(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnRdoT(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnRdoJ(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnDown(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnUp(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnSet(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}

int16_t OnClr(int16_t currentStateID, int16_t nextStateID, int16_t eventID)
{
	return 0;
}


static int16_t stateList[] = {ST_Tt, ST_Tj, ST_Jt, ST_Jj, ST_Kt, ST_Kj};
static int16_t eventList[] = {EV_BtnT, EV_BtnJ, EV_RdoT, EV_RdoJ, EV_Down, EV_Up, EV_Set, EV_Clr};
static STM_CELL stm[] = {
/*       ST_Tt           ST_Tj           ST_Jt        ST_Jj        ST_Kt        ST_Kj */
/*BtnT*/{Nop,    ST_Tt},{OnBtnT, ST_Tt},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
/*BtnJ*/{OnBtnJ, ST_Jj},{Nop,    ST_Tj},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
/*RdoT*/{Ban,    ST_Tt},{Nop,    ST_Tj},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
/*RdoJ*/{Ban,    ST_Tt},{Nop,    ST_Tj},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
/*Down*/{OnDown, ST_Kt},{Nop,    ST_Tj},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
/*Up  */{Nop,    ST_Tt},{Nop,    ST_Tj},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
/*Set */{OnSet,  ST_Kt},{Nop,    ST_Tj},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
/*Clr */{Nop,    ST_Tt},{Nop,    ST_Tj},{Nop, ST_Jt},{Nop, ST_Jj},{Nop, ST_Kt},{Nop, ST_Kj},
};

int main(int argc, char *argv[])
{
	const char* str[] = {"ST_Tt", "ST_Tj", "ST_Jt", "ST_Jj", "ST_Kt", "ST_Kj"};

	int16_t stmID = STM_Create(stm,
							   stateList, (int16_t)COUNTOF(stateList),
							   eventList, (int16_t)COUNTOF(eventList),
							   ST_Tt);
	printf("stateID=%s\n", str[STM_GetState(stmID)]);

	STM_SetEvent(stmID, EV_BtnJ);
	printf("stateID=%s\n", str[STM_GetState(stmID)]);

	/* ↓エラーになるはず */
	STM_SetEvent(stmID, 0x1000);
	printf("stateID=%s\n", str[STM_GetState(stmID)]);

	/* initalState が範囲外  */
	int16_t stmID2 = STM_Create(stm,
								stateList, (int16_t)COUNTOF(stateList),
								eventList, (int16_t)COUNTOF(eventList),
								0x0400);
	printf("stateID=%s\n", str[STM_GetState(stmID2)]);

	STM_SetState(stmID2, ST_Tt);
	printf("stateID=%s\n", str[STM_GetState(stmID2)]);
	return 0;
}

#endif /* STM_STATE_MAHINE_TEST_ON */
