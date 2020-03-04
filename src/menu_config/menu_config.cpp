#include "menu_config.h"
#include "../funs/funs.h"
#include <termio.h>
using namespace std;

Config::Config(string cf)
{
	config_file = cf;
	openFile("r");
};

Config::~Config() {
	closeFile();
};

bool Config::openFile(char *flags)
{
	if ((read_setup = fopen(config_file.c_str(), flags)) == NULL) {
		puts("Fail to open config.txt!");
		return 0;
	}
	return 1;
}

void Config::closeFile()
{
	fclose(read_setup);
}

bool Config::_str_cmp(char* a, char *b)
{
	int sum = 0;
	for (int i = 0; b[i] != '\0'; i++)
		sum++;
	char tmp[200] = { "" };
	strncpy(tmp, a + 0, sum);
	for (int i = 0; a[i] != '\0'; i++)
	{
		if (a[i] == '\n')
			a[i] = (char)NULL;
	}
	return !strcmp(tmp, b);
}

void Config::clearBlank()
{
	string config_contxt;
	fseek(read_setup, 0, SEEK_SET);
	char str[200];
	while (fgets(str, 200, read_setup) != NULL) {
		if (str[0] != 10)
			config_contxt += str;
	}
	closeFile();
	openFile("w+");
	fprintf(read_setup, "%s", config_contxt.c_str());
	closeFile();
}

void Config::readConfig(int &len, char **param, float *value)
{

	len = 0;
	char str_org[200];
	char str[200];
	fseek(read_setup, 0, SEEK_SET);
	while (fgets(str_org, 200, read_setup) != NULL) {
		if (!_str_cmp(str_org, (char *)" "))
		{
			memcpy(str, str_org, 200);
			const char * split = " ";
			char *p = strtok(str, split);
			p = strtok(NULL, split);
			if (p != NULL)
			{
				memcpy(param[len], str_org, 200);
				sscanf(p, "%f", &value[len]);
				len++;
			}
		}
	}
}

float Config::get_param(char *paramName)
{
	float per = 0;
	char str[200];
	fseek(read_setup, 0, SEEK_SET);
	while (fgets(str, 200, read_setup) != NULL) {
		if (_str_cmp(str, paramName))
		{
			const char * split = " ";
			char *p = strtok(str, split);
			p = strtok(NULL, split);
			sscanf(p, "%f", &per);
			break;
		}

	}
	return per;
}

void Config::set_param(char *paramName, float val)
{
	float per = 0;
	char str[200];
	fseek(read_setup, 0, SEEK_SET);
	long file_pos_up = 0;
	long file_pos_down = 0;
	while (fgets(str, 200, read_setup) != NULL) {
		if (_str_cmp(str, paramName))
		{
			file_pos_down = ftell(read_setup);
			fseek(read_setup, -10L, SEEK_CUR);
			while (1)
			{
				char ch = fgetc(read_setup);
				if (ch==32)
				{
					file_pos_up = ftell(read_setup);
					break;
				}
			}
			break;
		}

	}
	string config_contxt;
	fseek(read_setup, 0L, SEEK_SET);
	char str_t = 0;
	while (file_pos_up > ftell(read_setup)) {
		str_t= fgetc(read_setup);
		config_contxt += str_t;
		
	}
	char buffer_max[20];
	sprintf(buffer_max, "%0.1f", val);
	config_contxt += buffer_max;
	if(str_t!=10)
		config_contxt += "\n";
	fseek(read_setup, file_pos_down, SEEK_SET);

	while (!feof(read_setup)) {
		str_t = fgetc(read_setup);
		if (str_t == NULL|| str_t<0) break;
		config_contxt += str_t;

	}
	closeFile();
	openFile("w+");
	fseek(read_setup, 0L, SEEK_SET);
	fprintf(read_setup, "%s", config_contxt.c_str());
	closeFile();
}

int  Menu::get_userinput(int *index, int size)
{
    int ch;
    ch =scanKeyboard();
    string tmp="\033[";
    tmp+=int2string(15);
    tmp+=";10H";
    tmp+="\e[0;31m";
    tmp+="%d";
    printf(tmp.c_str(), ch);
    switch (ch)
    {
        //上 
        //如果选择上，那么光标向上移动 
    case UP_:
    case UP: if (*index > 0)  *index -= 1;  break;
        //下 
        //如果选择下，那么光标向下移动 
    case DOWN_:
    case DOWN:if (*index < size - 1)  *index += 1;  break;
        //左 
    case LEFT_: return LEFT;
    case LEFT: return LEFT;
        //右 
    case RIGHT_:return ENTER;
    case RIGHT:return ENTER;
        //回车 
    //case ENTER_: return ENTER;
    case ENTER: return ENTER;
        //ESC
    case ESC: return ESC;
    case K_:
    case K__:return K_;
    case T_:
    case T__:return T_;
    }
    return 0;
}

int Menu::scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    
    in = getchar();
    
    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}

void Menu::show(char **menu, int size, int index)
{
    int i;
    system("clear");

    for (i = 0; i < size; i++)
    {
        if (i == index)
        {
            //红色 
            string tmp="\033[";
            tmp+=int2string(i+5);
            tmp+=";10H";
            tmp+="\e[0;31m";
            tmp+="%s";
            printf(tmp.c_str(), menu[i]);
        }
        //否则显示为白色 
        else
        {
            //白色 
            string tmp="\033[";
            tmp+=int2string(i+5);
            tmp+=";10H";
            tmp+="\e[1;37m";
            tmp+="%s";
            printf(tmp.c_str(), menu[i]);
        }
    }
}

void Menu::run(int &status)
{
    isOut=false;
    status=MENU1;
    const char * split = " ";
    char **paramList;
    paramList = (char **)malloc(sizeof(char *) * 10);
    for (int i = 0; i < 10; i++)
        paramList[i] = (char *)malloc(sizeof(char) * 200);

    Config cfg(CONFIG_FILE);
    cfg.readConfig(paramLength, paramList, paramValueList);
    paramList[paramLength++] = "返回";

    while(!isOut)
    {
        	switch (status)
			{
			case MENU_OUT:
				isOut = true;
				break; //退出
			case MENU1:
				menu_size = 3;
				menu = menu1;
				index = &index1;
				break;  	//主界面
			case MENU2_1://采集中
				menu_size = 2;
				menu = menu2_1;
				index = &index2_1;
				break;
			case MENU2_2:
				menu_size = paramLength;
				menu = paramList;
				index = &paramIndex;
				break;  	//参数配置界面
			}

        show(menu, menu_size, *index);
        ret = get_userinput(index, menu_size);
        //按键处理
        if (ret == ESC)
            break;
        else if (ret == LEFT)
        {
            switch (status)
            {
            case MENU1://主界面
                isOut = true;
                break;
            case MENU2_1:
                    status = 1;
                    break;
            case MENU2_2://参数配置界面
                status = 1;
                break;
            }
        }
        else if (ret == ENTER)
        {
            switch (status)
            {
            case MENU1://主界面
                switch (index1)
                {
                case COLUMN1:   status = MENU2_1; isOut = true;    break;
                case COLUMN2:	status = MENU2_2;	     break; 
                case COLUMN3:   isOut = true;        break;    //返回
                }
                break; 
            case MENU2_1://采集中
                switch (index2_1)
                {
                case 0:                                         break; 
                case 1: 
                    status = MENU1;   
                    break;    //返回
                }
                break; 
            case MENU2_2://参数配置界面
                if (paramIndex == paramLength - 1)
                {
                    status = MENU1;
                    break;
                }
                char paramTmp[200];
                memcpy(paramTmp, paramList[paramIndex],200);
                strtok(paramTmp, split);
                float paramValue= cfg.get_param(paramTmp);

                float new_value=0;
                cout << "\n\n\n请输入新的参数:  ";
                scanf("%f", &new_value);
                cfg.set_param(paramTmp, new_value);
                cfg.openFile("r");
                cfg.readConfig(paramLength, paramList, paramValueList);
                paramList[paramLength++] = "返回";
                break;
            }

        }
    }

}