//インクルード
#include "game.h"
#include "dx11sprite.h"
#include "input.h"

//定数定義
#define MAX_SPRITE  6			// ゲームで使うスプライトの数(とりあえず３に設定)
#define JUMP_POWER  0.005f		//ジャンプの馬力を「0.003f」に設定
#define BLOCK_SPRITE 10
#define DEFAURT_Y -0.75f
#define DIR_Right		0.4f		//サク子が右方向を向いた時に代入されるUV座標を定数化
#define DIR_Left		0.0f		//サク子が左方向を向いた時に代入されるUV座標を定数化

// 構造体
static struct GAME {
	
	SPRITE_POS_TEX trans[MAX_SPRITE];		//各スプライトの「割り当て番号」「ポジション座標」「テクスチャ座標」を格納
	
	//「割り当て番号」は数が小さい方が背面へ、数が大きい方が全面に描画される。

	//「背景」
	ID3D11Buffer* pVb_Bg;  // 背景の頂点バッファの変数
	DX11TEXTURE* pTex_Bg;  // 背景のテクスチャの変数

	//「サク子」
	ID3D11Buffer* pVb_Sakuko;  // 「サク子」の頂点バッファの変数
	DX11TEXTURE* pTex_Sakuko;  // 「サク子」のテクスチャの変数


	//チョコブロック
	ID3D11Buffer* pVb_Choco_block;
	DX11TEXTURE*  pTex_Choco_block;

	//魔法のオーブン
	ID3D11Buffer* pVb_oven;  // 「サク子」の頂点バッファの変数
	DX11TEXTURE* pTex_oven;  // 「サク子」のテクスチャの変数

	//フラグポインター
	ID3D11Buffer* pVb_Flag;  // 「サク子」の頂点バッファの変数
	DX11TEXTURE* pTex_Flag;  // 「サク子」のテクスチャの変数

	float jump = 0;				//ジャンプ変数
	float jump_flag;			//ジャンプフラグ変数
	float fall_flag;			//重力フラグ変数
	
	float point_flg;

	float JamSand_flg;

} gGame;



// ゲームの初期化処理はこの関数定義の中に書く
HRESULT GAME_Initialize(HWND hWnd) {
	DX11UTIL* dx = DX11UTIL_GetPointer();
	HRESULT hr;

	// DirectX11の初期化処理を呼び出し
	DX11UTIL_Create(hWnd);

	//=============================================================================================
	//			「背景」のスプライト設定 
	//=============================================================================================

	// 頂点の配列を宣言して、それぞれの頂点の情報（座標・色・UV座標）を設定

	//「背景」のUV座標を「サク子」の初期位置付近にセット
	DX11SPRITE_VERTEX2D vx_bg[4] = {
		{ { -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f,3.0f / 4.0f} },  // 頂点０
		{ {  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,3.0f / 4.0f} },  // 頂点１
		{ { -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f,1.0f} },  // 頂点２
		{ {  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,1.0f} },  // 頂点３
	};

	//頂点バッファを作成
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Bg, NULL, sizeof(vx_bg));
	if (FAILED(hr))  return hr;
	// 「背景」のテクスチャ画像をファイルから読み込み
	gGame.pTex_Bg = DX11TEXTURE_CreateAndLoadFile("assets/haikei.jpg");
	//割り当て０番を「背景」の設定
	DX11SPRITE* pSp_Bg = DX11SPRITE_MANAGER_GetSprite(0);
	// 「背景」の頂点バッファをセット
	DX11SPRITE_SetVertexBuffer(pSp_Bg, gGame.pVb_Bg, vx_bg, 4);
	// 「背景」のテクスチャをセット
	DX11SPRITE_SetTexture(pSp_Bg, gGame.pTex_Bg);

	
	// ↓↓↓↓↓↓↓↓「さくこ」のスプライト設定↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	
	// 頂点の配列を宣言して、それぞれの頂点の情報（座標・色・UV座標）を設定
	/*
	座標を変えれば形が変わる。
	UV座標を変えれば画像の切り抜く部分を指定できる。
	*/

	DX11SPRITE_VERTEX2D vx2[4] = {
		{ { -0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // 頂点０
		{ {  0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,0} },  // 頂点１
		{ { -0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f / 4.0f} },  // 頂点２
		{ {  0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,1.0f / 4.0f} },  // 頂点３
	};

	//頂点バッファを作成
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Sakuko, NULL, sizeof(vx2));
	if (FAILED(hr))  return hr;
	//「サク子」のテクスチャ画像をファイルから読み込み
	gGame.pTex_Sakuko = DX11TEXTURE_CreateAndLoadFile("assets/sakuko.png"); 
	//割り当て1番を「サク子」の設定
	DX11SPRITE* pSp_Sakuko = DX11SPRITE_MANAGER_GetSprite(5);
	// 「サク子」に頂点バッファをセット
	DX11SPRITE_SetVertexBuffer(pSp_Sakuko, gGame.pVb_Sakuko, vx2, 4);
	// 「サク子」にテクスチャをセット
	DX11SPRITE_SetTexture(pSp_Sakuko, gGame.pTex_Sakuko);
	// 「サク子」のポジション座標
	gGame.trans[5].pos.x = -0.5f;
	gGame.trans[5].pos.y = 0.0f;

	//=============================================================================================
	//「チョコブロック」のスプライト生成
	//=============================================================================================

	DX11SPRITE_VERTEX2D vx_choco[4] = {
		{ { -0.05f , 0.05f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // 頂点０
		{ {  0.05f,  0.05f }, { 1.0f, 1.0f, 1.0f }, {1.0f,0} },  // 頂点１
		{ { -0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f } },  // 頂点２
		{ {  0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {1.0f ,1.0f} },  // 頂点３
	};


	//頂点バッファを作成
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Choco_block, NULL, sizeof(vx_choco));
	if (FAILED(hr))  return hr;
	//「チョコブロック」のテクスチャ画像をファイルから読み込み
	gGame.pTex_Choco_block = DX11TEXTURE_CreateAndLoadFile("assets/choco_block.png");
	//割り当て1番を「サク子」の設定
	DX11SPRITE* pSp_Choco_block = DX11SPRITE_MANAGER_GetSprite(2);
	// 「サク子」に頂点バッファをセット
	DX11SPRITE_SetVertexBuffer(pSp_Choco_block, gGame.pVb_Choco_block, vx_choco, 4);
	// 「サク子」にテクスチャをセット
	DX11SPRITE_SetTexture(pSp_Choco_block, gGame.pTex_Choco_block);
	// 「サク子」のポジション座標
	gGame.trans[2].pos.x = 0.0f;
	gGame.trans[2].pos.y = -0.8f;

	//=============================================================================================
	//「魔法のオーブン」のスプライト生成
	//=============================================================================================

	DX11SPRITE_VERTEX2D vx_oven[4] = {
		{ { -0.05f , 0.05f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // 頂点０
		{ {  0.05f,  0.05f }, { 1.0f, 1.0f, 1.0f }, {1.0f,0} },  // 頂点１
		{ { -0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f } },  // 頂点２
		{ {  0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {1.0f ,1.0f} },  // 頂点３
	};


	//頂点バッファを作成
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_oven, NULL, sizeof(vx_oven));
	if (FAILED(hr))  return hr;
	//「チョコブロック」のテクスチャ画像をファイルから読み込み
	gGame.pTex_oven = DX11TEXTURE_CreateAndLoadFile("assets/magic_oven.png");
	//割り当て1番を「サク子」の設定
	DX11SPRITE* pSp_oven = DX11SPRITE_MANAGER_GetSprite(3);
	// 「サク子」に頂点バッファをセット
	DX11SPRITE_SetVertexBuffer(pSp_oven, gGame.pVb_oven, vx_oven, 4);
	// 「サク子」にテクスチャをセット
	DX11SPRITE_SetTexture(pSp_oven, gGame.pTex_oven);

	gGame.trans[3].pos.x = 0.8f;
	gGame.trans[3].pos.y = -0.85f;


	//=============================================================================================
	//「フラグポインター」のスプライト生成
	//=============================================================================================

	DX11SPRITE_VERTEX2D vx_Flag[4] = {
		{ { -0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // 頂点０
		{ {  0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,0} },  // 頂点１
		{ { -0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f / 4.0f} },  // 頂点２
		{ {  0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,1.0f / 4.0f} },  // 頂点３
	};


	//頂点バッファを作成
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Flag, NULL, sizeof(vx_Flag));
	if (FAILED(hr))  return hr;
	//「チョコブロック」のテクスチャ画像をファイルから読み込み
	gGame.pTex_Flag = DX11TEXTURE_CreateAndLoadFile("assets/flg.png");
	//割り当て1番を「サク子」の設定
	DX11SPRITE* pSp_Flag = DX11SPRITE_MANAGER_GetSprite(4);
	// 「サク子」に頂点バッファをセット
	DX11SPRITE_SetVertexBuffer(pSp_Flag, gGame.pVb_Flag, vx_Flag, 4);
	// 「サク子」にテクスチャをセット
	DX11SPRITE_SetTexture(pSp_Flag, gGame.pTex_Flag);


	return hr;
}



//ゲーム更新処理
void GAME_Update() {

	gGame.fall_flag = true;		//重力は常に「ON」
	
	gGame.point_flg = false;	//フラグポインターは常に「OFF」

	//Magic_Oven();

	//もし「サク子」と「魔法のオーブン」が重なりあった時
	if (gGame.trans[5].pos.x + 0.1f >= gGame.trans[3].pos.x && gGame.trans[3].pos.x + 0.1f >= gGame.trans[5].pos.x) {
		if (gGame.trans[5].pos.y + 0.1f >= gGame.trans[3].pos.y && gGame.trans[3].pos.y + 0.1f >= gGame.trans[5].pos.y) {

			gGame.point_flg = true;

			if (GetAsyncKeyState(VK_UP) & 0x8000) {

				gGame.JamSand_flg = true;
			}

		}
	}
	else
	{
		gGame.trans[4].tex.u = 0;
		gGame.trans[4].tex.v = 1;

	}
	

	if (gGame.point_flg) {

		gGame.trans[4].tex.u = 0;
		gGame.trans[4].tex.v = 0;

		gGame.trans[4].pos.x = gGame.trans[5].pos.x ;
		gGame.trans[4].pos.y = gGame.trans[5].pos.y + 0.1f;
	}

	if (gGame.JamSand_flg) {

		if (GetAsyncKeyState(VK_UP) & 0x8000) {

			gGame.jump += 0;
		}
	}
	
	//重力の引力を「-0.003f」に設定
	if (gGame.fall_flag) {
		gGame.trans[5].pos.y -= 0.003f;
	}
	// 右矢印キーをユーザーが押していれば
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		gGame.trans[5].pos.x += 0.001f;		//「サク子」のポジションをX軸に「+0.0001f」ずつ加算
		gGame.trans[5].tex.u = DIR_Right;			//「サク子」のテクスチャ画像のUV座標が右向きの座標へ
		gGame.trans[0].tex.u += 0.0001f;		//「サク子」右に移動すれば、「背景」のUV座標も移動（カメラ処理的な）
		//「サク子」が右画面外へ出ないようにする
		if (gGame.trans[5].pos.x >= 1.0f) {
			gGame.trans[5].pos.x = 1.0f;
		}
	}
	// 左矢印キーをユーザーが押していれば
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		gGame.trans[5].pos.x -= 0.001f;		//「サク子」のポジションをX軸に「-0.0001f」ずつ加算
		gGame.trans[5].tex.u = DIR_Left;			//「サク子」のテクスチャ画像のUV座標が左向きの座標へ
		gGame.trans[0].tex.u -= 0.0001f;		//「サク子」左に移動すれば、「背景」のUV座標も移動（カメラ処理的な）
		//「サク子」が左画面外へ出ないようにする
		if (gGame.trans[5].pos.x <= -1.0f) {
			gGame.trans[5].pos.x = -1.0f;
		}
	}
	//「サク子」が地面に足をついてる時
	if (gGame.trans[5].pos.y <= DEFAURT_Y ) {
		//SPACEキーをユーザーが押していれば
		if (GetKeyboardTrigger(DIK_SPACE)) {	//トリガー　・・・　キー押した瞬間を判定する
			gGame.jump = JUMP_POWER;			//「サク子」のジャンプに馬力を加算
			gGame.jump_flag = true;				//ジャンプフラグを「ON」
		}
	}
	//ジャンプフラグが「ON」になっている時
	if (gGame.jump_flag) {
		gGame.trans[5].pos.y += gGame.jump;		//「サク子」のY軸座標にジャンプを加算
		gGame.jump += 0.00003;					//ジャンプ力を設定
	}
	//「サク子」のY軸座標が「0.0f」を超えた時（これは現段階だけの処理。後に改良予定）
	if (gGame.trans[5].pos.y > 1.0f) {
		gGame.jump_flag = false;				//ジャンプフラグを「OFF」
	}
	//「サク子」のY軸座標が地面より下にいった時、地面に戻す
	if (gGame.trans[5].pos.y < DEFAURT_Y) {
		gGame.trans[5].pos.y = DEFAURT_Y;
	}

	// 全スプライトのトランスフォームを適用する
	for (int i = 0; i < MAX_SPRITE; i++) {
		DX11SPRITE_SetTransform(DX11SPRITE_MANAGER_GetSprite(i), &gGame.trans[i]);
	}

	//GAME_Hit();
	
	
}

// ゲームの描画処理はこの関数定義の中に書く（といってもDX11UTIL_Render関数を呼び出すだけ）
void GAME_Render() {
	DX11UTIL_Render();
}

// ゲームの終了・解放処理はこの関数定義の中に書く
void GAME_Release() {
	if (gGame.pVb_Sakuko != NULL) {
		gGame.pVb_Sakuko->Release();
		gGame.pVb_Sakuko = NULL;
	}

	if (gGame.pVb_Bg != NULL) {
		gGame.pVb_Bg->Release();
		gGame.pVb_Bg = NULL;
	}
	if (gGame.pVb_Choco_block != NULL) {
		gGame.pVb_Choco_block->Release();
		gGame.pVb_Choco_block = NULL;
	}
	if (gGame.pVb_oven != NULL) {
		gGame.pVb_oven->Release();
		gGame.pVb_oven = NULL;
	}

	DX11TEXTURE_Release(gGame.pTex_Sakuko);
	DX11TEXTURE_Release(gGame.pTex_Bg);
	DX11TEXTURE_Release(gGame.pTex_Choco_block);
	DX11TEXTURE_Release(gGame.pTex_oven);
	
	DX11UTIL_Release();
}

/*void GAME_Hit() {

	if (gGame.trans[1].pos.x + 0.1f > gGame.trans[2].pos.x && gGame.trans[2].pos.x + 0.1f > gGame.trans[1].pos.x) {
		if (gGame.trans[1].pos.y + 0.1f > gGame.trans[2].pos.y && gGame.trans[2].pos.y + 0.1f > gGame.trans[1].pos.y) {

			gGame.jump_flag = false;
			//gGame.trans[1].pos.y = gGame.choco_floor_y;

		}
	}
}

/*int CheckHit()
{
	int L1 = gGame.trans[1].pos.x;      // 左
	int R1 = gGame.trans[1].pos.x + 1.0f; // 右(左+横幅)
	int L2 = gGame.trans[2].pos.x;      // 左
	int R2 = gGame.trans[2].pos.x + 1.0f; // 右(左+横幅)

	if (R1 < L2) return 0;
	if (R2 < L1) return 0;


	// それ以外の場合は当たっている
	return 1;
	gGame.fall_flag = false;
}*/
