#ifndef MENU_CONFIG_H
#define MENU_CONFIG_H

#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

#define   NR(x)   (sizeof(x)/sizeof(x[0]+0))
#define CONFIG_FILE "..//config.txt"
enum
{
	UP = 87,
	UP_ = 119,
	DOWN = 83,
	DOWN_ = 115,
	LEFT = 65,
	LEFT_ = 97,
	RIGHT = 68,
	RIGHT_ = 100,
	//ENTER = 10,
	ENTER = 13,
	ESC = 27,
	K_= 75,
	K__=107,
	T_=84,
	T__=116,
};

//菜单显示status
enum
{
    MENU_OUT=0,
    MENU1 =1,
    MENU2_1=2,
    MENU2_2=3,
};

//菜单选中栏目
enum
{
    COLUMN1=0,
    COLUMN2 =1,
    COLUMN3=2,
    COLUMN4=3,
    COLUMN5=4,
    COLUMN6=5,
    COLUMN7=6,
    COLUMN8=7,
    COLUMN9=8,
    COLUMN10=9,
};

static char *menu2_1[200] =
{
	"＊ 采集中...                      ＊",
	"＊ 返回                           ＊",
};
static char *menu1[200] =
{
	"＊ 1  启动采集              ＊",
	"＊ 2  参数配置              ＊",
	"＊ 3  退出                  ＊",
};
static char *menu2_2[200] =
{
	"＊ 1  frame_downsampling_percent            ＊",
	"＊ 2  continues_frame_goal_cnt              ＊",
	"＊ 3  返回                                  ＊",
};

class Config {
private:
	string config_file;
	FILE *read_setup;
	bool _str_cmp(char* a, char *b);
public:
	Config(string cf);
	~Config();
	bool openFile(char *flags);
	void closeFile();
	void clearBlank();
	float get_param(char *paramName);
	void set_param(char *paramName, float val);
	void readConfig(int &len,char **param,float *value);
};

class Menu {
private:
	char **menu ;
	int *index;
    int menu_size;
	int paramLength;//参数长度
	float paramValueList[10];//参数菜单
	int ret;
	int index1;//一级菜单
	int index2_1;//二级菜单1_1
	int paramIndex;//二级菜单
	bool isOut;
public:

	Menu() {
		index1 = 0;
		index2_1=0;
		paramIndex = 0;
		isOut = false;
	};
	~Menu() {
		cout<<endl;
	};
	//获取用户输入的接口 
	int  get_userinput(int *index, int size);
	void run(int &status);
	int scanKeyboard();
	void show(char **menu, int size, int index);

};
#endif