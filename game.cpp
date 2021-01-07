//�C���N���[�h
#include "game.h"
#include "dx11sprite.h"
#include "input.h"

//�萔��`
#define MAX_SPRITE  6			// �Q�[���Ŏg���X�v���C�g�̐�(�Ƃ肠�����R�ɐݒ�)
#define JUMP_POWER  0.005f		//�W�����v�̔n�͂��u0.003f�v�ɐݒ�
#define BLOCK_SPRITE 10
#define DEFAURT_Y -0.75f
#define DIR_Right		0.4f		//�T�N�q���E���������������ɑ�������UV���W��萔��
#define DIR_Left		0.0f		//�T�N�q�������������������ɑ�������UV���W��萔��

// �\����
static struct GAME {
	
	SPRITE_POS_TEX trans[MAX_SPRITE];		//�e�X�v���C�g�́u���蓖�Ĕԍ��v�u�|�W�V�������W�v�u�e�N�X�`�����W�v���i�[
	
	//�u���蓖�Ĕԍ��v�͐��������������w�ʂցA�����傫�������S�ʂɕ`�悳���B

	//�u�w�i�v
	ID3D11Buffer* pVb_Bg;  // �w�i�̒��_�o�b�t�@�̕ϐ�
	DX11TEXTURE* pTex_Bg;  // �w�i�̃e�N�X�`���̕ϐ�

	//�u�T�N�q�v
	ID3D11Buffer* pVb_Sakuko;  // �u�T�N�q�v�̒��_�o�b�t�@�̕ϐ�
	DX11TEXTURE* pTex_Sakuko;  // �u�T�N�q�v�̃e�N�X�`���̕ϐ�


	//�`���R�u���b�N
	ID3D11Buffer* pVb_Choco_block;
	DX11TEXTURE*  pTex_Choco_block;

	//���@�̃I�[�u��
	ID3D11Buffer* pVb_oven;  // �u�T�N�q�v�̒��_�o�b�t�@�̕ϐ�
	DX11TEXTURE* pTex_oven;  // �u�T�N�q�v�̃e�N�X�`���̕ϐ�

	//�t���O�|�C���^�[
	ID3D11Buffer* pVb_Flag;  // �u�T�N�q�v�̒��_�o�b�t�@�̕ϐ�
	DX11TEXTURE* pTex_Flag;  // �u�T�N�q�v�̃e�N�X�`���̕ϐ�

	float jump = 0;				//�W�����v�ϐ�
	float jump_flag;			//�W�����v�t���O�ϐ�
	float fall_flag;			//�d�̓t���O�ϐ�
	
	float point_flg;

	float JamSand_flg;

} gGame;



// �Q�[���̏����������͂��̊֐���`�̒��ɏ���
HRESULT GAME_Initialize(HWND hWnd) {
	DX11UTIL* dx = DX11UTIL_GetPointer();
	HRESULT hr;

	// DirectX11�̏������������Ăяo��
	DX11UTIL_Create(hWnd);

	//=============================================================================================
	//			�u�w�i�v�̃X�v���C�g�ݒ� 
	//=============================================================================================

	// ���_�̔z���錾���āA���ꂼ��̒��_�̏��i���W�E�F�EUV���W�j��ݒ�

	//�u�w�i�v��UV���W���u�T�N�q�v�̏����ʒu�t�߂ɃZ�b�g
	DX11SPRITE_VERTEX2D vx_bg[4] = {
		{ { -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f,3.0f / 4.0f} },  // ���_�O
		{ {  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,3.0f / 4.0f} },  // ���_�P
		{ { -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f,1.0f} },  // ���_�Q
		{ {  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,1.0f} },  // ���_�R
	};

	//���_�o�b�t�@���쐬
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Bg, NULL, sizeof(vx_bg));
	if (FAILED(hr))  return hr;
	// �u�w�i�v�̃e�N�X�`���摜���t�@�C������ǂݍ���
	gGame.pTex_Bg = DX11TEXTURE_CreateAndLoadFile("assets/haikei.jpg");
	//���蓖�ĂO�Ԃ��u�w�i�v�̐ݒ�
	DX11SPRITE* pSp_Bg = DX11SPRITE_MANAGER_GetSprite(0);
	// �u�w�i�v�̒��_�o�b�t�@���Z�b�g
	DX11SPRITE_SetVertexBuffer(pSp_Bg, gGame.pVb_Bg, vx_bg, 4);
	// �u�w�i�v�̃e�N�X�`�����Z�b�g
	DX11SPRITE_SetTexture(pSp_Bg, gGame.pTex_Bg);

	
	// �����������������u�������v�̃X�v���C�g�ݒ聫����������������������������������������������������������������������
	
	// ���_�̔z���錾���āA���ꂼ��̒��_�̏��i���W�E�F�EUV���W�j��ݒ�
	/*
	���W��ς���Ό`���ς��B
	UV���W��ς���Ή摜�̐؂蔲���������w��ł���B
	*/

	DX11SPRITE_VERTEX2D vx2[4] = {
		{ { -0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // ���_�O
		{ {  0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,0} },  // ���_�P
		{ { -0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f / 4.0f} },  // ���_�Q
		{ {  0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,1.0f / 4.0f} },  // ���_�R
	};

	//���_�o�b�t�@���쐬
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Sakuko, NULL, sizeof(vx2));
	if (FAILED(hr))  return hr;
	//�u�T�N�q�v�̃e�N�X�`���摜���t�@�C������ǂݍ���
	gGame.pTex_Sakuko = DX11TEXTURE_CreateAndLoadFile("assets/sakuko.png"); 
	//���蓖��1�Ԃ��u�T�N�q�v�̐ݒ�
	DX11SPRITE* pSp_Sakuko = DX11SPRITE_MANAGER_GetSprite(5);
	// �u�T�N�q�v�ɒ��_�o�b�t�@���Z�b�g
	DX11SPRITE_SetVertexBuffer(pSp_Sakuko, gGame.pVb_Sakuko, vx2, 4);
	// �u�T�N�q�v�Ƀe�N�X�`�����Z�b�g
	DX11SPRITE_SetTexture(pSp_Sakuko, gGame.pTex_Sakuko);
	// �u�T�N�q�v�̃|�W�V�������W
	gGame.trans[5].pos.x = -0.5f;
	gGame.trans[5].pos.y = 0.0f;

	//=============================================================================================
	//�u�`���R�u���b�N�v�̃X�v���C�g����
	//=============================================================================================

	DX11SPRITE_VERTEX2D vx_choco[4] = {
		{ { -0.05f , 0.05f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // ���_�O
		{ {  0.05f,  0.05f }, { 1.0f, 1.0f, 1.0f }, {1.0f,0} },  // ���_�P
		{ { -0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f } },  // ���_�Q
		{ {  0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {1.0f ,1.0f} },  // ���_�R
	};


	//���_�o�b�t�@���쐬
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Choco_block, NULL, sizeof(vx_choco));
	if (FAILED(hr))  return hr;
	//�u�`���R�u���b�N�v�̃e�N�X�`���摜���t�@�C������ǂݍ���
	gGame.pTex_Choco_block = DX11TEXTURE_CreateAndLoadFile("assets/choco_block.png");
	//���蓖��1�Ԃ��u�T�N�q�v�̐ݒ�
	DX11SPRITE* pSp_Choco_block = DX11SPRITE_MANAGER_GetSprite(2);
	// �u�T�N�q�v�ɒ��_�o�b�t�@���Z�b�g
	DX11SPRITE_SetVertexBuffer(pSp_Choco_block, gGame.pVb_Choco_block, vx_choco, 4);
	// �u�T�N�q�v�Ƀe�N�X�`�����Z�b�g
	DX11SPRITE_SetTexture(pSp_Choco_block, gGame.pTex_Choco_block);
	// �u�T�N�q�v�̃|�W�V�������W
	gGame.trans[2].pos.x = 0.0f;
	gGame.trans[2].pos.y = -0.8f;

	//=============================================================================================
	//�u���@�̃I�[�u���v�̃X�v���C�g����
	//=============================================================================================

	DX11SPRITE_VERTEX2D vx_oven[4] = {
		{ { -0.05f , 0.05f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // ���_�O
		{ {  0.05f,  0.05f }, { 1.0f, 1.0f, 1.0f }, {1.0f,0} },  // ���_�P
		{ { -0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f } },  // ���_�Q
		{ {  0.05f, -0.15f }, { 1.0f, 1.0f, 1.0f }, {1.0f ,1.0f} },  // ���_�R
	};


	//���_�o�b�t�@���쐬
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_oven, NULL, sizeof(vx_oven));
	if (FAILED(hr))  return hr;
	//�u�`���R�u���b�N�v�̃e�N�X�`���摜���t�@�C������ǂݍ���
	gGame.pTex_oven = DX11TEXTURE_CreateAndLoadFile("assets/magic_oven.png");
	//���蓖��1�Ԃ��u�T�N�q�v�̐ݒ�
	DX11SPRITE* pSp_oven = DX11SPRITE_MANAGER_GetSprite(3);
	// �u�T�N�q�v�ɒ��_�o�b�t�@���Z�b�g
	DX11SPRITE_SetVertexBuffer(pSp_oven, gGame.pVb_oven, vx_oven, 4);
	// �u�T�N�q�v�Ƀe�N�X�`�����Z�b�g
	DX11SPRITE_SetTexture(pSp_oven, gGame.pTex_oven);

	gGame.trans[3].pos.x = 0.8f;
	gGame.trans[3].pos.y = -0.85f;


	//=============================================================================================
	//�u�t���O�|�C���^�[�v�̃X�v���C�g����
	//=============================================================================================

	DX11SPRITE_VERTEX2D vx_Flag[4] = {
		{ { -0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {0,0} },  // ���_�O
		{ {  0.1f,  0.1f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,0} },  // ���_�P
		{ { -0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {0,1.0f / 4.0f} },  // ���_�Q
		{ {  0.1f, -0.25f }, { 1.0f, 1.0f, 1.0f }, {1.0f / 3.0f,1.0f / 4.0f} },  // ���_�R
	};


	//���_�o�b�t�@���쐬
	hr = DX11UTIL_CreateVertexBuffer(&gGame.pVb_Flag, NULL, sizeof(vx_Flag));
	if (FAILED(hr))  return hr;
	//�u�`���R�u���b�N�v�̃e�N�X�`���摜���t�@�C������ǂݍ���
	gGame.pTex_Flag = DX11TEXTURE_CreateAndLoadFile("assets/flg.png");
	//���蓖��1�Ԃ��u�T�N�q�v�̐ݒ�
	DX11SPRITE* pSp_Flag = DX11SPRITE_MANAGER_GetSprite(4);
	// �u�T�N�q�v�ɒ��_�o�b�t�@���Z�b�g
	DX11SPRITE_SetVertexBuffer(pSp_Flag, gGame.pVb_Flag, vx_Flag, 4);
	// �u�T�N�q�v�Ƀe�N�X�`�����Z�b�g
	DX11SPRITE_SetTexture(pSp_Flag, gGame.pTex_Flag);


	return hr;
}



//�Q�[���X�V����
void GAME_Update() {

	gGame.fall_flag = true;		//�d�͂͏�ɁuON�v
	
	gGame.point_flg = false;	//�t���O�|�C���^�[�͏�ɁuOFF�v

	//Magic_Oven();

	//�����u�T�N�q�v�Ɓu���@�̃I�[�u���v���d�Ȃ肠������
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
	
	//�d�͂̈��͂��u-0.003f�v�ɐݒ�
	if (gGame.fall_flag) {
		gGame.trans[5].pos.y -= 0.003f;
	}
	// �E���L�[�����[�U�[�������Ă����
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		gGame.trans[5].pos.x += 0.001f;		//�u�T�N�q�v�̃|�W�V������X���Ɂu+0.0001f�v�����Z
		gGame.trans[5].tex.u = DIR_Right;			//�u�T�N�q�v�̃e�N�X�`���摜��UV���W���E�����̍��W��
		gGame.trans[0].tex.u += 0.0001f;		//�u�T�N�q�v�E�Ɉړ�����΁A�u�w�i�v��UV���W���ړ��i�J���������I�ȁj
		//�u�T�N�q�v���E��ʊO�֏o�Ȃ��悤�ɂ���
		if (gGame.trans[5].pos.x >= 1.0f) {
			gGame.trans[5].pos.x = 1.0f;
		}
	}
	// �����L�[�����[�U�[�������Ă����
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		gGame.trans[5].pos.x -= 0.001f;		//�u�T�N�q�v�̃|�W�V������X���Ɂu-0.0001f�v�����Z
		gGame.trans[5].tex.u = DIR_Left;			//�u�T�N�q�v�̃e�N�X�`���摜��UV���W���������̍��W��
		gGame.trans[0].tex.u -= 0.0001f;		//�u�T�N�q�v���Ɉړ�����΁A�u�w�i�v��UV���W���ړ��i�J���������I�ȁj
		//�u�T�N�q�v������ʊO�֏o�Ȃ��悤�ɂ���
		if (gGame.trans[5].pos.x <= -1.0f) {
			gGame.trans[5].pos.x = -1.0f;
		}
	}
	//�u�T�N�q�v���n�ʂɑ������Ă鎞
	if (gGame.trans[5].pos.y <= DEFAURT_Y ) {
		//SPACE�L�[�����[�U�[�������Ă����
		if (GetKeyboardTrigger(DIK_SPACE)) {	//�g���K�[�@�E�E�E�@�L�[�������u�Ԃ𔻒肷��
			gGame.jump = JUMP_POWER;			//�u�T�N�q�v�̃W�����v�ɔn�͂����Z
			gGame.jump_flag = true;				//�W�����v�t���O���uON�v
		}
	}
	//�W�����v�t���O���uON�v�ɂȂ��Ă��鎞
	if (gGame.jump_flag) {
		gGame.trans[5].pos.y += gGame.jump;		//�u�T�N�q�v��Y�����W�ɃW�����v�����Z
		gGame.jump += 0.00003;					//�W�����v�͂�ݒ�
	}
	//�u�T�N�q�v��Y�����W���u0.0f�v�𒴂������i����͌��i�K�����̏����B��ɉ��Ǘ\��j
	if (gGame.trans[5].pos.y > 1.0f) {
		gGame.jump_flag = false;				//�W�����v�t���O���uOFF�v
	}
	//�u�T�N�q�v��Y�����W���n�ʂ�艺�ɂ��������A�n�ʂɖ߂�
	if (gGame.trans[5].pos.y < DEFAURT_Y) {
		gGame.trans[5].pos.y = DEFAURT_Y;
	}

	// �S�X�v���C�g�̃g�����X�t�H�[����K�p����
	for (int i = 0; i < MAX_SPRITE; i++) {
		DX11SPRITE_SetTransform(DX11SPRITE_MANAGER_GetSprite(i), &gGame.trans[i]);
	}

	//GAME_Hit();
	
	
}

// �Q�[���̕`�揈���͂��̊֐���`�̒��ɏ����i�Ƃ����Ă�DX11UTIL_Render�֐����Ăяo�������j
void GAME_Render() {
	DX11UTIL_Render();
}

// �Q�[���̏I���E��������͂��̊֐���`�̒��ɏ���
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
	int L1 = gGame.trans[1].pos.x;      // ��
	int R1 = gGame.trans[1].pos.x + 1.0f; // �E(��+����)
	int L2 = gGame.trans[2].pos.x;      // ��
	int R2 = gGame.trans[2].pos.x + 1.0f; // �E(��+����)

	if (R1 < L2) return 0;
	if (R2 < L1) return 0;


	// ����ȊO�̏ꍇ�͓������Ă���
	return 1;
	gGame.fall_flag = false;
}*/
