/**
 * @file Stm.cpp
 * @brief State Machine System
 * @author nodamotoki
 */
#pragma once
#ifndef STM_H_INCLUDED
#define STM_H_INCLUDED

/******************************************************************************/
/* Macro Function                                                             */
/******************************************************************************/
#define COUNTOF(array)				(sizeof(array)/sizeof((array)[0]))
#define STM_IS_ERROR(resultValue)	((resultValue) < 0)


/******************************************************************************/
/* Constant                                                                   */
/******************************************************************************/
/**
 * 関数の戻り値
 */
const int16_t STM_OK					=  0;	/* OK */
const int16_t STM_NOT_ENOUGH_MEMORY		= -1;	/* メモリが足りない */
const int16_t STM_RANGE_ERROR			= -2;	/* 範囲エラー */
const int16_t STM_NO_ACTION				= -3;	/* アクション関数が NULL */
const int16_t STM_NO_NEXT_STATE			= -4;	/* 実行時に次の状態を決めるはずなのに nextState 関数が NULL */


const int16_t STM_MAX_NUM				=  8;	/* 生成可能な状態遷移オブジェクトの個数 */
const int16_t STM_AMBIGUOUS				= -1;	/* STM_CELL.nextDefaultStateID が一意に決められず */
												/* 実行時に STM_CELL.action()の戻り値を次の状態 */
												/* として使う */


/******************************************************************************/
/* Type                                                                       */
/******************************************************************************/
/**
 * イベント発生時に実行される関数の型
 * 
 * @param currentStateID [in]			現在の状態(イベントが発生したときの状態)
 * @param nextStateID [in]				次の状態
 * @param eventID [in]					発生したイベント
 * @return								次の状態(静的に決められない場合この値が使われる)
 */
typedef int16_t (*STM_PF_ACTION)(int16_t currentStateID, int16_t nextStateID, int16_t eventID);

/**
 * 2次元の状態遷移表のセル一つ分
 */
struct STM_CELL
{
	const STM_PF_ACTION	action;					/* 現在の状態とイベントに応じた処理を行なう関数へのポインタ */
	int16_t				nextStateID;			/* 次の状態のデフォルト値。次の状態が一意に決められない場合 */
												/* STM_AMBIGUOUS をセットしておくと action() の戻り値を次の */
												/* 状態として使う。次の状態が一意に決まっているほうが処理の */
												/* 流れがわかりやすいので出来るだけこちらを使うべき */
};



/******************************************************************************/
/* Prototype                                                                  */
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
				   int16_t initialStateID);

/**
 * @brief 現在の状態と発生したイベントに応じて対応する処理を実行し状態を遷移する
 * 
 * @param stmID [in]			状態遷移マシンID
 * @param eventID [in]			イベントID
 * 
 * @return						0: OK, 0未満: エラー
 */
int16_t STM_SetEvent(int16_t stmID, int16_t eventID);

/**
 * @brief 現在の状態を取得する
 * 
 * @param stmID [in]			状態遷移マシンID
 * 
 * @return						0以上の値: 状態ID, 0未満の値: エラー
 */
int16_t STM_GetState(int16_t stmID);

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
void STM_SetState(int16_t stmID, int16_t stateID);


#endif /* STM_H_INCLUDED */
