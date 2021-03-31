#pragma once
#include "uge.h"
using namespace uge;
namespace game {

	struct MapHeader {
		short width;	//��ͼ��
		short height;	//��ͼ��
		char title[16];
		int updatetime[2];
		char Reserved[24];
	};

	struct MapInfo {
		unsigned short wBkImg;	//��һ��
		unsigned short wMidImg; //�ڶ���
		unsigned short wFrImg;	//������
		byte btDoorIndex;
		byte btDoorOffset;
		byte btAniFrame;
		byte btAniTick;
		byte btArea;
		byte btLight;
	};

	class Map {
		UGE* pUge;								//�������
		MapHeader mapHead;						//mapͷ���ṹ��
		MapInfo* mapInfo;						//map���ݽṹ��

		int bWidth = 48;						//�������ؿ��
		int bHeight = 32;						//�������ظ߶�
		int offsetX = -7;						//��ͼ�������ĵ�Xƫ��
		int offsetY = 44;						//��ͼ�������ĵ�Yƫ��
		static const int mapSizeW = 25;			//��ͼ���Ƴߴ�21x21
		static const int mapSizeH = 25;			//��ͼ���Ƴߴ�21x21
		ugeImage uiTiles[mapSizeW][mapSizeH];		//���ש
		ugeImage uiSmTiles[mapSizeW][mapSizeH];	//С��ש
		ugeImage uiObjects[mapSizeW][mapSizeH*2];	//�������
	public:
		Map(UGE* pUge);
		void Load(const char* path);
		void LoadMap(int x, int y);
		void Show(bool fillmode);
		void Tiles(int sx, int sy, int tx, int ty, int sort);
		void SmTiles(int sx, int sy,int sort);
		void Objects(int sx, int sy, int sort);
	};
}