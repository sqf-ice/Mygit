#ifndef GUIUTIL_H_
#define GUIUTIL_H_

#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/mman.h>

#include <linux/input.h>
/**
 * @ingroup list
 * @{
 */
/**
 * ͨ������ڵ�ṹ
 */
typedef struct {
	void * next;    /**< ָ����һ���ڵ� */
	void * element; /**< ָ��ڵ��ŵ������� */
}list_node_t;


typedef enum {
	POS_LEFT      = 0x00000001,
	POS_HCENTER   = 0x00000002,
	POS_RIGHT     = 0x00000004,
	POS_UP        = 0x00000008,
	POS_DOWN      = 0x00000010,
	POS_VCENTER   = 0x00000020,
	POS_CENTER    = 0x00000022,
	POS_VERT	  = 0x00000005,
	POS_HORIZ     = 0x00000018,
}position_t;

/**
 * ͨ������ͷ�ڵ�ṹ
 */
typedef struct {
	int num_elt;		/**< �ڵ����� */
	list_node_t * node; /**< ָ���һ����� */
}list_t;

/**
 * �ؼ��������е���Ϣ
 */
typedef struct {
	void * current_widget;
	int current_position;
}widget_list;


int list_init(list_t ** li);
int list_uninit(list_t * li);
int list_eol(list_t * li, int i);
list_node_t * list_get_node(list_t * li, int pos);
void * list_get(list_t * li, int pos);
void * list_remove(list_t * li, int pos);
int list_add(list_t * li, void * element, int pos);
void * list_find(list_t * li, int (*cmp_func) (void * data, void * node), void * data);
int list_get_pos(list_t * li, void * node);
int list_for_each(list_t * li, void (*call)(void * data, void * element), void * data);
int list_for_each_breakoff(list_t * li, int (*call)(void * data, void * element), void * data);
widget_list list_find_down(list_t * li, int pos,int (*call)(void * data, void * element), void * data);
widget_list list_find_up(list_t * li, int pos,int (*call)(void * data, void * element), void * data);
int list_cleanup(list_t * li, void (*callback)(void * data, void * element), void * data, int freehead);

typedef struct {
	int x;
	int y;
	int w;
	int h;
}rect_t;

typedef struct {
	char *name;		/**< ������ */
	char *value;	/**< ����ֵ */
}config_var_t;

typedef struct {
	char *name;			/**< ����� */
	list_t *variable;	/**< �����б� */
}config_cat_t;

typedef struct {
	list_t *category;	/**< ������� */
}config_t;

int config_initialize (config_t **cfg);
int config_parse (config_t *cfg, char *name);
int config_uninit (config_t *cfg);
int config_category_uninit (config_cat_t *cat);
int config_variable_uninit (config_var_t *var);
int config_category_init (config_cat_t **cat, char *name);

typedef unsigned int   uint32;
typedef unsigned char  uint8;
typedef unsigned short uint16;

typedef uint32 color_t; /**< ��ɫ��ʾ������Ҫ������ʾģ�����ɫģʽ�ϳ� */
void color_rgb(color_t c, int *r, int *g, int *b);
color_t rgb_color(int r, int g, int b);

/**
 * ͼ�����ݽṹ
 * ���ݽ���⣬��libpng��libjpeg�����֧��24λɫ����32λɫ��
 * ����ͨ��8λɫ����ڻ������Щ��ɫģʽ�Ļᱻת��
 * ��ˣ�bytes_per_pixel�����ֽڶ��루2��3�ֽڣ�
 * ��ʾģ����ܻ��޸ĸ����ݽṹ���磺
 * ������ʾ��������24����32λɫ��װ��Ϊ���ص�ɫ��棬��
 * �����32λɫ���ʾģ����Խ�image_buf�е�alphaͨ�����뵽image_alpha��
 */
typedef struct {
	uint32 width;  			/**< ͼ���ȣ����أ� */
	uint32 height; 			/**< ͼ��߶ȣ����أ�*/
	uint32 bytes_per_pixel; /**< ÿ�����ض����ֽ� */
	void * image_buf;		/**< ͼ��ɫ������ */
	void * image_alpha;		/**< ͼ��alphaͨ�� */
	int    user;			/**< ͼ�񱻹���Ĵ��� */
}image_t;

image_t * image_decode(char * filepath);
int image_encode(image_t * pimage, char * filepath, char * type);
void image_destroy(image_t * pimage);
image_t * image_share(image_t * pimage);

int init_gui(void);
void uninit_gui(void);
int init_guiforapp(unsigned char*appname);

#include "widget.h"

int pt_in_rect(rect_t * r, int x, int y);
void rect_inflate(rect_t * r, int dx, int dy);
void rect_deflate(rect_t * r, int dx, int dy);
int rect_intersect(rect_t * r, rect_t * r1, rect_t * r2);
void rect_set(rect_t * rect, int x, int y, int w, int h);
void rect_offset(rect_t * r, int x, int y);
void rect_position(rect_t * contain, int w, int h, rect_t * pos, position_t where);
int rect_is_empty(rect_t * rect);
int rect_equal(rect_t * r1, rect_t * r2);
void rect_range(rect_t * con, image_t * image, int strw, int strh, rect_t * img, rect_t * str, position_t pos,int inner);


int get_lcdtype(char * type);
int get_screensize(unsigned int * width,unsigned int * height);
int detect_colorscreen(void);

#define EKEY_ESC          0x1B
#define EKEY_ALPHA        0x1C
#define EKEY_ENTER        0x0D
#define EKEY_DOT          0x2E
#define EKEY_BACKSPACE    0x0A

#define LCDBUF_TOP		  0
#define LCDBUF_BOTTOM     1

#define EKEY_F1           0x01
#define EKEY_F4           0x04
#define EKEY_F2           0x02
#define EKEY_F3           0x03
#define EKEY_MENU		EKEY_F3	
#if defined(CONFIG_GP730_MONO)||defined(CONFIG_GP730_COLOR)
#define EKEY_UP			EKEY_F1
#define EKEY_DOWN		EKEY_F2
#define EKEY_LEFT       EKEY_F1
#define EKEY_RIGHT		EKEY_F2
#endif

#ifdef CONFIG_NLGP720
#define EKEY_UP			0x11
#define EKEY_DOWN		0x12
#define EKEY_LEFT       0x13
#define EKEY_RIGHT		0x14
#endif

#ifdef CONFIG_NLGP750
#define EKEY_UP			EKEY_F2
#define EKEY_DOWN		EKEY_F3
#define EKEY_LEFT       EKEY_F2
#define EKEY_RIGHT		EKEY_F3
#endif


#define EKEY_DOT_EXRA   (EKEY_DOT|0X80)
#define EKEY_ALPHA_EXRA   (EKEY_ALPHA|0X80)

#endif
