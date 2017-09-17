/**
 * @file Stm.cpp
 * @brief State Machine System
 * @author nodamotoki
 */

#include <stdio.h>
#include "Stm.h"

/******************************************************************************/
/* Macro Function                                                             */
/******************************************************************************/
#ifdef STM_DEBUG_ON
#define STM_DEBUG_PRINT(...)		do { printf("%s:%d:%s:", __FILE__, __LINE__, __func__); printf(__VA_ARGS__); } while(0)
#endif /* STM_DEBUG_ON */

/******************************************************************************/
/* Type                                                                       */
/******************************************************************************/
/**
 * 状態遷移オブジェクト
 * 状態遷移表と現在の状態を持つ
 */
struct STM_OBJECT
{
	const STM_CELL*		matrix;					/* 状態遷移表へのポインタ(状態遷移表の実体は STM_CELL の */
												/* 1次元配列だがステート軸とイベント軸を持つ2次元の表として使う) */
	const int16_t*		stateList;				/* ステート軸(横軸)の配列へのポインタ */
	const int16_t*		eventList;				/* イベント軸(縦軸)の配列へのポインタ */
	int16_t				stateNum;				/* ステート軸配列の個数 */
	int16_t				eventNum;				/* イベント軸配列の個数 */
	int16_t				stateID;				/* 現在の状態 */
};

/**
 * 状態遷移の情報を一括管理するメモリ
 */
struct STM_STORAGE
{
	STM_OBJECT			stmObjects[STM_MAX_NUM];	/* 状態遷移オブジェクト用のメモリ */
	int16_t				stmNum;						/* 状態遷移オブジェクトの個数 */
};

/******************************************************************************/
/* Data                                                                       */
/******************************************************************************/
static STM_STORAGE s_strage;


/******************************************************************************/
/* Prototype                                                                  */
/******************************************************************************/
static int16_t STM_GetIndex(const int16_t* array, size_t arrayNum, int16_t searchKey);


/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
/**
 * @brief 状態遷移オブジェクトを生成し、戻り値としてそのIDを取得する
 * 
 * @param matrix [in]			状態遷移表へのポインタ(状態遷移表の実体は STM_CELL の 
 * 								1次元配列だがステート軸とイベント軸を持つ2次元の表として使う)
 * @param stateList [in]		ステート軸(横軸)の配列へのポインタ
 * @param eventList [in]		イベント軸(縦軸)の配列へのポインタ
 * @param stateNum [in]			ステート軸配列の個数
 * @param eventNum [in]			イベント軸配列の個数
 * @param initialStateID [in]	現在の状態
 * 
 * @return						0以上の値: 状態遷移オブジェクトのID, 0未満の値: エラー
 */
int16_t STM_Create(const STM_CELL* matrix,
				   const int16_t* stateList,
				   int16_t stateNum,
				   const int16_t* eventList,
				   int16_t eventNum,
				   int16_t initialStateID)
{
	int16_t stmID = s_strage.stmNum;

	if (stmID >= STM_MAX_NUM) {
		/* 状態遷移オブジェクトを作りすぎ。*/
		/* s_strage.stmObjects[]配列のサイズを増やす必要がある。 */
		return STM_NOT_ENOUGH_MEMORY;
	}

	/* ストレージからカラの状態遷移オブジェクトを一つ取得 */
	STM_OBJECT* self = &s_strage.stmObjects[stmID];

	/* 状態遷移オブジェクトを初期設定 */
	self->matrix	= matrix;
	self->stateList = stateList;
	self->eventList = eventList;
	self->stateNum	= stateNum;
	self->eventNum	= eventNum;
	self->stateID	= initialStateID;

	/* ストレージに保持している状態遷移オブジェクトの個数をインクリメント */
	s_strage.stmNum += 1;

	/* 状態遷移オブジェクトのIDを返す */
	return stmID;
}

/**
 * @brief 配列を検索しヒットした要素のindexを返す
 * 
 * @param array [in]			検索する配列へのポインタ
 * @param arrayNum [in]			配列の要素数
 * @param searchKey [in]		検索する値
 * 
 * @return						ヒットした要素のインデックス。ヒットしないときは -1
 */
static int16_t STM_GetIndex(const int16_t* array, size_t arrayNum, int16_t searchKey)
{
	int16_t result = -1;						/* エラー値 */
	for (int i = 0; i < (int16_t) arrayNum; ++i) {
		if (array[i] == searchKey) {
			result = i;
			break;
		}
	}
	return result;
}

/**
 * @brief 現在の状態と発生したイベントに応じて対応する処理を実行し状態を遷移する
 * 
 * @param stmID [in]			状態遷移マシンID
 * @param eventID [in]			イベントID
 * 
 * @return						0: OK, 0未満: エラー
 */
int16_t STM_SetEvent(int16_t stmID, int16_t eventID)
{
	/* 範囲チェック */
	if (stmID < 0 || STM_MAX_NUM <= stmID) {
		STM_DEBUG_PRINT("stmID(%d) range error.\n", stmID);
		return STM_RANGE_ERROR;
	}

	/* 状態遷移オブジェクトのIDから状態遷移オブジェクトを取得 */
	STM_OBJECT* self = &s_strage.stmObjects[stmID];

	/* ステート軸の配列から stateID を検索しそのインデックスを取得 */
	int16_t stateIndex = STM_GetIndex(self->stateList, self->stateNum, self->stateID);
	if (stateIndex < 0 || self->stateNum <= stateIndex) {
		STM_DEBUG_PRINT("stateID(%d)'s index(%d) range error.\n", self->stateID, stateIndex);
		return STM_RANGE_ERROR;
	}

	/* イベント軸の配列から eventID を検索してそのインデックスを取得 */
	int16_t eventIndex = STM_GetIndex(self->eventList, self->eventNum, eventID);
	if (eventIndex < 0 || self->eventNum <= stateIndex) {
		STM_DEBUG_PRINT("eventID(%d)'s index(%d) range error.\n", eventID, eventIndex);
		return STM_RANGE_ERROR;
	}

	/* stateを横軸、eventを縦軸と見なして matrix[] のインデックスを計算 */
	int16_t index = eventIndex * self->stateNum + stateIndex;

	/* CELL の情報を取得 */
	const STM_PF_ACTION	action		= self->matrix[index].action;
	int16_t				nextStateID = self->matrix[index].nextStateID;

	/* 念のため NULL チェック(action は NULL にすることはないと思うが) */
	if (! action) {
		STM_DEBUG_PRINT("action NULL.\n");
		return STM_NO_ACTION;
	}

	/* action関数を実行する */
	int16_t dynamicNextStateID = action(self->stateID, nextStateID, eventID);

	/* 静的に次の状態を決められない場合 */
	if (nextStateID == STM_AMBIGUOUS) {
		nextStateID = dynamicNextStateID;
	}

	/* 状態を遷移させる */
	self->stateID = nextStateID;

	return STM_OK;
}

/**
 * @brief 現在の状態を取得する
 * 
 * @param stmID [in]			状態遷移マシンID
 * 
 * @return						0以上の値: 状態ID, 0未満の値: エラー
 */
int16_t STM_GetState(int16_t stmID)
{
	/* 範囲チェック */
	if (stmID >= STM_MAX_NUM) {
		STM_DEBUG_PRINT("stmID(%d) range error.\n", stmID);
		return STM_RANGE_ERROR;
	}

	/* 状態遷移オブジェクトのIDから状態遷移オブジェクトを取得 */
	STM_OBJECT* self = &s_strage.stmObjects[stmID];

	return self->stateID;
}

/**
 * @brief 現在の状態を強制的に書き換える。デバッグ用の機能
 *
 * @note 通常は、状態遷移表と発生したイベントに応じて自動的に状態が書
 *       き換わるため直接状態を書き換えてはいけない。もし状態遷移がう
 *       まくいかず、この関数を利用したくなったなら、状態遷移表を変更
 *       すべき。
 * 
 * @param stmID [in]			状態遷移マシンID
 * @param stateID [in]			
 */
void STM_SetState(int16_t stmID, int16_t stateID)
{
	/* 範囲チェック */
	if (stmID >= STM_MAX_NUM) {
		STM_DEBUG_PRINT("stmID(%d) range error.\n", stmID);
	}

	/* 状態遷移オブジェクトのIDから状態遷移オブジェクトを取得 */
	STM_OBJECT* self = &s_strage.stmObjects[stmID];

	self->stateID = stateID;
}
