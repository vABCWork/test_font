#include "iodefine.h"
#include  "misratypes.h"

#include "ILI9488.h"

#include "font_8_16.h"
#include "font_48_96.h"

#include "lcd_num_disp.h"


//
// 数字の表示テスト
// 
// 入力:     dig :表示数字 0～9
//       size_id : 0=8(W)x16(H)で表示, 1=48(W)x96(H)で表示 
//
//　座標:
//                  Clolumn
//            0                 319
//          0 +------------------+
// Page(Row)  |                  |
//            |                  |
//            |                  |
//            |                  |
//            |                  |
//            |                  |
//            |                  |    
//            |                  |
//            |                  |
//            |                  |
//            |                  |
//       479  |                  |
//            +------------------+
//             。 。　       。 。　       
//　　　　　　VCC GND          T_IRQ
//

void disp_num_test(uint8_t dig, uint8_t size_id)
{
	uint8_t para[1];
	
	uint8_t color;		//  (0:白, 1:緑, 2:赤:, 3:青, 4:黄, 5:シアン, 6:マゼンタ)

	
	uint32_t st_col;
	uint32_t st_row;
	
	uint32_t end_col;
	uint32_t end_row;
	
	
	color = 1;		// 表示色= 緑
	
	
	st_col = 0;		// 開始カラム
	st_row = 0;		// 開始行
	
	
	
	if ( size_id == 0 ) {	// 8(W)x16(H)の場合
		end_col = 7;	// 終了カラム
		end_row = 15;	// 終了行
	}
	else {			// 48(W)x96(H)の場合
		end_col = 47;	// 終了カラム
		end_row = 95;	// 終了行
	}
	
	
	
	lcd_adrs_set(st_col, st_row, end_col, end_row );	 // 書き込み範囲指定 
	
	spi_cmd_send(0x2c,0,&para[0]);		// Memory Write (2Ch)  先頭位置(コマンド2a,2bで指定した位置)からデータ書き込み
	
	spi_data_send_id(dig,size_id,color);  	// データ送信　	
	
	
	
}

//  8x16 または48 x 96 pixel のデータ送信
//  入力: index:  送信するデータ
//     size_id:	0= 8(W)x16(H)で表示, 1= 48(W)x96(H)で表示    
//    color  :文字の表示色(0:白, 1:緑, 2:赤:, 3:青, 4:黄, 5:シアン, 6:マゼンタ)
//
//  a ) size_id  = 0 ( 8 x 16 pixel) の場合:
//  フォントデータのサイズ(byte):  16 = 8/8 * 16
//  送信データ数 (byte):　 64 = 8/2 * 16    (1pixel = 2byte) 
//
//  b) size_id  = 1 ( 48 x 96 pixel) の場合:
//  フォントデータのサイズ(byte):  576 = 48/8 * 96
//  送信データ数 (byte)　2304 = 48/2 *96
//    (1pixel = 2byte) 

//  index  表示文字
//    0       0
//    1       1 
//    2       2 
//    3       3
//    4       4
//    5       5
//    6       6
//    7       7
//    8       8
//    9       9
//
void spi_data_send_id(uint8_t index, uint8_t size_id ,uint8_t color ) {
	
	uint8_t *pt;
 	
	uint32_t len;    // フォントのデータサイズ　(pack 形式)
	uint32_t num;    //  送信データ数

	if ( size_id == 0 ) {			// フォントサイズ 8(W)x16(H)の場合				
		pt = (uint8_t *)&font_8w_16h_seg[index][0];  // フォントデータの格納アドレス
	        len = 16;
		num = 64;
		
	}
	else {					// フォントサイズ 48(W)x96(H)の場合						
		pt = (uint8_t *)&font_48w_96h_seg[index][0];  // フォントデータの格納アドレス
	   	len = 576;
		num = 2304;
	}

	unpack_font_data( len ,pt , color );  // フォントデータをRGB111(rgb111_data_bur)へ展開 
	
	spi_data_send(num, &rgb111_data_buf[0],0);  // ピクセルデータ送信

}



// ビットマップのフォントデータ(1byte単位)を、RGB111に変換して、
// 表示用のバッファ にセット
//  入力: * ptt: フォントデータの先頭アドレス
//         len:  フォントデータのバイト数
//               (8Wx16H ならば 16byte)
//      color_index:文字の表示色(0:白, 1:緑, 2:赤:, 3:青, 4:黄, 5:シアン, 6:マゼンタ)
//
//        
// ・　隣あったビットのON/OFF 情報と文字色の組合わせで、出力するデータは異なる。
//　　(pn:最初のビット, pn1:次のビット) 色(index)
//    pn  pn1　  White (0)    Green(1)      Red(2)       Blue(3)       Yellow(4)    Cyan(5)         Magenta(6)
//     0  0      0x00          0x00         0x00         0x00           0x00         0x00            0x00
//     1  0      0x38          0x10         0x20         0x08           0x30         0x18            0x28
//     0  1      0x07          0x02         0x04         0x01           0x06         0x03            0x05
//     1  1      0x3f          0x12         0x24         0x09           0x36         0x1b            0x2d
  


const uint8_t b2_on_off[7] = {0x38, 0x10, 0x20, 0x08, 0x30, 0x18, 0x28 };  
const uint8_t b2_off_on[7] = {0x07, 0x02, 0x04, 0x01, 0x06, 0x03, 0x05 };  
const uint8_t b2_on_on[7]  = {0x3f, 0x12, 0x24, 0x09, 0x36, 0x1b, 0x2d };


void unpack_font_data ( uint32_t len, uint8_t * src_ptr , uint8_t color_index )
{
	uint8_t  bd;
	uint8_t  bdn;
	uint8_t  cd;
	uint32_t i;
	uint32_t pt;
	
	pt = 0;
	
	for ( i = 0; i < len  ; i++ ){
		
		bd = *src_ptr;		// ビットマップデータ取り出し
					// b7,b6の処理
		if (( bd & 0xc0 ) == 0xc0 ) {    //  ON,ON (b7=1,b6=1)
		    cd = b2_on_on[color_index];
		}
		else if (( bd & 0xc0 ) == 0x80 ) {  // ON,OFF (b7=1,b6=0)
		    cd = b2_on_off[color_index];
		}
		else if (( bd & 0xc0) == 0x40 ){	  // OFF,ON (b7=0,b7=1)
		    cd = b2_off_on[color_index];
		}
		else if (( bd & 0xc0) == 0x00 ){	  // OFF,OFF (b7=0,b7=0)
		    cd = 0x00;
		}
	
		rgb111_data_buf[pt] = cd;	// rgb111データを格納
		pt = pt + 1;		        // 格納位置のインクリメント
		
		
						// b5,b4の処理
		if ((bd & 0x30 ) == 0x30 ) {    //  ON,ON (b5=1,b4=1)
		    cd = b2_on_on[color_index];
		}
		else if (( bd & 0x30 ) == 0x20 ) {  // ON,OFF (b5=1,b4=0)
		    cd = b2_on_off[color_index];
		}
		else if (( bd & 0x30) == 0x10 ){	  // OFF,ON (b5=0,b4=1)
		    cd = b2_off_on[color_index];
		}
		else if (( bd & 0x30) == 0x00 ){	  // OFF,OFF (b5=0,b4=0)
		    cd = 0x00;
		}
	
		rgb111_data_buf[pt] = cd;	// rgb111データを格納
		pt = pt + 1;		        // 格納位置のインクリメント
		
		
						// b3,b2の処理
		if ((bd & 0x0c ) == 0x0c ) {    //  ON,ON (b3=1,b2=1)
		    cd = b2_on_on[color_index];
		}
		else if (( bd & 0x0c ) == 0x08 ) {  // ON,OFF (b3=1,b2=0)
		    cd = b2_on_off[color_index];
		}
		else if ((bd & 0x0c) == 0x04 ){	  // OFF,ON (b3=0,b2=1)
		    cd = b2_off_on[color_index];
		}
		else if ((bd & 0x0c) == 0x00 ){	  // OFF,OFF (b3=0,b2=0)
		    cd = 0x00;
		}
	
		rgb111_data_buf[pt] = cd;	// rgb111データを格納
		pt = pt + 1;		        // 格納位置のインクリメント
	
		
						// b1,b0の処理
		if ((bd & 0x03 ) == 0x03 ) {    //  ON,ON (b1=1,b0=1)
		    cd = b2_on_on[color_index];
		}
		else if ((bd & 0x03 ) == 0x02 ) {  // ON,OFF (b1=1,b0=0)
		    cd = b2_on_off[color_index];
		}
		else if ((bd & 0x03) == 0x01 ){	  // OFF,ON (b1=0,b0=1)
		    cd = b2_off_on[color_index];
		}
		else if ((bd & 0x03) == 0x00 ){	  // OFF,OFF (b1=0,b0=0)
		    cd = 0x00;
		}
	
		rgb111_data_buf[pt] = cd;	// rgb111データを格納
		pt = pt + 1;		        // 格納位置のインクリメント
		
		src_ptr++;				// 取り出し位置インクリメント
		
	}
	
}




