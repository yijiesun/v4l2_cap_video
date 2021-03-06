#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <sys/time.h>
#include <stdio.h>
#include <termio.h>
#include "screen/screen.h"
#include "v4l2/v4l2.h" 
#include <linux/fb.h>
#include <linux/videodev2.h>
#include "menu_config/menu_config.h"
#include "funs/funs.h"
#define DEV_VIDEO      "/dev/video10"
#define FBDEVICE		"/dev/fb0"
#define IMAGEWIDTH      640
#define IMAGEHEIGHT     480

V4L2 v4l2_;
SCREEN screen_;
unsigned int screen_pos_x,screen_pos_y;
cv::Mat rgb;
bool quit;
bool isStartRecording;
pthread_mutex_t mutex_;
string log_fps,log_w,log_h,log_cap_num,externShowLog;
int video_width;
int video_height;
int fps;
int cap_num;
void *v4l2_thread(void *threadarg);
void *key_thread(void *threadarg);

int main(int argc, char *argv[])
{
    bool screen_v4l2_init=false;
    isStartRecording=false;
    int MenuStatus=MENU2_1;
    Menu menu;
    
    pthread_mutex_init(&mutex_, NULL);

    menu.run(MenuStatus);
    while(MenuStatus==MENU2_1)
    {
        Config cfg(CONFIG_FILE);
        video_width=static_cast<int>(cfg.get_param("video_width"));
        video_height=static_cast<int>(cfg.get_param("video_height"));
        fps=static_cast<int>(cfg.get_param("fps"));
        cap_num=static_cast<int>(cfg.get_param("cap_num"));
        screen_pos_x=static_cast<unsigned int>(cfg.get_param("screen_pos_x"));
        screen_pos_y=static_cast<unsigned int>(cfg.get_param("screen_pos_y"));

        log_fps="fps:";
        log_fps+=int2string(fps);
        log_w="video_width: ";
        log_w+=int2string(video_width);
        log_h="video_height:";
        log_h+=int2string(video_height);
        log_cap_num="/dev/video";
        log_cap_num+=int2string(cap_num);
        externShowLog=""; 

        if(!screen_v4l2_init)
        {
            screen_v4l2_init=true;
            screen_.init((char *)FBDEVICE,video_width,video_height);
            rgb.create(video_height,video_width,CV_8UC3);
            v4l2_.init(log_cap_num.c_str(),video_width,video_height);
            v4l2_.open_device();
            v4l2_.init_device();
            v4l2_.start_capturing();
        }

        quit = false;
        pthread_t threads_v4l2;
        pthread_create(&threads_v4l2, NULL, v4l2_thread, NULL);
        pthread_t threads_key;
        pthread_create(&threads_key, NULL, key_thread, NULL);
        pthread_join(threads_v4l2,NULL);
        pthread_join(threads_key,NULL);
        menu.run(MenuStatus);

    }
        screen_.uninit();
        v4l2_.stop_capturing();
        v4l2_.uninit_device();
    return 0;
}

void *key_thread(void *threadarg)
{
    while(1)
    {
        int key;
        struct termios new_settings;
        struct termios stored_settings;
        tcgetattr(0,&stored_settings);
        new_settings = stored_settings;
        new_settings.c_lflag &= (~ICANON);
        new_settings.c_cc[VTIME] = 0;
        tcgetattr(0,&stored_settings);
        new_settings.c_cc[VMIN] = 1;
        tcsetattr(0,TCSANOW,&new_settings);
        
        key = getchar();
        
        tcsetattr(0,TCSANOW,&stored_settings);

        if(key == K_ || key == K__)
        {
            isStartRecording=true;
        }
        else if(key == T_ || key == T__)
        {
            isStartRecording=false;
        }
        else if(key == LEFT || key == LEFT_)
        {
            quit=true;
            break;
        }
    }

}

void *v4l2_thread(void *threadarg)
{
    VideoWriter outputVideo;
    bool caping=false;

	while (1)
	{
        if(!caping&&isStartRecording)
		{
			string videoFileName="../data/";
			videoFileName+=getCurrentDate();
			videoFileName+=".avi";
			outputVideo.open(videoFileName, VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(video_width, video_height));// 配置输出视频文件
			externShowLog=videoFileName;
			caping=true;

		}
        v4l2_.read_frame(rgb);
        if(caping)
		{
			outputVideo.write(rgb);// 将该帧写入视频文件
			circle(rgb,Point(20,20),4,Scalar(0,0,255),20);
		}
        if(!isStartRecording&&caping)
		{
			caping=false;
			outputVideo.release();
		}
        putText(rgb,log_fps.c_str(),Point(50,20),FONT_HERSHEY_COMPLEX,0.5,Scalar(0, 255, 0),1);
		putText(rgb,log_w.c_str(),Point(50,40),FONT_HERSHEY_COMPLEX,0.5,Scalar(0, 255, 0),1);
		putText(rgb,log_h.c_str(),Point(50,60),FONT_HERSHEY_COMPLEX,0.5,Scalar(0, 255, 0),1);
		putText(rgb,externShowLog.c_str(),Point(50,80),FONT_HERSHEY_COMPLEX,0.5,Scalar(0, 255, 0),1);

        screen_.show_bgr_mat_at_screen(rgb,screen_pos_x,screen_pos_y);
        if (quit)
            break;
    }
}