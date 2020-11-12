//**************************************************************
//		HEW2020　prototype版
//		中川柊人　キャラクター移動
//		2020/11/11
//		「この処理どーゆーこと？みたいなのあったらゆーてね！」
//**************************************************************

//定数定義
#define JUMP_POWER 5		//ジャンプの馬力を「５」に設定
#define DEFAULT_Y 30		//落下時に着地するY軸座標を「３０」に設定

//インクルード
#include <stdio.h>
#include <Windows.h>

//変数宣言
int block_x = 0;
int block_y = 0;

//メイン処理（今はmain関数だけど皆のプログラムと合体させる時は関数化しまひょ！！）
int main(void)
{
	//プレイヤー（主人公）の構造体
	struct PLAYER
	{
		int y = 30;				//Y軸の初期位置を「３０」に設定
		int x = 0;				//X軸の初期位置を「０」に設定
		int foot_markX = 0;		//キャラをX軸に動かした時に発生する足跡を消す為の変数
		int foot_markY = 0;		//キャラをY軸に動かした時に発生する足跡を消す為の変数
		int jump = 0;			//キャラのジャンプ変数。基本は「０」に設定
		int jump_flag;			//ジャンプのフラグ判定のための変数
	};
	struct PLAYER player;

	/*【定数の「JUMP＿POWER」と変数の「jump」を分けてる理由】
	
		基本的なジャンプの馬力を設定しておく事で今後何かしらのアイテムで
		通常のジャンプより高くなったり、低くなったりと変数の数字をその処理の
		中で代入するだけで、変数の値の塗り替えバグを無くす為、多様性が出るからだよん
	*/
	
	//カーソル位置の設定
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;

	//メインループ処理（Whileだと汎用性が厳しいなら言ってね！）
	while (1)
	{
		//操作するキャラの座標を足跡消す変数にセット
		player.foot_markX = player.x;
		player.foot_markY = player.y;

		//入力処置
		//ジャンプ（SPACE）
		if (GetAsyncKeyState(VK_SPACE) && !player.jump_flag) {
			player.jump = -JUMP_POWER;
			player.jump_flag = true;
		}
		if (player.jump_flag) {
			player.y += player.jump;
			player.jump += 3;
		}
		if (player.y > DEFAULT_Y) {
			player.y = DEFAULT_Y;
			player.jump_flag = false;
		}
		//右移動（アローキー右）
		if (GetAsyncKeyState(VK_RIGHT)){
			player.x += 2;

			//移動範囲の制御
			if (player.x > 60 - 2){
				player.x = 60 - 2;
			}
		}
		//左移動（アローキー左）
		if (GetAsyncKeyState(VK_LEFT)){
			player.x -= 2;

			//移動範囲の制御
			if (player.x < 0){
				player.x = 0;
			}
		}

		//カーソルに足跡座標を代入して描画を塗りつぶす為に黒色に設定
		pos.X = player.foot_markX;
		pos.Y = player.foot_markY;
		SetConsoleCursorPosition(hCons, pos);
		printf("　");

		//カーソルにプレイヤー座標を代入して描画を「☆」に設定
		pos.X = player.x;
		pos.Y = player.y;
		SetConsoleCursorPosition(hCons, pos);
		printf("☆");

		/*for (block_y = 0; block_y <= 6; block_y++)
		{
			for (block_x = 0; block_x <= 30; block_x++)
			{
				if (block_y == 0 && block_x == 0 || block_y == 0 && block_x == 30 || block_y == 6 && block_x == 0 || block_y == 6 && block_x == 30)
				{
					printf("■");
				}
				else if (block_y == 0 || block_y == 6)
				{
					printf("■");
				}
				else
				{
					printf("  ");
				}
			}
			printf("\n");
		}
		break;
		*/
		//更新フレーム制御を「５０」に設定
		Sleep(50);
	}
}