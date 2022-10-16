#include	<machine.h>
#include	 "iodefine.h"
#include	 "misratypes.h"
#include	"delay.h"
#include	 "spi.h"
#include	 "ILI9488.h"
#include 	"lcd_num_disp.h"

void clear_module_stop(void);

void main(void)
{
	
	clear_module_stop();	//  モジュールストップの解除
	
	RSPI_Init_Port();	// RSPI ポートの初期化  (LCDコントローラ用)   
     	RSPI_Init_Reg();        // SPI レジスタの設定  

     	RSPI_SPCMD_0();	        // SPI 転送フォーマットを設定, SSLA0使用	
	
	ILI9488_Reset();	// LCD のリセット	
	 
	ILI9488_Init();		// LCDの初期化
	
	disp_black();		// 画面　黒  ( 80 [msec] at 16[MHz] ) 80[msecc]	
	
	while(1){
		
		//disp_num_test(3,0);	// 数字 3 , フォントサイズ(8x16) 表示
		
		disp_num_test(7,1);	// 数字 7 , フォントサイズ(48x96) 表示
		
		delay_msec(500);
	
	}
	
}





// モジュールストップの解除
// コンペアマッチタイマ(CMT) ユニット0(CMT0, CMT1) 
//   シリアルペリフェラルインタフェース0(RSPI)
//
void clear_module_stop(void)
{
	SYSTEM.PRCR.WORD = 0xA50F;	// クロック発生、消費電力低減機能関連レジスタの書き込み許可	
	
	MSTP(CMT0) = 0;			// コンペアマッチタイマ(CMT) ユニット0(CMT0, CMT1) モジュールストップの解除
	
	MSTP(RSPI0) = 0;		// シリアルペリフェラルインタフェース0 モジュールストップの解除
	
	SYSTEM.PRCR.WORD = 0xA500;	// クロック発生、消費電力低減機能関連レジスタ書き込み禁止
}

