#include "ugeresource.h"
#include "ugehelp.h"

namespace uge {

	UgeResource::UgeResource()
	{
	}

	UgeResource::~UgeResource()
	{

		if (!_wzlMap.empty())
		{
			for (UWzlMap::iterator it = _wzlMap.begin(); it != _wzlMap.end(); it++)
			{
				delete it->second;
			}
		}
	}

	WzlBmp* UgeResource::GetWzlCache(const char* path)
	{
		//��ȡ����
		std::string name = get_file_name(path);

		//����wzl��Դ
		UWzlMap::const_iterator curWzlMap = _wzlMap.find(name);
		if (curWzlMap == _wzlMap.end())
		{
			//������Դ���ڴ�
			WzlBmp* bmp = new WzlBmp(path);

			_wzlMap.insert(std::pair<std::string, WzlBmp*>(name, bmp));
			curWzlMap = _wzlMap.find(name);
		}

		return curWzlMap->second;
	}

	bool UgeResource::ReleaseWzlCache(const char*path,int sort)
	{
		//��ȡ����
		std::string name = get_file_name(path);

		//Ѱ��Ԫ��
		UWzlMap::const_iterator t_CurWzl = _wzlMap.find(name);
		if (t_CurWzl == _wzlMap.end())
		{
			return false;
		}

		//�ҵ�
		bool erase = false;
		if ( ! _wzlMap.at(name)->ReleaseTexture(sort,&erase)) {
			return false;
		}

		//�Ƿ���Ҫ�Ƴ���Դ
		if (erase)
		{
			//ɾ������
			_wzlMap.erase(name);
		}
		
		return true;
	}
}