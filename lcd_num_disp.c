#include "iodefine.h"
#include  "misratypes.h"

#include "ILI9488.h"

#include "font_8_16.h"
#include "font_48_96.h"

#include "lcd_num_disp.h"


//
// �����̕\���e�X�g
// 
// ����:     dig :�\������ 0�`9
//       size_id : 0=8(W)x16(H)�ŕ\��, 1=48(W)x96(H)�ŕ\�� 
//
//�@���W:
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
//             �B �B�@       �B �B�@       
//�@�@�@�@�@�@VCC GND          T_IRQ
//

void disp_num_test(uint8_t dig, uint8_t size_id)
{
	uint8_t para[1];
	
	uint8_t color;		//  (0:��, 1:��, 2:��:, 3:��, 4:��, 5:�V�A��, 6:�}�[���^)

	
	uint32_t st_col;
	uint32_t st_row;
	
	uint32_t end_col;
	uint32_t end_row;
	
	
	color = 1;		// �\���F= ��
	
	
	st_col = 0;		// �J�n�J����
	st_row = 0;		// �J�n�s
	
	
	
	if ( size_id == 0 ) {	// 8(W)x16(H)�̏ꍇ
		end_col = 7;	// �I���J����
		end_row = 15;	// �I���s
	}
	else {			// 48(W)x96(H)�̏ꍇ
		end_col = 47;	// �I���J����
		end_row = 95;	// �I���s
	}
	
	
	
	lcd_adrs_set(st_col, st_row, end_col, end_row );	 // �������ݔ͈͎w�� 
	
	spi_cmd_send(0x2c,0,&para[0]);		// Memory Write (2Ch)  �擪�ʒu(�R�}���h2a,2b�Ŏw�肵���ʒu)����f�[�^��������
	
	spi_data_send_id(dig,size_id,color);  	// �f�[�^���M�@	
	
	
	
}

//  8x16 �܂���48 x 96 pixel �̃f�[�^���M
//  ����: index:  ���M����f�[�^
//     size_id:	0= 8(W)x16(H)�ŕ\��, 1= 48(W)x96(H)�ŕ\��    
//    color  :�����̕\���F(0:��, 1:��, 2:��:, 3:��, 4:��, 5:�V�A��, 6:�}�[���^)
//
//  a ) size_id  = 0 ( 8 x 16 pixel) �̏ꍇ:
//  �t�H���g�f�[�^�̃T�C�Y(byte):  16 = 8/8 * 16
//  ���M�f�[�^�� (byte):�@ 64 = 8/2 * 16    (1pixel = 2byte) 
//
//  b) size_id  = 1 ( 48 x 96 pixel) �̏ꍇ:
//  �t�H���g�f�[�^�̃T�C�Y(byte):  576 = 48/8 * 96
//  ���M�f�[�^�� (byte)�@2304 = 48/2 *96
//    (1pixel = 2byte) 

//  index  �\������
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
 	
	uint32_t len;    // �t�H���g�̃f�[�^�T�C�Y�@(pack �`��)
	uint32_t num;    //  ���M�f�[�^��

	if ( size_id == 0 ) {			// �t�H���g�T�C�Y 8(W)x16(H)�̏ꍇ				
		pt = (uint8_t *)&font_8w_16h_seg[index][0];  // �t�H���g�f�[�^�̊i�[�A�h���X
	        len = 16;
		num = 64;
		
	}
	else {					// �t�H���g�T�C�Y 48(W)x96(H)�̏ꍇ						
		pt = (uint8_t *)&font_48w_96h_seg[index][0];  // �t�H���g�f�[�^�̊i�[�A�h���X
	   	len = 576;
		num = 2304;
	}

	unpack_font_data( len ,pt , color );  // �t�H���g�f�[�^��RGB111(rgb111_data_bur)�֓W�J 
	
	spi_data_send(num, &rgb111_data_buf[0],0);  // �s�N�Z���f�[�^���M

}



// �r�b�g�}�b�v�̃t�H���g�f�[�^(1byte�P��)���ARGB111�ɕϊ����āA
// �\���p�̃o�b�t�@ �ɃZ�b�g
//  ����: * ptt: �t�H���g�f�[�^�̐擪�A�h���X
//         len:  �t�H���g�f�[�^�̃o�C�g��
//               (8Wx16H �Ȃ�� 16byte)
//      color_index:�����̕\���F(0:��, 1:��, 2:��:, 3:��, 4:��, 5:�V�A��, 6:�}�[���^)
//
//        
// �E�@�ׂ������r�b�g��ON/OFF ���ƕ����F�̑g���킹�ŁA�o�͂���f�[�^�͈قȂ�B
//�@�@(pn:�ŏ��̃r�b�g, pn1:���̃r�b�g) �F(index)
//    pn  pn1�@  White (0)    Green(1)      Red(2)       Blue(3)       Yellow(4)    Cyan(5)         Magenta(6)
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
		
		bd = *src_ptr;		// �r�b�g�}�b�v�f�[�^���o��
					// b7,b6�̏���
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
	
		rgb111_data_buf[pt] = cd;	// rgb111�f�[�^���i�[
		pt = pt + 1;		        // �i�[�ʒu�̃C���N�������g
		
		
						// b5,b4�̏���
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
	
		rgb111_data_buf[pt] = cd;	// rgb111�f�[�^���i�[
		pt = pt + 1;		        // �i�[�ʒu�̃C���N�������g
		
		
						// b3,b2�̏���
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
	
		rgb111_data_buf[pt] = cd;	// rgb111�f�[�^���i�[
		pt = pt + 1;		        // �i�[�ʒu�̃C���N�������g
	
		
						// b1,b0�̏���
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
	
		rgb111_data_buf[pt] = cd;	// rgb111�f�[�^���i�[
		pt = pt + 1;		        // �i�[�ʒu�̃C���N�������g
		
		src_ptr++;				// ���o���ʒu�C���N�������g
		
	}
	
}




