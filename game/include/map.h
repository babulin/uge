#pragma once
#include <uge.h>
#include <wzlcache.h>

using namespace uge;
using namespace wzl;
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
		byte btLight;			//12�ֽ�
		//byte btBackIndex;
		//byte btMidIndex;		//14�ֽ�
		//unsigned short TileAnimationImage;//15-16�ֽ�
		//byte reserved1[5];//����17-21�ֽ�
		//byte TileAnimationOffset;	//22�ֽ�
		//unsigned short TileAnimationFrame;//23-24�ֽ�
		//byte reserved2[12];//����25-36�ֽ�
	};

	class Map {

		WzlCache* pWzlCache;					//wzl�������
		UGE* pUge;								//�������
		MapHeader mapHead;						//mapͷ���ṹ��
		MapInfo* mapInfo;						//map���ݽṹ��

		int bWidth = 48;						//�������ؿ��
		int bHeight = 32;						//�������ظ߶�
		int offsetX = -7;						//��ͼ�������ĵ�Xƫ��
		int offsetY = 44;						//��ͼ�������ĵ�Yƫ��
		static const int mapSizeW = 41;			//��ͼ���Ƴߴ�21x21
		static const int mapSizeH = 41;			//��ͼ���Ƴߴ�21x21
		wzl::ugeImage uiTiles[mapSizeW][mapSizeH];		//���ש
		wzl::ugeImage uiSmTiles[mapSizeW][mapSizeH];	//С��ש
		wzl::ugeImage uiObjects[mapSizeW][mapSizeH*2];	//�������

		int screen_width;
		int screen_height;
		int centerX;
		int centerY;
	public:
		Map(UGE* pUge);
		void Update();
		void Show(bool fillmode);
	private:
		float _GetWidthOffset(int val, int val2 = 0);
		float _GetHeightOffset(int val,int val2 = 0);
		void _Load(const char* path);
		void _LoadMap(int x, int y);
		void _Tiles(int sx, int sy, int tx, int ty, int sort);
		void _SmTiles(int sx, int sy,int sort);
		void _Objects(int sx, int sy, int sort);
	};
}