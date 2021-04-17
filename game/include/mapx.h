#pragma once
#include <uge.h>
#include <wzlcache.h>
#include <vector>
#include <algorithm>

using namespace uge;
using namespace wzl;
namespace game {

	struct MapHeaderX {
		short width;	//��ͼ��
		short height;	//��ͼ��
		char title[16];
		int updatetime[2];
		char Reserved[24];
	};

	struct MapInfo36 {
		unsigned short wBkImg;	//��һ��
		unsigned short wMidImg; //�ڶ���
		unsigned short wFrImg;	//������
		byte btDoorIndex;
		byte btDoorOffset;
		byte btAniFrame;
		byte btAniTick;
		byte btArea;
		byte btLight;			//12�ֽ�
		byte btBackIndex;
		byte btMidIndex;		//14�ֽ�
		unsigned short TileAnimationImage;//15-16�ֽ�
		byte reserved1[5];//����17-21�ֽ�
		byte TileAnimationOffset;	//22�ֽ�
		unsigned short TileAnimationFrame;//23-24�ֽ�
		byte reserved2[12];//����25-36�ֽ�
	};

	class MapX {

		WzlCache* pWzlCache;					//wzl�������
		UGE* pUge;								//�������
		MapHeaderX mapHead;						//mapͷ���ṹ��
		MapInfo36* mapInfo;						//map���ݽṹ��

		int bWidth = 48;						//�������ؿ��
		int bHeight = 32;						//�������ظ߶�
		int offsetX = -7;						//��ͼ�������ĵ�Xƫ��
		int offsetY = 44;						//��ͼ�������ĵ�Yƫ��
		static const int mapSizeW = 41;			//��ͼ���Ƴߴ�21x21
		static const int mapSizeH = 41;			//��ͼ���Ƴߴ�21x21

		std::vector<wzl::ugeImage> vUiTiles;		//���ש
		std::vector<wzl::ugeImage> vUiSmTiles;		//С��ש
		std::vector<wzl::ugeImage> vUiObjects;		//�������
		std::vector<wzl::ugeAnimation> vBtLight;//��ͼ�ƶ���

		int screen_width;
		int screen_height;
		int centerX;
		int centerY;
	public:
		MapX(UGE* pUge);
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