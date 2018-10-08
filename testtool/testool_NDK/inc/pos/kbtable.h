/*
 * FileName:       
 * Author:         zhangnw  Version: 3.0.0  Date: 2012-12-26
 * Description:  
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version>  <desc>
 */
//#ifndef _KB_H_
//#define _KB_H_

/*---------------constants/macro definition---------------------*/

/*------------global variables definition-----------------------*/
	//��һ�ż��������ı�(ʹ�ò�Ʒ��,��������)
	KeyTable table[] =
	{
#if	defined GP730||defined GP730CP||defined GP730TOUCH||defined SPW70
		{"���ϼ�ͷ", KEY_F1}, {"�˵�", KEY_MENU}, {"���¼�ͷ", KEY_F2}, 
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"С����", DOT}, {"��ĸ", KEY_ALPHA}, 
#elif defined GP720
		{"F1", KEY_F1}, {"F2", KEY_F2}, {"F3", KEY_F3}, {"F4", KEY_F4},
		{"���ϼ�ͷ", KEY_UP}, {"���¼�ͷ", KEY_DOWN}, {"�����ͷ", LEFT}, {"���Ҽ�ͷ", RIGHT},	
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"С����", DOT}, {"��ĸ", KEY_ALPHA}, 
#elif defined ME31||defined SP10
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"F1/���ϼ�ͷ", KEY_F1}, {"F2/���¼�ͷ", KEY_F2}, 
#elif defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME30GX||defined ME32||defined ME32GX||defined ME32THM//14���Ĳ�Ʒ
	 	{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"���ܼ�",KEY_F1}, 
#elif defined ME50S//15���Ĳ�Ʒ
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"#��", KEY_ALPHA}, {"*��", DOT},
#elif defined ME50//15���Ĳ�Ʒ
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"#��/���¼�ͷ", KEY_F2}, {"*��/���ϼ�ͷ", KEY_F1},
#elif defined ME50C||defined ME50H//19���Ĳ�Ʒ
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC},  {"���ܼ�",KEY_F3/*K_FUNC*/}, {"�˵���", K_MENU},{"�ػ���",KEY_F5}, {"#��/���¼�ͷ", K_F2}, {"*��/���ϼ�ͷ", K_F1}, {"+��", K_ADD},		
#elif defined ME50N||defined ME50NX   //||defined ME68//16���Ĳ�Ʒ  ME68û�й��ܼ� 20180416 sull modify
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"���ܼ�",KEY_F3},  {"#��/���¼�ͷ", KEY_F2}, {"*��/���ϼ�ͷ", KEY_F1},
#elif defined ME68
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC},  {"#��/���¼�ͷ", KEY_F2}, {"*��/���ϼ�ͷ", KEY_F1},
#elif ANDIROD_ENABLE//�߶˲�Ʒ//defined IM81||defined N900||
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, 
#elif defined SP80||defined SP80G||defined NL8510GP||defined SP930G||defined SP930PG||defined SP830PG||CPU5830X_ENABLE||defined SP610//18���Ĳ�Ʒ
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"F1/���ϼ�ͷ", KEY_F1}, {"F2/���¼�ͷ", KEY_F2}, {"�˵�/F3", KEY_F3},  {"С����", DOT}, {"��ĸ", KEY_ALPHA}, 
#elif defined SP630 || defined SP630PG
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"F1", KEY_F1}, {"F2", KEY_F2}, {"F3", KEY_F3}, {"F4", KEY_F4},  {"��Դ", KEY_MENU}, {"С����", DOT}, {"��ĸ", KEY_ALPHA}, 
#elif defined ME62 //20����Դ���޼�ֵ
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"С����", DOT}, {"#��", KEY_ALPHA},
		{"F1", KEY_F1}, {"F2", KEY_F2}, {"F3", KEY_F3}, {"F4", KEY_F4},
#else	
		//Ĭ�ϵĲ�Ʒ��19������9�����ܼ���10�����ּ�,����SP60,SP50,ME31S,SP33,SP600,�Լ���Ӧ�ĺ����Ʒ
		{"�˸�", BACKSPACE}, {"ȷ��", ENTER }, {"ȡ��", ESC}, {"С����", DOT}, {"��ĸ", KEY_ALPHA},
		{"F1/���ϼ�ͷ", KEY_F1}, {"F2/���¼�ͷ", KEY_F2}, {"�˵�/F3", KEY_F3}, {"��Դ", KEY_F4},
#endif
		//��ΪPOS��Ʒ,���۾�����ʲô��Ʒ,0-9�Ǳض����߱���
		{"����0", '0'}, {"����1", '1'}, {"����2", '2'}, {"����3", '3'}, {"����4", '4'}, 
		{"����5", '5'}, {"����6", '6'}, {"����7", '7'}, {"����8", '8'}, {"����9", '9'}
	};

#if defined ME50C  //ME50C����PS2���̣���������  20180523  sull  add
	//������߶�Ӧ�����ֱ�
	KeyTable ps2_num [] = 
	{
		{"����0", '0'}, {"����1", '1'}, {"����2", '2'}, {"����3", '3'}, {"����4", '4'}, 
		{"����5", '5'}, {"����6", '6'}, {"����7", '7'}, {"����8", '8'}, {"����9", '9'}
	};
	//������߶�Ӧ�Ĵ�д��ĸ��
	KeyTable ps2_big_letter [] = 
	{
		{"��д��ĸA", 'A'}, {"��д��ĸB", 'B'}, {"��д��ĸC", 'C'}, {"��д��ĸD", 'D'}, {"��д��ĸE", 'E'}, 
		{"��д��ĸF", 'F'}, {"��д��ĸG", 'G'}, {"��д��ĸH", 'H'}, {"��д��ĸI", 'I'}, {"��д��ĸJ", 'J'},
		{"��д��ĸK", 'K'}, {"��д��ĸL", 'L'}, {"��д��ĸM", 'M'}, {"��д��ĸN", 'N'}, {"��д��ĸO", 'O'}, 
		{"��д��ĸP", 'P'}, {"��д��ĸQ", 'Q'}, {"��д��ĸR", 'R'}, {"��д��ĸS", 'S'}, {"��д��ĸT", 'T'},
		{"��д��ĸU", 'U'}, {"��д��ĸV", 'V'}, {"��д��ĸW", 'W'}, {"��д��ĸX", 'X'}, {"��д��ĸY", 'Y'}, 
		{"��д��ĸZ", 'Z'}
	};
	//������߶�Ӧ��Сд��ĸ��
	KeyTable ps2_small_letter [] = 
	{
		{"Сд��ĸa", 'a'}, {"Сд��ĸb", 'b'}, {"Сд��ĸc", 'c'}, {"Сд��ĸd", 'd'}, {"Сд��ĸe", 'e'}, 
		{"Сд��ĸf", 'f'}, {"Сд��ĸg", 'g'}, {"Сд��ĸh", 'h'}, {"Сд��ĸi", 'i'}, {"Сд��ĸj", 'j'}, 
		{"Сд��ĸk", 'k'}, {"Сд��ĸl", 'l'}, {"Сд��ĸm", 'm'}, {"Сд��ĸn", 'n'}, {"Сд��ĸo", 'o'}, 
		{"Сд��ĸp", 'p'}, {"Сд��ĸq", 'q'}, {"Сд��ĸr", 'r'}, {"Сд��ĸs", 's'}, {"Сд��ĸt", 't'}, 
		{"Сд��ĸu", 'u'}, {"Сд��ĸv", 'v'}, {"Сд��ĸw", 'w'}, {"Сд��ĸx", 'x'}, {"Сд��ĸy", 'y'}, 
		{"Сд��ĸz", 'z'}
	};
	//������߶�Ӧ�ķ��ű�
	KeyTable ps2_char []  = 
	{
		{"����)", ')'}, {"����!", '!'}, {"����@", '@'}, {"����#", '#'}, {"����$", '$'}, 
		{"����%", '%'}, {"����^", '^'}, {"����&", '&'}, {"����*", '*'}, {"����(", '('},
		{"����~", '~'}, {"����_", '_'}, {"����+", '+'}, {"����|", '|'}, {"�ո��", ' '}, 
		{"����{", '{'}, {"����}", '}'}, {"����:", ':'}, {"˫����", '"'}, {"����<", '<'},
		{"����>", '>'}, {"����?", '?'}, {"����.", '.'}, {"����`", '`'}, {"����-", '-'},
		{"����=", '='}, {"����[", '['}, {"����]", ']'}, {"����;", ';'}, {"��б��", PS2_KEY_CHAR1}, 
		{"����,", ','}, {"����/", '/'}
	};
	//���̶�Ӧ��F ����
	KeyTable ps2_f [] = 
	{
		{"F1", PS2_KEY_F1}, {"F2", PS2_KEY_F2}, {"F3", PS2_KEY_F3}, {"F4", PS2_KEY_F4}, 
		{"F5", PS2_KEY_F5}, {"F6", PS2_KEY_F6}, {"F7", PS2_KEY_F7}, {"F8", PS2_KEY_F8}, 
		{"F9", PS2_KEY_F9}, {"F10", PS2_KEY_F10}, {"F11", PS2_KEY_F11}, {"F12", PS2_KEY_F12} 
	};
	//�����ұ߶�Ӧ�ı�
	KeyTable ps2_right [] = 
	{
		{"����0", '0'}, {"����1", '1'}, {"����2", '2'}, {"����3", '3'}, {"����4", '4'}, 
		{"����5", '5'}, {"����6", '6'}, {"����7", '7'}, {"����8", '8'}, {"����9", '9'},
		{"����+", '+'}, {"����-", '-'}, {"����*", '*'}, {"����/", '/'}, {"Enter", PS2_KEY_EN},
		{"����.", '.'}, {"Num Lock", PS2_KEY_NUM}	
	};	
	//�������ⰴ��
	KeyTable ps2_other [] = 
	{
		{"Backspace", PS2_KEY_BKSP}, {"�ո�", PS2_KEY_SPACE}, {"TAB", PS2_KEY_TAB},  
		{"Caps Lock", PS2_KEY_CAPS}, {"��Ctrl", PS2_KEY_L_CTRL}, 
		{"��GUI", PS2_KEY_L_GUI}, {"��A/t", PS2_KEY_L_ALT},  
		{"��Ctrl", PS2_KEY_R_CTRL}, {"��GUI", PS2_KEY_R_GUI}, {"��A/t", PS2_KEY_R_ALT}, 
		{"APPS", PS2_KEY_APPS}, {"Enter", PS2_KEY_ENTER}, {"Esc", PS2_KEY_ESC}, 
		{"Print Screen", PS2_KEY_SCRN}, {"Scroll Lock", PS2_KEY_SCROLL}, {"Pause", PS2_KEY_PAUSE}, 
		{"Insert", PS2_KEY_INSERT}, {"Home", PS2_KEY_HOME}, {"Page Up", PS2_KEY_PG_UP},
		{"Delete", PS2_KEY_DELETE}, {"End", PS2_KEY_END}, {"Page Down", PS2_KEY_PG_DN},
		{"����(����)", PS2_KEY_U_ARROW}, {"����(����)", PS2_KEY_L_ARROW}, {"����(����)", PS2_KEY_D_ARROW},
		{"����(����)", PS2_KEY_R_ARROW}
	};				
#endif

//#endif

